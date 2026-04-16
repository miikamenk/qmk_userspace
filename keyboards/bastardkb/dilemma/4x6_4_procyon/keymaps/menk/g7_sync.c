#include "g7_sync.h"
#include "timer.h"
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
        case G7_TREND_SINGLE_UP:      return "^";
        case G7_TREND_FORTY_FIVE_UP:  return "/^";
        case G7_TREND_FLAT:           return "->";
        case G7_TREND_FORTY_FIVE_DN:  return "\\v";
        case G7_TREND_SINGLE_DN:      return "v";
        case G7_TREND_DOUBLE_DN:      return "vv";
        case G7_TREND_NOT_COMPUTABLE: return "??";
        case G7_TREND_OUT_OF_RANGE:   return "OOR";
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

                // Only push to graph if value actually changed (new reading)
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

// Split transport receive handler (runs on slave / left side)
void g7_sync_recv(uint8_t in_buflen, const void *in_data, uint8_t out_buflen, void *out_data) {
    if (in_buflen == sizeof(g7_sync_t)) {
        const g7_sync_t *sync = (const g7_sync_t *)in_data;
        memcpy(&g7_data, &sync->data, sizeof(g7_data_t));
        memcpy(g7_graph, sync->graph, G7_GRAPH_SAMPLES);
    }
}
