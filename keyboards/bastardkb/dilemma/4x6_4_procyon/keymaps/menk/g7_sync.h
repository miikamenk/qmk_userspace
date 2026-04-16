#pragma once

#include <stdint.h>
#include <stdbool.h>

// VIA custom channel ID for G7 data
#define G7_VIA_CHANNEL      0xA0

// Sub-commands within our channel
#define G7_CMD_READING      0x01
#define G7_CMD_CONFIG       0x02
#define G7_CMD_HISTORY      0x03

// Graph: each sample = one 5-min reading, 24 samples = 2 hours
#define G7_GRAPH_SAMPLES    24

// Dexcom trend codes
#define G7_TREND_NONE           0
#define G7_TREND_DOUBLE_UP      1
#define G7_TREND_SINGLE_UP      2
#define G7_TREND_FORTY_FIVE_UP  3
#define G7_TREND_FLAT           4
#define G7_TREND_FORTY_FIVE_DN  5
#define G7_TREND_SINGLE_DN      6
#define G7_TREND_DOUBLE_DN      7
#define G7_TREND_NOT_COMPUTABLE 8
#define G7_TREND_OUT_OF_RANGE   9

// Default thresholds (mg/dL)
#define G7_DEFAULT_LOW      70
#define G7_DEFAULT_HIGH     180

// Units
#define G7_UNIT_MGDL        0
#define G7_UNIT_MMOL        1

typedef struct {
    uint16_t glucose_mgdl;
    uint8_t  trend;
    uint8_t  minutes_ago;       // initial offset from host
    uint8_t  status;            // 0=ok, 1=warming, 2=no_data
    uint8_t  unit;              // G7_UNIT_MGDL or G7_UNIT_MMOL
    uint16_t low_threshold;     // mg/dL
    uint16_t high_threshold;    // mg/dL
    bool     valid;
} g7_data_t;

// Combined struct for split sync (data + graph in one RPC call)
typedef struct {
    g7_data_t data;
    uint8_t   graph[G7_GRAPH_SAMPLES];
} g7_sync_t;

void       g7_init(void);
void       g7_process_hid(uint8_t *data, uint8_t length);
g7_data_t *g7_get_data(void);
uint8_t   *g7_get_graph(void);
uint32_t   g7_get_last_reading_time(void);
void       g7_sync_recv(uint8_t in_buflen, const void *in_data, uint8_t out_buflen, void *out_data);

// Returns trend as short ASCII string
const char *g7_trend_str(uint8_t trend);

// Convert mg/dL to mmol/L * 10 (e.g. 100 mg/dL -> 55 = 5.5 mmol/L)
uint16_t g7_mgdl_to_mmol_x10(uint16_t mgdl);
