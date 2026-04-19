#!/usr/bin/env python3
"""
Dexcom G7 -> QMK Raw HID bridge

Polls Dexcom Share for glucose readings via pydexcom and broadcasts them
to every connected VIA-compatible keyboard (e.g. the dilemma 4x6_4_procyon
and the killerwhale duo).

Requirements:
    pip install hidapi pydexcom

Usage:
    python dexcom_g7_hid.py --username YOUR_DEXCOM_USER --password YOUR_DEXCOM_PASS

    Or set environment variables:
        DEXCOM_USERNAME, DEXCOM_PASSWORD

    For non-US accounts, add: --ous
"""

import argparse
import hid
import os
import struct
import sys
import time
from datetime import datetime

from pydexcom import Dexcom, Region

# VIA HID identifiers
VIA_USAGE_PAGE = 0xFF60
VIA_USAGE_ID   = 0x61

# Protocol constants (must match g7_sync.h)
VIA_CMD_CUSTOM_SET = 0x07
G7_VIA_CHANNEL     = 0xA0
G7_CMD_READING     = 0x01
G7_CMD_CONFIG      = 0x02
G7_CMD_HISTORY     = 0x03

G7_UNIT_MGDL = 0
G7_UNIT_MMOL = 1

# pydexcom trend string -> our trend code mapping
TREND_MAP = {
    "":              0,
    "double_up":     1,
    "single_up":     2,
    "forty_five_up": 3,
    "flat":          4,
    "forty_five_down": 5,
    "single_down":   6,
    "double_down":   7,
    "not_computable": 8,
    "rate_out_of_range": 9,
}

HID_REPORT_SIZE = 32


RESCAN_INTERVAL = 5  # seconds between scans for newly plugged-in keyboards


def enumerate_keyboards():
    """Return a list of all VIA-compatible keyboards currently enumerated."""
    return [
        dev for dev in hid.enumerate()
        if dev["usage_page"] == VIA_USAGE_PAGE and dev["usage"] == VIA_USAGE_ID
    ]


def describe(dev_info):
    mfr = dev_info.get("manufacturer_string") or "?"
    prod = dev_info.get("product_string") or "?"
    return f"{mfr} {prod}"


def open_device(dev_info):
    """Open an HID device. Returns the device handle or None on failure."""
    try:
        device = hid.device()
        device.open_path(dev_info["path"])
        device.set_nonblocking(True)
        return device
    except OSError as e:
        print(f"  failed to open {describe(dev_info)}: {e}")
        return None


def hid_write(device, data):
    """Write to HID device, returning False if disconnected."""
    try:
        device.write(data)
        return True
    except OSError:
        return False


def build_reading_packet(mgdl, trend, minutes_ago):
    buf = bytearray(HID_REPORT_SIZE)
    buf[0] = VIA_CMD_CUSTOM_SET
    buf[1] = G7_VIA_CHANNEL
    buf[2] = G7_CMD_READING
    struct.pack_into("<H", buf, 3, mgdl)
    buf[5] = trend
    buf[6] = min(minutes_ago, 255)
    return bytes(buf)


def build_config_packet(unit=G7_UNIT_MMOL, low=70, high=180):
    buf = bytearray(HID_REPORT_SIZE)
    buf[0] = VIA_CMD_CUSTOM_SET
    buf[1] = G7_VIA_CHANNEL
    buf[2] = G7_CMD_CONFIG
    buf[3] = unit
    struct.pack_into("<H", buf, 4, low)
    struct.pack_into("<H", buf, 6, high)
    return bytes(buf)


def build_history_packet(start_idx, values):
    """values = list of mgdl/2 uint8 values"""
    buf = bytearray(HID_REPORT_SIZE)
    buf[0] = VIA_CMD_CUSTOM_SET
    buf[1] = G7_VIA_CHANNEL
    buf[2] = G7_CMD_HISTORY
    buf[3] = start_idx
    count = min(len(values), HID_REPORT_SIZE - 5)
    buf[4] = count
    for i in range(count):
        buf[5 + i] = min(values[i], 255)
    return bytes(buf)


def send_history(device, readings):
    """Send historical readings as graph data (newest first).
    Returns False if the device disconnected during send."""
    graph_values = [min(r["mgdl"] // 2, 255) for r in readings]

    chunk_size = HID_REPORT_SIZE - 5
    for i in range(0, len(graph_values), chunk_size):
        chunk = graph_values[i:i + chunk_size]
        pkt = build_history_packet(i, chunk)
        if not hid_write(device, b'\x00' + pkt):
            return False
        time.sleep(0.02)
    return True


def get_trend_code(reading):
    """Extract trend code from a pydexcom GlucoseReading."""
    trend = getattr(reading, "trend", 0)
    return trend if isinstance(trend, int) and 0 <= trend <= 9 else 0


def get_minutes_ago(reading):
    """Calculate how many minutes ago a reading was taken."""
    dt = getattr(reading, "datetime", None)
    if dt is None:
        return 0
    delta = datetime.now(dt.tzinfo) - dt if dt.tzinfo else datetime.now() - dt
    return min(int(delta.total_seconds() / 60), 255)


class KeyboardPool:
    """Maintains open HID handles for every detected VIA keyboard and
    re-scans for new plug-ins on a timer."""

    def __init__(self, config_pkt, history_capacity=24):
        # config_pkt is replayed to each newly connected keyboard, followed
        # by the cached history (so hot-plugged boards get the full graph).
        self.config_pkt = config_pkt
        self.history = []  # list of {"mgdl": X}, newest first
        self.history_capacity = history_capacity
        self.connections = {}  # path -> (device, label)
        self._last_scan = 0

    def set_history(self, readings):
        """Seed the cached history; newest first."""
        self.history = list(readings)[:self.history_capacity]

    def push_reading(self, mgdl):
        """Prepend a new reading if it differs from the latest cached one."""
        if self.history and self.history[0]["mgdl"] == mgdl:
            return
        self.history.insert(0, {"mgdl": mgdl})
        del self.history[self.history_capacity:]

    def sync(self, force=False):
        """Open newly-appeared keyboards and drop any that no longer enumerate."""
        now = time.monotonic()
        if not force and now - self._last_scan < RESCAN_INTERVAL:
            return
        self._last_scan = now

        current = {d["path"]: d for d in enumerate_keyboards()}

        # Drop keyboards that vanished from enumeration.
        for path in list(self.connections):
            if path not in current:
                self._close(path, reason="unplugged")

        # Open any newly-seen keyboards.
        for path, info in current.items():
            if path in self.connections:
                continue
            device = open_device(info)
            if device is None:
                continue
            label = describe(info)
            self.connections[path] = (device, label)
            print(f"Connected: {label}")
            if not self._send_init(path, device):
                self._close(path, reason="init failed")

    def _send_init(self, path, device):
        if not hid_write(device, b'\x00' + self.config_pkt):
            return False
        time.sleep(0.02)
        if self.history and not send_history(device, self.history):
            return False
        return True

    def _close(self, path, reason=""):
        entry = self.connections.pop(path, None)
        if entry is None:
            return
        device, label = entry
        try:
            device.close()
        except Exception:
            pass
        suffix = f" ({reason})" if reason else ""
        print(f"Disconnected: {label}{suffix}")

    def broadcast(self, packet):
        """Send `packet` (without report-id prefix) to every connected keyboard.
        Drops any keyboard whose write fails."""
        if not self.connections:
            return 0
        dead = []
        sent = 0
        for path, (device, _label) in self.connections.items():
            if hid_write(device, b'\x00' + packet):
                sent += 1
            else:
                dead.append(path)
        for path in dead:
            self._close(path, reason="write failed")
        return sent

    def send_history(self, readings):
        """Send history to every connected keyboard; drop any that fails."""
        if not self.connections:
            return
        dead = []
        for path, (device, _label) in self.connections.items():
            if not send_history(device, readings):
                dead.append(path)
        for path in dead:
            self._close(path, reason="history send failed")

    def __len__(self):
        return len(self.connections)


def main():
    parser = argparse.ArgumentParser(description="Dexcom G7 -> QMK HID bridge")
    parser.add_argument("--username", default=os.environ.get("DEXCOM_USERNAME"))
    parser.add_argument("--password", default=os.environ.get("DEXCOM_PASSWORD"))
    parser.add_argument("--region", default="ous", choices=["us", "ous", "jp"],
                        help="Dexcom region (default: ous)")
    parser.add_argument("--unit", default="mmol", choices=["mgdl", "mmol"])
    parser.add_argument("--low", type=int, default=70, help="Low threshold in mg/dL")
    parser.add_argument("--high", type=int, default=180, help="High threshold in mg/dL")
    parser.add_argument("--interval", type=int, default=60,
                        help="Poll interval in seconds (default: 60)")
    args = parser.parse_args()

    if not args.username or not args.password:
        print("Error: Dexcom credentials required.")
        print("Use --username/--password or set DEXCOM_USERNAME/DEXCOM_PASSWORD env vars.")
        sys.exit(1)

    unit = G7_UNIT_MMOL if args.unit == "mmol" else G7_UNIT_MGDL
    region_map = {"us": Region.US, "ous": Region.OUS, "jp": Region.JP}

    # Initialize pydexcom client
    print("Authenticating with Dexcom Share...")
    dex = Dexcom(username=args.username, password=args.password, region=region_map[args.region])
    print("Authenticated.")

    # Pre-build the config packet; it is replayed to each new keyboard on connect.
    config_pkt = build_config_packet(unit, args.low, args.high)
    pool = KeyboardPool(config_pkt)

    # Seed the pool's cached history first, so the very first keyboard that
    # connects (and any later hot-plug reconnects) immediately receives it.
    try:
        readings = dex.get_glucose_readings(max_count=24, minutes=1440)
        if readings:
            pool.set_history([{"mgdl": r.value} for r in readings])
            print(f"Cached {len(pool.history)} historical readings.")
    except Exception as e:
        print(f"Warning: Could not load history: {e}")

    print("Waiting for keyboards...")
    while len(pool) == 0:
        pool.sync(force=True)
        if len(pool) == 0:
            time.sleep(RESCAN_INTERVAL)

    print(f"Polling every {args.interval}s, broadcasting to {len(pool)} keyboard(s). "
          "Press Ctrl+C to stop.")

    last_value = None

    while True:
        pool.sync()

        try:
            reading = dex.get_current_glucose_reading()

            if reading:
                mgdl = reading.value
                trend = get_trend_code(reading)
                mins = get_minutes_ago(reading)

                if args.unit == "mmol":
                    display_val = f"{mgdl / 18.018:.1f} mmol/L"
                else:
                    display_val = f"{mgdl} mg/dL"

                pool.push_reading(mgdl)
                pkt = build_reading_packet(mgdl, trend, mins)
                pool.broadcast(pkt)

                if last_value != mgdl:
                    last_value = mgdl
                    trend_desc = getattr(reading, "trend_description", "?")
                    print(f"[{datetime.now().strftime('%H:%M')}] {display_val} "
                          f"({trend_desc}, {mins}m ago) -> {len(pool)} kb")
            else:
                print(f"[{datetime.now().strftime('%H:%M')}] No reading available.")

        except Exception as e:
            print(f"Error: {e}")
            # Re-authenticate if session expired
            if "session" in str(e).lower() or "auth" in str(e).lower():
                try:
                    dex = Dexcom(username=args.username, password=args.password,
                                 region=region_map[args.region])
                    print("Re-authenticated.")
                except Exception as re_err:
                    print(f"Re-auth failed: {re_err}")

        # Sleep in small increments so we can pick up hot-plugs quickly.
        end = time.monotonic() + args.interval
        while time.monotonic() < end:
            time.sleep(min(1.0, end - time.monotonic()))
            pool.sync()


if __name__ == "__main__":
    main()
