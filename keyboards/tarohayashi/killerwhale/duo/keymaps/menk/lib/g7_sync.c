#include "g7_sync.h"
#include "quantum.h"
#include "transactions.h"
#include <string.h>

static g7_data_t g7_data = {
    .glucose_mgdl   = 0,
    .trend          = G7_TREND_NONE,
    .minutes_ago    = 0,
    .status         = 2,
    .unit           = G7_UNIT_MMOL,
    .low_threshold  = G7_DEFAULT_LOW,
    .high_threshold = G7_DEFAULT_HIGH,
    .valid          = false,
};

static uint8_t  g7_graph[G7_GRAPH_SAMPLES] = {0};
static uint32_t g7_last_reading_time        = 0;

void g7_init(void) {
    memset(g7_graph, 0, sizeof(g7_graph));
    transaction_register_rpc(USER_SYNC_G7, g7_sync_recv);
}

g7_data_t *g7_get_data(void) {
    return &g7_data;
}

uint8_t *g7_get_graph(void) {
    return g7_graph;
}

uint32_t g7_get_last_reading_time(void) {
    return g7_last_reading_time;
}

uint16_t g7_mgdl_to_mmol_x10(uint16_t mgdl) {
    return (uint16_t)((mgdl * 10UL) / 18);
}

const char *g7_trend_str(uint8_t trend) {
    switch (trend) {
        case G7_TREND_DOUBLE_UP:      return "^^";
        case G7_TREND_SINGLE_UP:      return "^ ";
        case G7_TREND_FORTY_FIVE_UP:  return "/^";
        case G7_TREND_FLAT:           return "->";
        case G7_TREND_FORTY_FIVE_DN:  return "\\v";
        case G7_TREND_SINGLE_DN:      return "v ";
        case G7_TREND_DOUBLE_DN:      return "vv";
        case G7_TREND_NOT_COMPUTABLE: return "??";
        case G7_TREND_OUT_OF_RANGE:   return "!!";
        default:                      return "--";
    }
}

void g7_process_hid(uint8_t *data, uint8_t length) {
    if (length < 1) return;

    switch (data[0]) {
        case G7_CMD_READING:
            if (length >= 5) {
                uint16_t prev = g7_data.glucose_mgdl;

                g7_data.glucose_mgdl = data[1] | (data[2] << 8);
                g7_data.trend        = data[3];
                g7_data.minutes_ago  = data[4];
                g7_data.status       = 0;
                g7_data.valid        = true;
                g7_last_reading_time = timer_read32();

                // Push to graph only when value actually changes (new sample).
                if (g7_data.glucose_mgdl != prev || prev == 0) {
                    for (uint8_t i = G7_GRAPH_SAMPLES - 1; i > 0; i--) {
                        g7_graph[i] = g7_graph[i - 1];
                    }
                    g7_graph[0] = g7_data.glucose_mgdl > 510 ? 255 : g7_data.glucose_mgdl / 2;
                }
            }
            break;

        case G7_CMD_CONFIG:
            if (length >= 6) {
                g7_data.unit           = data[1];
                g7_data.low_threshold  = data[2] | (data[3] << 8);
                g7_data.high_threshold = data[4] | (data[5] << 8);
            }
            break;

        case G7_CMD_HISTORY:
            if (length >= 3) {
                uint8_t start = data[1];
                uint8_t count = data[2];
                for (uint8_t i = 0; i < count && (3 + i) < length && (start + i) < G7_GRAPH_SAMPLES; i++) {
                    g7_graph[start + i] = data[3 + i];
                }
            }
            break;
    }
}

void g7_sync_recv(uint8_t in_buflen, const void *in_data, uint8_t out_buflen, void *out_data) {
    if (in_buflen == sizeof(g7_sync_t)) {
        // in_data isn't guaranteed to be 4-byte aligned; copy into a local
        // aligned struct before touching uint32_t fields (Cortex-M0+ faults
        // on unaligned word loads).
        g7_sync_t sync;
        memcpy(&sync, in_data, sizeof(g7_sync_t));
        memcpy(&g7_data, &sync.data, sizeof(g7_data_t));
        memcpy(g7_graph, sync.graph, G7_GRAPH_SAMPLES);
        // Reconstruct a local timer anchor so timer_elapsed32() on the slave
        // matches the master's sense of "ms since last HID reading".
        g7_last_reading_time = timer_read32() - sync.ms_since_reading;
    }
}

void g7_sync_task(void) {
    if (!is_keyboard_master()) return;

    static uint32_t  last_sync       = 0;
    static g7_data_t last_sent_data  = {0};
    static uint8_t   last_sent_graph[G7_GRAPH_SAMPLES] = {0};
    g7_sync_t        current;

    memcpy(&current.data, &g7_data, sizeof(g7_data_t));
    memcpy(current.graph, g7_graph, G7_GRAPH_SAMPLES);
    current.ms_since_reading = timer_elapsed32(g7_last_reading_time);

    // ms_since_reading changes every tick, so exclude it from change detection —
    // otherwise we'd flood the split link. The 1s fallback keeps the slave fresh.
    bool needs_sync = false;
    if (memcmp(&current.data, &last_sent_data, sizeof(g7_data_t)) != 0 ||
        memcmp(current.graph, last_sent_graph, G7_GRAPH_SAMPLES) != 0) {
        needs_sync = true;
    } else if (timer_elapsed32(last_sync) > 1000) {
        needs_sync = true;
    }

    if (needs_sync && transaction_rpc_send(USER_SYNC_G7, sizeof(g7_sync_t), &current)) {
        last_sync = timer_read32();
        memcpy(&last_sent_data, &current.data, sizeof(g7_data_t));
        memcpy(last_sent_graph, current.graph, G7_GRAPH_SAMPLES);
    }
}
