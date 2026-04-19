// Copyright 2021 Hayashi (@w_vwbw)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "custom_oled.h"
#include "os_detection.h"
#include "common_killerwhale.h"
#include "glcdfont.c"
#include "custom_keycodes.h"
#include "moominhouse.h"
#include "transactions.h"
#include "g7_sync.h"
#include "timer.h"
#include <string.h>
#include <stdio.h>

// ---- State ----
static bool     interrupted;
static uint16_t interrupted_time;
static uint16_t interrupt_type;

static bool     tempch;
static uint16_t tempch_time;
static uint16_t tempch_type;

static uint8_t  cur_layer;

void oled_init_addedoled(void) {
    interrupted      = false;
    interrupted_time = 0;
    interrupt_type   = 0;
    tempch           = false;
    tempch_time      = 0;
    tempch_type      = 0;
    cur_layer        = 0;
}

// ---- Helpers ----

static uint8_t active_oled_state(void) {
    return is_keyboard_master() ? kw_config.oled_state_master : kw_config.oled_state_slave;
}

static bool host_is_apple(void) {
    os_variant_t os = detected_host_os();
    return os == OS_MACOS || os == OS_IOS;
}

// " J:" if this side holds the joystick, otherwise " L:" / " R:".
static const char* side_label(bool is_right) {
    uint8_t attached = get_joystick_attached();
    bool    has_joy  = is_right ? (attached == JOYSTICK_RIGHT) : (attached == JOYSTICK_LEFT);
    if (has_joy) return PSTR(" J:");
    return is_right ? PSTR(" R:") : PSTR(" L:");
}

static const char* pd_mode_label(uint8_t mode) {
    switch (mode) {
        case SCROLL_MODE: return PSTR("SCROL");
        case CURSOR_MODE: return PSTR("CURSR");
        case GAME_MODE:   return PSTR(" GAME");
        default:          return PSTR("  KEY");
    }
}

static const char* pd_mode_msg(uint8_t mode, bool is_right) {
    if (is_right) {
        switch (mode) {
            case SCROLL_MODE: return PSTR("SCROLL MODE RIGHT    ");
            case CURSOR_MODE: return PSTR("CURSOR MODE RIGHT    ");
            case KEY_INPUT:   return PSTR("KEY INPUT MODE RIGHT ");
            case GAME_MODE:   return PSTR("GAME MODE RIGHT      ");
        }
    } else {
        switch (mode) {
            case SCROLL_MODE: return PSTR("SCROLL MODE LEFT     ");
            case CURSOR_MODE: return PSTR("CURSOR MODE LEFT     ");
            case KEY_INPUT:   return PSTR("KEY INPUT MODE LEFT  ");
            case GAME_MODE:   return PSTR("GAME MODE LEFT       ");
        }
    }
    return PSTR("                     ");
}

// Static interrupt messages. NULL means the message needs dynamic state.
static const char* static_interrupt_msg(uint16_t type) {
    switch (type) {
        case UG_HUEU:    return PSTR("INCREASE HUE         ");
        case UG_HUED:    return PSTR("DECREASE HUE         ");
        case UG_SATU:    return PSTR("INCREASE SATURATION  ");
        case UG_SATD:    return PSTR("DECREASE SATURATION  ");
        case UG_VALU:    return PSTR("INCREASE VALUE       ");
        case UG_VALD:    return PSTR("DECREASE VALUE       ");
        case UG_SPDU:    return PSTR("INCREASE LIGHT SPEED ");
        case UG_SPDD:    return PSTR("DECREASE LIGHT SPEED ");
        case UG_NEXT:    return PSTR("NEXT LIGHT PATTERN   ");
        case UG_PREV:    return PSTR("PREV LIGHT PATTERN   ");
        case UNDO:       return PSTR("UNDO                 ");
        case REDO:       return PSTR("REDO                 ");
        case COPY:       return PSTR("COPY                 ");
        case CUT:        return PSTR("CUT                  ");
        case PASTE:      return PSTR("PASTE                ");
        case SC_UP:      return PSTR("SCALE UP             ");
        case SC_DOWN:    return PSTR("SCALE DOWN           ");
        case SC_RESET:   return PSTR("SCALE RESET          ");
        case CAPTCHA:    return PSTR("SCREEN CAPTHA        ");
        case SAVE:       return PSTR("SAVE                 ");
        case SAVEAS:     return PSTR("SAVE AS...           ");
        case NEXTTAB:    return PSTR("NEXT TAB             ");
        case PREVTAB:    return PSTR("PREV TAB             ");
        case CLOSETAB:   return PSTR("CLOSE TAB            ");
        case RSTRTAB:    return PSTR("RESTORE TAB          ");
        case L_SPD_I:    return PSTR("SPEED UP LEFT        ");
        case R_SPD_I:    return PSTR("SPEED UP RIGHT       ");
        case L_SPD_D:    return PSTR("SPEED DOWN LEFT      ");
        case R_SPD_D:    return PSTR("SPEED DOWN RIGHT     ");
        case L_ANG_I:    return PSTR("ANGLE TURNED CW LEFT ");
        case R_ANG_I:    return PSTR("ANGLE TURNED CW RIGHT");
        case L_ANG_D:    return PSTR("ANGLE TURNED CCW LEFT");
        case R_ANG_D:    return PSTR("ANGLE TURNED CCW RHGT");
        case L_INV:      return PSTR("AXIS INVERTED LEFT   ");
        case R_INV:      return PSTR("AXIS INVERTED RIGHT  ");
        case INV_SCRL:   return PSTR("SCROLL INVERTED      ");
        case OLED_MOD:   return PSTR("UPDATE LAYER         ");
        case QK_USER_16: return PSTR("JOYSTICK INITIALIZED ");
        default:         return NULL;
    }
}

// ---- Render sub-phases ----

static void render_interrupt_line(uint16_t type) {
    oled_set_cursor(0, 3);

    const char* s = static_interrupt_msg(type);
    if (s) {
        oled_write_P(s, false);
        return;
    }

    switch (type) {
        case UG_TOGG:
            oled_write_P(rgblight_is_enabled() ? PSTR("RGB ON               ")
                                               : PSTR("RGB OFF              "), false);
            break;
        case L_CHMOD:
            oled_write_P(pd_mode_msg(kw_config.pd_mode_l, false), false);
            break;
        case R_CHMOD:
            oled_write_P(pd_mode_msg(kw_config.pd_mode_r, true), false);
            break;
        case AUTO_MOUSE:
            oled_write_P(kw_config.auto_mouse ? PSTR("AUTO MOUSE ON        ")
                                              : PSTR("AUTO MOUSE OFF       "), false);
            break;
        case QK_USER_14:
            oled_write_P(get_dpad_exclusion() ? PSTR("DPAD EXCLUSION ON    ")
                                              : PSTR("DPAD EXCLUSION OFF   "), false);
            break;
        case QK_USER_15:
            oled_write_P(kw_config.rgb_layers ? PSTR("RGBLIGHT LAYER ON    ")
                                              : PSTR("RGBLIGHT LAYER OFF   "), false);
            break;
        case QK_USER_22:
        case QK_USER_23:
            oled_write_P(PSTR("OFFSET MIN: "), false);
            oled_write(get_u16_str(get_joystick_offset_min(), ' '), false);
            oled_write_P(PSTR("   "), false);
            break;
        case QK_USER_24:
        case QK_USER_25:
            oled_write_P(PSTR("OFFSET MAX: "), false);
            oled_write(get_u16_str(get_joystick_offset_max(), ' '), false);
            oled_write_P(PSTR("   "), false);
            break;
    }
}

static void render_layer_digits(void) {
    oled_set_cursor(0, 0);
    cur_layer = get_highest_layer(layer_state);
    const char* glyph = gpio_read_pin(GP10) ? reverse_number[cur_layer]
                                            : number[cur_layer];
    oled_write_raw_P(glyph, sizeof(reverse_number[cur_layer]));
}

static void render_speed_line(void) {
    oled_set_cursor(0, 0);
    oled_write_P(PSTR("SPD "), false);

    if (get_joystick_attached() == JOYSTICK_LEFT) {
        oled_write_P(PSTR(" J:"), false);
        oled_write_P(get_u16_str(16 + (uint16_t)kw_config.spd_l * 3, ' '), false);
    } else {
        oled_write_P(PSTR(" L:"), false);
        oled_write(get_u16_str(400 + (uint16_t)kw_config.spd_l * 200, ' '), false);
    }

    if (get_joystick_attached() == JOYSTICK_RIGHT) {
        oled_write_P(PSTR(" J:"), false);
        oled_write(get_u16_str(16 + (uint16_t)kw_config.spd_r * 3, ' '), false);
    } else {
        oled_write_P(PSTR(" R:"), false);
        oled_write_P(get_u16_str(400 + (uint16_t)kw_config.spd_r * 200, ' '), false);
    }
}

static void render_angle_line(void) {
    oled_set_cursor(0, 1);
    oled_write_P(PSTR("ANG "), false);
    oled_write_P(side_label(false), false);
    oled_write(get_u16_str((uint16_t)kw_config.angle_l * 12, ' '), false);
    oled_write_P(side_label(true), false);
    oled_write(get_u16_str((uint16_t)kw_config.angle_r * 12, ' '), false);
}

static void render_axis_line(void) {
    oled_set_cursor(0, 2);
    oled_write_P(PSTR("AXIS"), false);
    oled_write_P(side_label(false), false);
    oled_write_P(kw_config.inv_l ? PSTR("  INV") : PSTR("     "), false);
    oled_write_P(side_label(true), false);
    oled_write_P(kw_config.inv_r ? PSTR("  INV") : PSTR("     "), false);
}

static void render_mode_line(void) {
    oled_set_cursor(0, 3);
    oled_write_P(PSTR("MODE"), false);
    oled_write_P(side_label(false), false);
    oled_write_P(pd_mode_label(kw_config.pd_mode_l), false);
    oled_write_P(side_label(true), false);
    oled_write_P(pd_mode_label(kw_config.pd_mode_r), false);
}

// Overwrites row 3 with held modifiers when any are active.
static void render_mod_overlay(void) {
    uint8_t mods = get_mods();
    if (!mods) return;

    oled_set_cursor(0, 3);
    oled_write_P((mods & MOD_MASK_SHIFT) ? PSTR("SHIFT  ") : PSTR("       "), false);
    oled_write_P((mods & MOD_MASK_CTRL)  ? PSTR("CTRL  ")  : PSTR("      "),  false);

    if (mods & MOD_MASK_ALT) {
        oled_write_P(host_is_apple() ? PSTR("OPT  ") : PSTR("ALT  "), false);
    } else {
        oled_write_P(PSTR("     "), false);
    }

    if (mods & MOD_MASK_GUI) {
        oled_write_P(host_is_apple() ? PSTR("CMD") : PSTR("WIN"), false);
    } else {
        oled_write_P(PSTR("   "), false);
    }
}

static void render_info_view(void) {
    render_speed_line();
    render_angle_line();
    render_axis_line();
    render_mode_line();
    render_mod_overlay();
}

// ---- G7 glucose view ----

// Format elapsed minutes into an exact 3-char string:
//   " 5m", "59m", " 1h", "99h", "---"
static void fmt_minutes_ago(uint8_t raw_min, uint32_t extra_ms, char *out, size_t outsz) {
    uint32_t mins = (uint32_t)raw_min + (extra_ms / 60000UL);
    if (mins < 60) {
        snprintf(out, outsz, "%2lum", (unsigned long)mins);
    } else if (mins < 60UL * 100UL) {
        snprintf(out, outsz, "%2luh", (unsigned long)(mins / 60));
    } else {
        snprintf(out, outsz, "---");
    }
}

// Each row is exactly 21 columns wide (128 px / 6 px-per-char).
// Buffers are oversized so GCC -Wformat-truncation stays happy even for
// pathological uint16_t inputs; we pad/clip on write instead of in snprintf.
#define G7_ROW_COLS 21

static void write_row(uint8_t row, const char *text) {
    char out[G7_ROW_COLS + 1];
    size_t i = 0;
    while (i < G7_ROW_COLS && text[i] != '\0') {
        out[i] = text[i];
        i++;
    }
    while (i < G7_ROW_COLS) {
        out[i++] = ' ';
    }
    out[G7_ROW_COLS] = '\0';
    oled_set_cursor(0, row);
    oled_write(out, false);
}

// Line graph rendered in the 16-px band between text rows 0 and 3.
// 24 samples at 5px pitch = 115px wide, 6px left margin.
// Newest sample is stored at index 0, so the newest point is drawn on the right.
#define G7_GRAPH_Y_TOP     8
#define G7_GRAPH_HEIGHT    16
#define G7_GRAPH_PITCH     5
#define G7_GRAPH_X_LEFT    6
// Range clamp: 54 mg/dL (3.0 mmol) .. 270 mg/dL (15.0 mmol).
// graph values are stored as mgdl/2, so clamp in halved units.
#define G7_GRAPH_MIN_HALF  27
#define G7_GRAPH_MAX_HALF  135

// Bresenham line between two points in the graph band.
static void draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    int16_t dx  =  (x1 > x0) ? (x1 - x0) : (x0 - x1);
    int16_t dy  = -((y1 > y0) ? (y1 - y0) : (y0 - y1));
    int16_t sx  = x0 < x1 ? 1 : -1;
    int16_t sy  = y0 < y1 ? 1 : -1;
    int16_t err = dx + dy;
    while (1) {
        oled_write_pixel((uint8_t)x0, (uint8_t)y0, true);
        if (x0 == x1 && y0 == y1) break;
        int16_t e2 = err << 1;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

static uint8_t sample_to_y(uint8_t v) {
    const uint8_t range    = G7_GRAPH_MAX_HALF - G7_GRAPH_MIN_HALF;
    const uint8_t y_bottom = G7_GRAPH_Y_TOP + G7_GRAPH_HEIGHT - 1;
    uint8_t clamped =
        v < G7_GRAPH_MIN_HALF ? G7_GRAPH_MIN_HALF :
        v > G7_GRAPH_MAX_HALF ? G7_GRAPH_MAX_HALF : v;
    uint8_t h = (uint8_t)(((uint16_t)(clamped - G7_GRAPH_MIN_HALF) * (G7_GRAPH_HEIGHT - 1)) / range);
    return (uint8_t)(y_bottom - h);
}

static void render_g7_graph(void) {
    const uint8_t *graph = g7_get_graph();

    // Clear the graph band.
    for (uint8_t y = G7_GRAPH_Y_TOP; y < G7_GRAPH_Y_TOP + G7_GRAPH_HEIGHT; y++) {
        for (uint8_t x = 0; x < 128; x++) {
            oled_write_pixel(x, y, false);
        }
    }

    // Draw a polyline between adjacent non-empty samples, chronologically
    // (oldest on the left, newest on the right).
    int16_t prev_x = -1;
    int16_t prev_y = 0;
    for (uint8_t display_i = 0; display_i < G7_GRAPH_SAMPLES; display_i++) {
        uint8_t i = G7_GRAPH_SAMPLES - 1 - display_i;
        uint8_t v = graph[i];
        if (v == 0) { prev_x = -1; continue; }

        int16_t x = G7_GRAPH_X_LEFT + display_i * G7_GRAPH_PITCH;
        int16_t y = sample_to_y(v);
        if (prev_x >= 0) {
            draw_line(prev_x, prev_y, x, y);
        } else {
            oled_write_pixel((uint8_t)x, (uint8_t)y, true);
        }
        prev_x = x;
        prev_y = y;
    }
}

static void render_g7_view(void) {
    g7_data_t *g7 = g7_get_data();

    char buf[80];

    // Row 0: " 7.5 mmol/L        ^^"
    {
        char value_buf[16];
        if (!g7->valid || g7->glucose_mgdl == 0) {
            snprintf(value_buf, sizeof(value_buf), "---");
        } else if (g7->unit == G7_UNIT_MMOL) {
            uint16_t x10 = g7_mgdl_to_mmol_x10(g7->glucose_mgdl);
            snprintf(value_buf, sizeof(value_buf), "%u.%u", x10 / 10, x10 % 10);
        } else {
            snprintf(value_buf, sizeof(value_buf), "%u", g7->glucose_mgdl);
        }
        const char *unit = (g7->unit == G7_UNIT_MMOL) ? "mmol/L" : "mg/dL";
        snprintf(buf, sizeof(buf), " %s %s          %s",
                 value_buf, unit, g7_trend_str(g7->trend));
    }
    write_row(0, buf);

    // Graph fills the middle 16 px (text rows 1-2).
    render_g7_graph();

    // Row 3: " 5m  lo 3.9  hi 10.0"
    {
        char lo_buf[16], hi_buf[16], time_buf[8];
        if (g7->unit == G7_UNIT_MMOL) {
            uint16_t lo = g7_mgdl_to_mmol_x10(g7->low_threshold);
            uint16_t hi = g7_mgdl_to_mmol_x10(g7->high_threshold);
            snprintf(lo_buf, sizeof(lo_buf), "%u.%u", lo / 10, lo % 10);
            snprintf(hi_buf, sizeof(hi_buf), "%u.%u", hi / 10, hi % 10);
        } else {
            snprintf(lo_buf, sizeof(lo_buf), "%u", g7->low_threshold);
            snprintf(hi_buf, sizeof(hi_buf), "%u", g7->high_threshold);
        }
        if (g7->valid) {
            fmt_minutes_ago(g7->minutes_ago, timer_elapsed32(g7_get_last_reading_time()),
                            time_buf, sizeof(time_buf));
        } else {
            snprintf(time_buf, sizeof(time_buf), "---");
        }
        snprintf(buf, sizeof(buf), "%s  lo %s  hi %s", time_buf, lo_buf, hi_buf);
    }
    write_row(3, buf);
}

// ---- Public OLED task ----

bool oled_task_user(void) {
    uint8_t state = active_oled_state();

    if (state == STATE_ANIMONE) {
        render_moominhouse();
        return true;
    }

    if (interrupted) {
        if (timer_elapsed(interrupted_time) < INTERRUPT_TIME) {
            render_interrupt_line(interrupt_type);
        } else {
            interrupted = false;
        }
        return true;
    }

    if (state == STATE_LAYER) {
        render_layer_digits();
    } else if (state == STATE_INFO) {
        render_info_view();
    } else if (state == STATE_G7) {
        render_g7_view();
    }
    return true;
}

// ---- Split-sync of kw_config ----

static void kw_config_sync_handler(uint8_t in_buflen, const void *in_data, uint8_t out_buflen, void *out_data) {
    if (in_buflen == sizeof(kw_config_t)) {
        memcpy(&kw_config, in_data, in_buflen);
    }
}

void kw_config_sync_init(void) {
    transaction_register_rpc(OLEDFORCE_SYNC, kw_config_sync_handler);
}

void kw_config_sync_task(void) {
    if (!is_keyboard_master()) return;

    static uint32_t    last_sync  = 0;
    static kw_config_t last_sent  = {0};
    bool               needs_sync = false;

    if (memcmp(&kw_config, &last_sent, sizeof(kw_config_t)) != 0) {
        needs_sync = true;
    } else if (timer_elapsed32(last_sync) > 250) {
        needs_sync = true;
    }

    if (needs_sync && transaction_rpc_send(OLEDFORCE_SYNC, sizeof(kw_config_t), &kw_config)) {
        last_sync = timer_read32();
        memcpy(&last_sent, &kw_config, sizeof(kw_config_t));
    }
}

void housekeeping_task_kb(void) {
    if (is_oled_on() && last_input_activity_elapsed() > OLED_TIMEOUT) {
        oled_off();
    }
    kw_config_sync_task();
    g7_sync_task();
}

// ---- Interrupt / temp-char setters ----

void oled_interrupt(uint16_t keycode) {
    interrupted      = true;
    interrupted_time = timer_read();
    interrupt_type   = keycode;
}

void oled_tempch(bool on, uint16_t keycode) {
    tempch      = on;
    tempch_time = timer_read();
    tempch_type = keycode;
}
