#!/usr/bin/env python3
"""
Dexcom G7 -> QMK Raw HID bridge

Polls Dexcom Share for glucose readings via pydexcom and sends them
to the keyboard over HID using VIA's custom value channel.

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


def find_keyboard():
    for dev in hid.enumerate():
        if dev["usage_page"] == VIA_USAGE_PAGE and dev["usage"] == VIA_USAGE_ID:
            print(f"Found: {dev['manufacturer_string']} {dev['product_string']}")
            return dev["path"]
    return None


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
    """Send historical readings as graph data (newest first)."""
    graph_values = []
    for r in readings:
        graph_values.append(min(r["mgdl"] // 2, 255))

    chunk_size = HID_REPORT_SIZE - 5
    for i in range(0, len(graph_values), chunk_size):
        chunk = graph_values[i:i + chunk_size]
        pkt = build_history_packet(i, chunk)
        device.write(b'\x00' + pkt)
        time.sleep(0.02)


def get_trend_code(reading):
    """Extract trend code from a pydexcom GlucoseReading."""
    # reading.trend returns int 0-9 matching our firmware constants directly
    trend = getattr(reading, "trend", 0)
    return trend if isinstance(trend, int) and 0 <= trend <= 9 else 0


def get_minutes_ago(reading):
    """Calculate how many minutes ago a reading was taken."""
    dt = getattr(reading, "datetime", None)
    if dt is None:
        return 0
    delta = datetime.now(dt.tzinfo) - dt if dt.tzinfo else datetime.now() - dt
    return min(int(delta.total_seconds() / 60), 255)


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

    # Find keyboard
    kb_path = find_keyboard()
    if not kb_path:
        print("Error: Could not find keyboard HID device.")
        print("Make sure the keyboard is connected and VIA is enabled.")
        sys.exit(1)

    device = hid.device()
    device.open_path(kb_path)
    print("Connected to keyboard.")

    # Send config
    unit = G7_UNIT_MMOL if args.unit == "mmol" else G7_UNIT_MGDL
    device.write(b'\x00' + build_config_packet(unit, args.low, args.high))
    print(f"Config sent: unit={'mmol/L' if unit else 'mg/dL'}, low={args.low}, high={args.high}")

    # Initialize pydexcom client
    print("Authenticating with Dexcom Share...")
    region_map = {"us": Region.US, "ous": Region.OUS, "jp": Region.JP}
    dex = Dexcom(username=args.username, password=args.password, region=region_map[args.region])
    print("Authenticated.")

    # Send initial history
    try:
        readings = dex.get_glucose_readings(max_count=24, minutes=1440)
        if readings:
            history = []
            for r in readings:
                history.append({"mgdl": r.value})
            send_history(device, history)
            print(f"Sent {len(history)} historical readings.")
    except Exception as e:
        print(f"Warning: Could not load history: {e}")

    # Main loop
    last_value = None
    print(f"Polling every {args.interval}s. Press Ctrl+C to stop.")

    while True:
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

                pkt = build_reading_packet(mgdl, trend, mins)
                device.write(b'\x00' + pkt)

                if last_value != mgdl:
                    last_value = mgdl
                    trend_desc = getattr(reading, "trend_description", "?")
                    print(f"[{datetime.now().strftime('%H:%M')}] {display_val} "
                          f"({trend_desc}, {mins}m ago)")
            else:
                print(f"[{datetime.now().strftime('%H:%M')}] No reading available.")

        except Exception as e:
            print(f"Error: {e}")
            # Re-authenticate if session expired
            if "session" in str(e).lower() or "auth" in str(e).lower():
                try:
                    dex = Dexcom(username=args.username, password=args.password, region=region_map[args.region])
                    print("Re-authenticated.")
                except Exception as re_err:
                    print(f"Re-auth failed: {re_err}")

        time.sleep(args.interval)


if __name__ == "__main__":
    main()
