/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 * Copyright 2023 casuanoob <casuanoob@hotmail.com> (@casuanoob)
 * Copyright 2025 Menk <miikamenk@duck.com> (@miikamenk)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "transactions.h"
#include <ctype.h>
#ifdef COMMUNITY_MODULE_QP_HELPERS_ENABLE
#    include "qp_helpers.h"
#endif

enum dilemma_keymap_layers {
    LAYER_BASE = 0,
    LAYER_LOWER,
    LAYER_RAISE,
    LAYER_POINTER,
};

// Automatically enable sniping-mode on the pointer layer.
// #define DILEMMA_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#define LOWER MO(LAYER_LOWER)
#define RAISE MO(LAYER_RAISE)
#define PT_Z LT(LAYER_POINTER, KC_Z)
#define PT_SLSH LT(LAYER_POINTER, KC_SLSH)

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
        KC_ESC,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,       KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_MINS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
        KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_BSLS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LCTL,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,       KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LSFT,    PT_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M, KC_COMM,  KC_DOT, PT_SLSH, KC_LALT,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                         LOWER,  KC_LGUI,  KC_SPC,   LOWER,      KC_RGUI,  KC_ENT, KC_BSPC, RAISE
  //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
  ),

  [LAYER_LOWER] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       KC_TILD, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,    KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_UNDS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       RM_NEXT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    KC_LBRC,   KC_P7,   KC_P8,   KC_P9, KC_RBRC, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       RM_TOGG, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,    KC_PPLS,   KC_P4,   KC_P5,   KC_P6, KC_PMNS, KC_PEQL,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       RM_PREV, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    KC_PAST,   KC_P1,   KC_P2,   KC_P3, KC_PSLS, KC_PDOT,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                         XXXXXXX, XXXXXXX, XXXXXXX, _______,    XXXXXXX, _______, XXXXXXX, XXXXXXX
  //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
  ),

  [LAYER_RAISE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
        KC_F12,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,      KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_MNXT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_VOLU,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_MPLY, KC_LEFT,   KC_UP, KC_DOWN, KC_RGHT, XXXXXXX,    XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, KC_MUTE,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_MPRV, KC_HOME, KC_PGUP, KC_PGDN,  KC_END, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_VOLD,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                         XXXXXXX, _______, _______, XXXXXXX,    _______, XXXXXXX, XXXXXXX, XXXXXXX
  //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
  ),

  [LAYER_POINTER] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, S_D_MOD, DPI_MOD, S_D_MOD,    S_D_MOD, DPI_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,    XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, _______, DRGSCRL, SNIPING, EE_CLR,  QK_BOOT,    QK_BOOT, EE_CLR,  SNIPING, DRGSCRL, _______, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                         XXXXXXX, MS_BTN2, MS_BTN1, MS_BTN3,    MS_BTN3, MS_BTN1, MS_BTN2, XXXXXXX
  //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
  ),
};
// clang-format on

uint8_t wpm_graph_samples[WPM_GRAPH_SAMPLES] = {0};

static uint8_t wpm_avg = 0;

static void update_wpm_avg(void) {
    static uint8_t wpm_samples[10] = {0};
    static uint8_t index           = 0;
    uint16_t       sum             = 0;

    wpm_samples[index] = get_current_wpm();
    index              = (index + 1) % 10;

    for (uint8_t i = 0; i < 10; i++) {
        sum += wpm_samples[i];
    }

    wpm_avg = sum / 10;

    /* Push into graph buffer every full cycle */
    if (index == 0) {
        for (uint8_t i = WPM_GRAPH_SAMPLES - 1; i > 0; i--) {
            wpm_graph_samples[i] = wpm_graph_samples[i - 1];
        }
        wpm_graph_samples[0] = wpm_avg;
    }
}

#ifdef CAPS_WORD_ENABLE
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        case KC_A ... KC_Z:
        case KC_SCLN:
        case KC_QUOT:
        case KC_SLSH:
            add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_MINS:
        case KC_DOT:
        case KC_COMM:
        case KC_UNDS:
            return true;

        default:
            return false;  // Deactivate Caps Word.
    }
}
#endif

// pointing device stuff
#ifdef POINTING_DEVICE_ENABLE
#    ifdef DILEMMA_AUTO_SNIPING_ON_LAYER
    layer_state_t layer_state_set_user(layer_state_t state) {
        dilemma_set_pointer_sniping_enabled(layer_state_cmp(state, DILEMMA_AUTO_SNIPING_ON_LAYER));
        return state;
    }
#    endif // DILEMMA_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLE

typedef struct {
    uint16_t dpi;
    uint16_t sniping_dpi;
} dpi_state_t;

static dpi_state_t dpi_state;
static bool is_caps_on = false;
static uint8_t display_mode = 0;

void user_sync_dpi_recv(uint8_t in_buflen, const void *in_data, uint8_t out_buflen, void *out_data) {
    if (in_buflen == sizeof(dpi_state)) {
        memcpy(&dpi_state, in_data, sizeof(dpi_state));
    }
}


void user_sync_caps_word_recv(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    if (in_buflen == sizeof(is_caps_on)) {
        memcpy(&is_caps_on, in_data, in_buflen);
    }
}


#ifdef RGB_MATRIX_ENABLE
// Forward-declare this helper function since it is defined in rgb_matrix.c.
void rgb_matrix_update_pwm_buffers(void);
#endif // RGB_MATRIX_ENABLE

#ifdef ENCODER_MAP_ENABLE
// clang-format off
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [LAYER_BASE]       = {ENCODER_CCW_CW(MS_WHLU, MS_WHLD), ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [LAYER_LOWER]      = {ENCODER_CCW_CW(KC_UP, KC_DOWN), ENCODER_CCW_CW(KC_LEFT, KC_RGHT)},
    [LAYER_RAISE]      = {ENCODER_CCW_CW(KC_PGUP, KC_PGDN), ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [LAYER_POINTER]    = {ENCODER_CCW_CW(RM_HUED, RM_HUEU), ENCODER_CCW_CW(RM_SATD, RM_SATU)},
};
// clang-format on
#endif // ENCODER_MAP_ENABLE


#ifdef QUANTUM_PAINTER_ENABLE
#    include "qp.h"
#    include "qp_comms.h"
#    include "color.h"
#    include "gfx/common.qgf.c"
#    include "gfx/bloodborne.qgf.c"
#    include "gfx/font_oled.qff.c"


typedef struct {
    hsv_t primary;
    hsv_t secondary;
} dual_hsv_t;

static dual_hsv_t user_hsv;

painter_device_t lcd;
painter_font_handle_t         font_oled;
painter_font_handle_t         font_menu;
static painter_image_handle_t shift_icon, control_icon, alt_icon,  windows_icon;
static painter_image_handle_t bloodborne;
#endif // QUANTUM_PAINTER_ENABLE

void draw_dpi_bar(
    uint16_t x,
    uint16_t y,
    uint16_t dpi,
    uint16_t screen_width,
    painter_font_handle_t font,
    bool sniping
) {
    char buf[16];
    snprintf(buf, sizeof(buf), sniping ? "%3u" : "%4u", dpi);
    qp_drawtext_recolor(
        lcd, x, y, font_oled, buf,
        0,
        0,
        0,
        37, 40, 242
    );

    // Calculate bar position and dimensions
    uint16_t text_width = sniping ? qp_textwidth(font, "999") : qp_textwidth(font, "9999");
    uint16_t bar_x = x + text_width + 4;
    uint16_t bar_y = y + (font->line_height / 2) - 2;
    uint16_t bar_h = 4;

    // Calculate available bar width (don't overflow screen)
    uint16_t bar_w;
    if (bar_x >= screen_width) {
        // Not enough space for bar
        return;
    }

    // Leave some right margin (4 pixels)
    if (screen_width - bar_x - 4 > 0) {
        bar_w = screen_width - bar_x - 4;
    } else {
        bar_w = 0;
    }

    // Draw background
    if (bar_w > 0) {
        qp_rect(lcd, bar_x, bar_y, bar_x + bar_w, bar_y + bar_h,
                40, 40, 40, true);
    }

    // Define DPI settings for each mode
    const uint16_t sniper_steps[] = {200, 300, 400, 500};
    const uint16_t sniper_step_count = 4;

    // Normal DPI steps as specified
    // You need to change /quantum/digitizer_mouse_fallback.c:64 to 3200 to match the keyboard's dpi steps

    const uint16_t normal_steps[] = {400, 600, 800, 1000, 1200, 1400, 1600,
                                     2000, 2200, 2400, 2600, 2800, 3000, 3200, 3400};
    const uint16_t normal_step_count = 15;

    if (sniping) {
        // Find closest sniper step
        uint16_t closest_index = 0;
        uint16_t min_diff = UINT16_MAX;

        for (int i = 0; i < sniper_step_count; i++) {
            uint16_t diff = (dpi > sniper_steps[i]) ? (dpi - sniper_steps[i]) : (sniper_steps[i] - dpi);
            if (diff < min_diff) {
                min_diff = diff;
                closest_index = i;
            }
        }

        if (bar_w > 0 && sniper_step_count > 1) {
            // Draw filled portion for sniper mode (discrete steps)
            uint16_t segment_width = bar_w / (sniper_step_count - 1);
            uint16_t filled_width = (closest_index + 1) * segment_width;

            if (filled_width > 0) {
                qp_rect(lcd,
                        bar_x,
                        bar_y,
                        bar_x + filled_width,
                        bar_y + bar_h,
                        90, 255, 255,  // Cyan color for normal mode
                        true);
            }

            // Draw markers for each sniper step
            for (int i = 1; i < sniper_step_count; i++) {
                uint16_t marker_x = bar_x + i * segment_width;
                qp_rect(lcd,
                        marker_x - 1, bar_y - 1,
                        marker_x + 1, bar_y + bar_h + 1,
                        60, 60, 60,  // Dark gray markers
                        true);
            }
        }
    } else {
        // Find closest normal step
        uint16_t closest_index = 0;
        uint16_t min_diff = UINT16_MAX;

        for (int i = 0; i < normal_step_count; i++) {
            uint16_t diff = (dpi > normal_steps[i]) ? (dpi - normal_steps[i]) : (normal_steps[i] - dpi);
            if (diff < min_diff) {
                min_diff = diff;
                closest_index = i;
            }
        }

        if (bar_w > 0 && normal_step_count > 1) {
            // Draw filled portion for normal mode (discrete steps)
            uint16_t segment_width = bar_w / (normal_step_count - 1);
            uint16_t filled_width = (closest_index + 1) * segment_width;

            if (filled_width > 0) {
                qp_rect(lcd,
                        bar_x,
                        bar_y,
                        bar_x + filled_width,
                        bar_y + bar_h,
                        90, 255, 255,  // Cyan color for normal mode
                        true);
            }

            // Draw markers for every 2nd step to avoid clutter
            for (int i = 2; i < normal_step_count; i += 2) {
                uint16_t marker_x = bar_x + i * segment_width;
                qp_rect(lcd,
                        marker_x - 1, bar_y - 1,
                        marker_x + 1, bar_y + bar_h + 1,
                        60, 60, 60,  // Dark gray markers
                        true);
            }
        }
    }

    // Draw outline around the entire bar
    if (bar_w > 0) {
        qp_rect(lcd, bar_x, bar_y, bar_x + bar_w, bar_y + bar_h,
                100, 100, 100, false);
    }
}

void painter_render_modifiers(painter_device_t device, painter_font_handle_t font, uint16_t x, uint16_t y,
                              uint16_t width, bool force_redraw, dual_hsv_t *user_hsv, uint8_t disabled_val) {
    static uint8_t last_mods = 0;
    uint8_t current_mods = get_mods() | get_weak_mods() | get_oneshot_mods();

    if (force_redraw || last_mods != current_mods) {
        last_mods = current_mods;

        // Combine left/right bits
        uint8_t shift    = MOD_BIT_LSHIFT | MOD_BIT_RSHIFT;
        uint8_t ctrl     = MOD_BIT_LCTRL  | MOD_BIT_RCTRL;
        uint8_t alt      = MOD_BIT_LALT   | MOD_BIT_RALT;
        uint8_t gui      = MOD_BIT_LGUI   | MOD_BIT_RGUI;

        if (qp_drawimage_recolor(device, x, y, shift_icon,
                                 last_mods & shift ? user_hsv->secondary.h : user_hsv->primary.h,
                                 last_mods & shift ? user_hsv->secondary.s : user_hsv->primary.s,
                                 last_mods & shift ? user_hsv->primary.v : disabled_val, 37, 40, 242)) {
            x += shift_icon->width + 2;
        }

        if (qp_drawimage_recolor(device, x, y, windows_icon,
                                 last_mods & gui ? user_hsv->secondary.h : user_hsv->primary.h,
                                 last_mods & gui ? user_hsv->secondary.s : user_hsv->primary.s,
                                 last_mods & gui ? user_hsv->primary.v : disabled_val, 37, 40, 242)) {
            x += windows_icon->width + 2;
        }

        if (qp_drawimage_recolor(device, x, y, alt_icon,
                                 last_mods & alt ? user_hsv->secondary.h : user_hsv->primary.h,
                                 last_mods & alt ? user_hsv->secondary.s : user_hsv->primary.s,
                                 last_mods & alt ? user_hsv->primary.v : disabled_val, 37, 40, 242)) {
            x += alt_icon->width + 2;
        }

        if (qp_drawimage_recolor(device, x, y, control_icon,
                                 last_mods & ctrl ? user_hsv->secondary.h : user_hsv->primary.h,
                                 last_mods & ctrl ? user_hsv->secondary.s : user_hsv->primary.s,
                                 last_mods & ctrl ? user_hsv->primary.v : disabled_val, 37, 40, 242)) {
            x += control_icon->width + 2;
        }
    }
}

const char *rgb_matrix_get_effect_name(void) {
    static char    buf[32]     = {0};
    static uint8_t last_effect = 0;
    if (last_effect != rgb_matrix_get_mode()) {
        last_effect = rgb_matrix_get_mode();
        snprintf(buf, sizeof(buf), "%s", rgb_matrix_get_mode_name(rgb_matrix_get_mode()));
        for (uint8_t i = 1; i < sizeof(buf); ++i) {
            if (buf[i] == 0)
                break;
            else if (buf[i] == '_')
                buf[i] = ' ';
            else if (buf[i - 1] == ' ')
                buf[i] = toupper(buf[i]);
            else if (buf[i - 1] != ' ')
                buf[i] = tolower(buf[i]);
        }
    }
    return buf;
}

void painter_render_wpm_graph(painter_device_t device, painter_font_handle_t font, uint16_t x, uint16_t y,
                              uint16_t width, uint16_t height, bool force_redraw, dual_hsv_t *curr_hsv) {
#if defined(WPM_ENABLE) && defined(COMMUNITY_MODULE_QP_HELPERS_ENABLE)
    static uint16_t wpm_timer = 0;

    if (force_redraw || timer_elapsed(wpm_timer) > 1000) {
        wpm_timer = timer_read();
        const graph_line_t lines[] = {
            {
                .data      = wpm_graph_samples,
                .color     = {.h = 85, .s = 180, .v = 194},
                .mode      = LINE,
                .max_value = 120,
            },
            GRAPHS_END,
        };

        const graph_config_t config = {
            .device      = device,
            .start       = {.x = x, .y = y},
            .size        = {.x = width, .y = height},
            .axis        = curr_hsv->primary,
            .background  = {.h = 0, .s = 0, .v = 0},
            .data_points = WPM_GRAPH_SAMPLES,
        };

        qp_draw_graph(&config, lines);
    }
#endif // WPM_ENABLE
}

void draw_interface(uint8_t mode) {
    qp_rect(lcd,   1, 134, 229, 142, 154, 255, 235, true); // bar background
    switch (mode) {
        case 0:
            // window 2 title
            qp_drawtext_recolor(lcd, 2, 135, font_oled, "Bloodborne",
                        0,
                        0,
                        255,
                        154, 255, 235
            );
            qp_drawimage(lcd, 1, 144, bloodborne);
            break;
        case 1:
            // window 2 title
            qp_drawtext_recolor(lcd, 2, 135, font_oled, "C:\\WINDOWS\\system32\\cmd.exe",
                        0,
                        0,
                        255,
                        154, 255, 235
            );
            qp_rect(lcd,   1, 144, 238, 318, 0,   0,   0, true); // window background (black for terminal)
            //keylogger output goes here 10, 87, 239, 251
            break;
    }
}

// keylogger terminal stuff

#include "keylogger.c"

// keylogger sync stuff
typedef struct {
    char text[TERMINAL_WIDTH + 1];
    uint8_t length;
    bool is_current;  // Flag to mark current editing line
    bool dirty;       // Flag to mark if line needs redraw
} terminal_line_t;

static terminal_line_t terminal_lines[MAX_TERMINAL_LINES];
static bool terminal_initialized = false;
static uint32_t last_terminal_redraw = 0;

// Compressed keylogger sync structure
typedef struct {
    uint8_t line_num;
    uint8_t current_line;
    char line[TERMINAL_WIDTH];
    uint8_t sync_counter;  // To track sync sessions
} keylogger_sync_t;


void user_sync_mode_recv(uint8_t in_buflen, const void *in_data, uint8_t out_buflen, void *out_data) {
    if (in_buflen == sizeof(display_mode)) {
        uint8_t new_mode;
        memcpy(&new_mode, in_data, sizeof(new_mode));

        // If switching away from terminal mode (1 → 0), clear the terminal
        if (display_mode == 1 && new_mode == 0) {
            for (int i = 0; i < MAX_TERMINAL_LINES; i++) {
                terminal_lines[i].text[0] = '\0';
                terminal_lines[i].length = 0;
                terminal_lines[i].is_current = false;
                terminal_lines[i].dirty = true;
            }
            // Force a redraw to clear the screen
            keylogger_set_dirty(true);
        }

        memcpy(&display_mode, in_data, sizeof(display_mode));
    }
}

void user_sync_keylogger_recv(uint8_t in_buflen, const void *in_data, uint8_t out_buflen, void *out_data) {
    if (in_buflen >= sizeof(keylogger_sync_t)) {
        const keylogger_sync_t* sync_data = (const keylogger_sync_t*)in_data;

        if (sync_data->line_num < MAX_TERMINAL_LINES) {
            // Check if line actually changed
            bool line_changed = strncmp(terminal_lines[sync_data->line_num].text, sync_data->line, TERMINAL_WIDTH) != 0;
            bool current_changed = (terminal_lines[sync_data->line_num].is_current != (sync_data->line_num == sync_data->current_line));

            if (line_changed || current_changed) {
                // Update the specific line
                strncpy(terminal_lines[sync_data->line_num].text, sync_data->line, TERMINAL_WIDTH);
                terminal_lines[sync_data->line_num].text[TERMINAL_WIDTH] = '\0';
                terminal_lines[sync_data->line_num].length = strlen(terminal_lines[sync_data->line_num].text);
                terminal_lines[sync_data->line_num].is_current = (sync_data->line_num == sync_data->current_line);
                terminal_lines[sync_data->line_num].dirty = true;
            }
        }
    }
}


static void init_terminal_display(void) {
    // Clear terminal area
    qp_rect(lcd,   1, 144, 238, 318, 0,   0,   0, true); // window background (black for terminal)

    // Initialize line buffers
    for (int i = 0; i < MAX_TERMINAL_LINES; i++) {
        terminal_lines[i].text[0] = '\0';
        terminal_lines[i].length = 0;
        terminal_lines[i].is_current = false;
        terminal_lines[i].dirty = false;
    }
    terminal_initialized = true;
}


static void update_terminal_from_keylogger(void) {
    if (!is_keyboard_master()) return; // Only master updates its own display

    static char last_local_lines[MAX_TERMINAL_LINES][TERMINAL_WIDTH + 1] = {0};
    static bool last_current_lines[MAX_TERMINAL_LINES] = {false};

    uint8_t current_line = keylogger_get_current_line();

    for (int i = 0; i < MAX_TERMINAL_LINES; i++) {
        char line_buffer[TERMINAL_WIDTH + 1];
        keylogger_get_line(i, line_buffer);
        bool is_current = (i == current_line);

        // Check if line changed or current line status changed
        bool line_changed = strcmp(line_buffer, last_local_lines[i]) != 0;
        bool current_changed = (is_current != last_current_lines[i]);

        if (line_changed || current_changed) {
            strncpy(last_local_lines[i], line_buffer, TERMINAL_WIDTH);
            last_local_lines[i][TERMINAL_WIDTH] = '\0';
            last_current_lines[i] = is_current;

            strncpy(terminal_lines[i].text, line_buffer, TERMINAL_WIDTH);
            terminal_lines[i].text[TERMINAL_WIDTH] = '\0';
            terminal_lines[i].length = strlen(terminal_lines[i].text);
            terminal_lines[i].is_current = is_current;
            terminal_lines[i].dirty = true;
        }
    }
}

static bool should_redraw_terminal(void) {
    // Check if any line is dirty
    for (int i = 0; i < MAX_TERMINAL_LINES; i++) {
        if (terminal_lines[i].dirty) {
            return true;
        }
    }
    return false;
}

static void draw_terminal(void) {
    if (!terminal_initialized) {
        init_terminal_display();
    }

    // If we're the master, update from our own keylogger
    if (is_keyboard_master()) {
        update_terminal_from_keylogger();
    }

    // Only redraw if enough time has passed and we have dirty lines
    uint32_t now = timer_read32();
    if (!should_redraw_terminal() || timer_elapsed32(last_terminal_redraw) < 33) { // ~30 FPS max
        return;
    }

    last_terminal_redraw = now;

    // Don't clear entire area - only clear and redraw dirty lines
    for (int i = 0; i < MAX_TERMINAL_LINES; i++) {
        if (terminal_lines[i].dirty) {
            uint16_t y = 146 + (i * (font_oled->line_height + 4));

            // Clear the line area (from x=9 to x=238, line height + spacing)
            qp_rect(lcd, 3, y - 2, 238, y + font_oled->line_height + 2,
                   0,   0,   0, true);

            if (terminal_lines[i].length > 0) {
                // Draw the line
                qp_drawtext_recolor(lcd, 6, y, font_oled, terminal_lines[i].text,
                                   85,   0, 255,        // Black text
                                    0,   0,   0);   // Green background
            }

            terminal_lines[i].dirty = false;
        }
    }
}

void draw_wpm_indicator(uint16_t x, uint16_t y, uint16_t text_width, uint16_t text_height, uint16_t wpm) {
    // Base y position for the indicator
    uint16_t base_y = y + (text_height * 3);

    // Define the two sections (left and right) x coordinates
    uint16_t left_start_x = x + text_width - 2;
    uint16_t left_end_x = x + text_width + 8;
    uint16_t right_start_x = x + text_width + 10;
    uint16_t right_end_x = x + text_width + 20;

    // Calculate how many lines to draw with active color based on WPM
    // Each active line represents a WPM threshold
    uint8_t active_lines = 0;
    if (wpm > 20) active_lines = 2;
    if (wpm > 40) active_lines = 4;
    if (wpm > 60) active_lines = 6;
    if (wpm > 80) active_lines = 8;
    if (wpm > 100) active_lines = 10;
    if (wpm > 120) active_lines = 12;
    if (wpm > 140) active_lines = 14;
    if (wpm > 160) active_lines = 16;

    // Draw 8 levels (each level has 2 lines: top and bottom, making 16 lines total)
    for (int i = 0; i < 8; i++) {
        // Calculate y positions for this level's two lines
        uint16_t y_bottom = base_y - 5 - (i * 2);

        // Determine color for this level (first two lines active if wpm > 20, etc.)
        uint8_t color_green = 80; // Default color
        if (active_lines > (i * 2)) {
            color_green = 194; // Active color
        }

        // Draw bottom line of this level (left and right segments)
        qp_line(lcd, left_start_x, y_bottom, left_end_x, y_bottom, 85, 180, color_green);
        qp_line(lcd, right_start_x, y_bottom, right_end_x, y_bottom, 85, 180, color_green);
    }
}


void ili9341_draw_user(void) {
    static layer_state_t last_layer = 0xFF;
    static bool          first_draw = true;
#ifdef WPM_ENABLE
    static uint16_t      last_wpm   = 0;
#endif
#ifdef CAPS_WORD_ENABLE
    static bool          last_caps   = false;
#endif
#ifdef RGB_MATRIX_ENABLE
    static uint8_t       last_rgb     = 0xFF;
    static hsv_t         last_rgb_hsv = {0};
#endif

#ifdef POINTING_DEVICE_ENABLE
    static uint16_t last_dpi = 0xFFFF;
    static uint16_t last_sdpi = 0xFFFF;
#endif
    static uint8_t mode = 0xFF;
    static uint32_t last_full_redraw = 0;

    uint16_t width, height;
    qp_get_geometry(lcd, &width, &height, NULL, NULL, NULL);

    // set up retro windows
    if (first_draw) {
        qp_rect(lcd, 0, 0, width, height, 0, 0, 0, true);

        // window #1
        // borders
        qp_line(lcd,   0,   0, 238,   0, 154, 173, 255); // top
        qp_line(lcd,   0,   0,   0, 131, 154, 173, 255); // left
        qp_line(lcd, 239,   0, 239, 131, 154, 255, 128); // right
        qp_line(lcd,   1, 131, 239, 131, 154, 255, 128); // window bottom
        qp_rect(lcd,   1,  11, 238, 130,  37,  40, 242, true); // window background
        // bar
        qp_line(lcd,   1,  10, 239,  10, 154, 255, 128); // bar bottom
        qp_rect(lcd,   1,   1, 229,   9, 154, 255, 235, true); // bar background
        // bar buttons
        qp_rect(lcd, 230,   1, 238,   9, 255,   0, 230, false); // bar x border
        qp_rect(lcd, 231,   2, 237,   8, 255, 255, 230, true); // bar x background
        qp_line(lcd, 232,   3, 236,   7, 255,   0, 255); // bar x
        qp_line(lcd, 232,   7, 236,   3, 255,   0, 255); // bar x
        qp_drawtext_recolor(lcd, 2, 1, font_oled, "System Information",
                    0,
                    0,
                    255,
                    154, 255, 235
        );
        // window #2 (terminal, dark souls)
        // borders
        qp_line(lcd,   0, 133, 238, 133, 154, 173, 255); // top
        qp_line(lcd,   0, 133,   0, 319, 154, 173, 255); // left
        qp_line(lcd, 239, 133, 239, 319, 154, 255, 128); // right
        qp_line(lcd,   0, 319, 239, 319, 154, 255, 128); // window bottom
        // bar
        qp_line(lcd,   1, 143, 239, 143, 154, 255, 128); // bar bottom
        qp_rect(lcd,   1, 134, 229, 142, 154, 255, 235, true); // bar background
        // bar buttons
        qp_rect(lcd, 230, 134, 238, 142, 255,   0, 230, false); // bar x border
        qp_rect(lcd, 231, 135, 237, 141, 255, 255, 230, true); // bar x background
        qp_line(lcd, 232, 136, 236, 140, 255,   0, 255); // bar x
        qp_line(lcd, 232, 140, 236, 136, 255,   0, 255); // bar x
    }

    if (mode != display_mode || first_draw) {
        mode = display_mode;
        draw_interface(display_mode);
        if (mode == 1) {
            // Re-initialize terminal when switching to terminal mode
            terminal_initialized = false;
            init_terminal_display();
        }
    }

    // Always try to draw terminal (it will self-throttle)
    if (mode == 1) {
        draw_terminal();
    }

    // Throttle other UI updates to reduce flashing
    if (timer_elapsed32(last_full_redraw) < 100 && !first_draw) {
        qp_flush(lcd);
        first_draw = false;
        return;
    }


    uint16_t x = 8;
    uint16_t y = 13;
    char buf[32];

    if (first_draw) {
        qp_rect(lcd, x-3, y+4, x + qp_textwidth(font_oled, "03 MOUSE") + 3, y +(font_oled->line_height * 2) + 8, 0, 0, 0, false);
        qp_drawtext_recolor(
            lcd, x, y, font_oled, "Layer",
            0,
            0,
            0,
            37, 40, 242
        );
    }

    y += font_oled->line_height + 6;

    uint8_t layer = get_highest_layer(layer_state);

    if (layer != last_layer) {
        last_layer = layer;

        // Clear the area first (background)
        uint16_t text_width = qp_textwidth(font_oled, "03 MOUSE"); // max possible width
        uint16_t text_height = font_oled->line_height;
        qp_rect(
            lcd,
            x,
            y,
            x + text_width,
            y + text_height - 1,
            37, 40, 242, true
        );

        switch (layer) {
            case 1:
                qp_drawtext_recolor(
                    lcd, x, y, font_oled, "01 FUNC",
                    0,
                    0,
                    0,
                    37, 40, 242
                );
                break;

            case 2:
                qp_drawtext_recolor(
                    lcd, x, y, font_oled, "02 NAV",
                    0,
                    0,
                    0,
                    37, 40, 242
                );
                break;

            case 3:
                qp_drawtext_recolor(
                    lcd, x, y, font_oled, "03 MOUSE",
                    0,
                    0,
                    0,
                    37, 40, 242
                );
                break;

            case 4:
                qp_drawtext_recolor(
                    lcd, x, y, font_oled, "04 CUST",
                    0,
                    0,
                    0,
                    37, 40, 242
                );
                break;

            default:
                qp_drawtext_recolor(
                    lcd, x, y, font_oled, "00 BASE",
                    0,
                    0,
                    0,
                    37, 40, 242
                );
                break;
        }
    }

    y = 13;
    x += qp_textwidth(font_oled, "03 MOUSE") + 11;

    if (first_draw) {
        qp_rect(lcd, x-3, y+4, width - 8, y +(font_oled->line_height * 2) + 8, 0, 0, 0, false);
        qp_drawtext_recolor(
            lcd, x, y, font_oled, "DPI",
            0,
            0,
            0,
            37, 40, 242
        );
    }

    y += font_oled->line_height + 6;


#if defined(POINTING_DEVICE_ENABLE)
    uint16_t dpi = dpi_state.dpi;
    uint16_t sniping_dpi = dpi_state.sniping_dpi;
    if (last_dpi != dpi) {
        last_dpi = dpi;
        draw_dpi_bar(x, y, dpi, x + (width / 2) - 8, font_oled, false);
    }
    x = x + (width / 2) - 4;
    if (last_sdpi != sniping_dpi) {
        last_sdpi = sniping_dpi;
        draw_dpi_bar(x, y, sniping_dpi, width - 8, font_oled, true);
    }
#endif


    y = 13 + (font_oled->line_height * 2) + 12;
    x = 8;


#if defined(RGB_MATRIX_ENABLE)
    hsv_t current_hsv = rgb_matrix_get_hsv();
    uint8_t current_mode = rgb_matrix_get_mode();

    if (last_rgb != current_mode ||
        last_rgb_hsv.h != current_hsv.h ||
        last_rgb_hsv.s != current_hsv.s ||
        last_rgb_hsv.v != current_hsv.v) {

        if (first_draw) {
            qp_rect(lcd, x-3, y+4, x + qp_textwidth(font_oled, "XXXXXXXXXXXXXXXXXXXXXXXXX") + 3, y + (font_oled->line_height * 3) + 14, 0, 0, 0, false);
            qp_drawtext_recolor(
                lcd, x, y, font_oled, "RGB",
                0,
                0,
                0,
                37, 40, 242
            );
        }

        y += font_oled->line_height + 6;

        last_rgb = current_mode;
        last_rgb_hsv = current_hsv;

        uint16_t text_width  = qp_textwidth(font_oled, "XXXXXXXXXXXXXXXXXXXXXXXXX"); // effect name
        uint16_t hsv_width   = qp_textwidth(font_oled, "255, 255, 255");
        uint16_t text_height = font_oled->line_height;

        /* Clear background */
        qp_rect(
            lcd,
            x,
            y,
            x + text_width,
            y + text_height,
            37, 40, 242,
            true
        );

        /* Effect name */
        snprintf(buf, sizeof(buf), "%s", rgb_matrix_get_effect_name());
        qp_drawtext_recolor(
            lcd, x, y, font_oled, buf,
            0,
            0,
            0,
            37, 40, 242
        );

        y += font_oled->line_height + 4;
        /* Clear background */
        qp_rect(
            lcd,
            x,
            y,
            x + hsv_width,
            y + text_height,
            37, 40, 242,
            true
        );

        /* HSV text */
        snprintf(
            buf, sizeof(buf),
            "%3u, %3u, %3u",
            last_rgb_hsv.h,
            last_rgb_hsv.s,
            last_rgb_hsv.v
        );
        qp_drawtext_recolor(
            lcd,
            x + 4,
            y,
            font_oled,
            buf,
            0,
            0,
            0,
            37, 40, 242
        );

        /* HSV color swatch */
        qp_rect(
            lcd,
            x + hsv_width + 4,
            y - 1,
            x + hsv_width + 4 + 10,
            y + 9,
            last_rgb_hsv.h,
            last_rgb_hsv.s,
            last_rgb_hsv.v,
            true
        );
    }
#endif


    y = 11 + (font_oled->line_height * 2) + 12;
    x += qp_textwidth(font_oled, "XXXXXXXXXXXXXXXXXXXXXXXXX") + 11;

    if (first_draw) {
        qp_rect(lcd, x-3, y+4, width - 8, y + (font_oled->line_height * 3) + 14, 0, 0, 0, false);
        qp_drawtext_recolor(
            lcd, x, y, font_oled, "Modifiers",
            0,
            0,
            0,
            37, 40, 242
        );
    }

    y += font_oled->line_height + 6;

    painter_render_modifiers(
        lcd,               // painter_device_t
        font_oled,         // font (used for "Modifiers:" label)
        x,                 // x start
        y,                 // y start
        width,             // available width
        first_draw,        // force redraw on first frame
        &user_hsv,         // HSV theme struct
        80                 // disabled_val (dimmed brightness)
    );


    y += font_oled->line_height + 4;

#if defined (CAPS_WORD_ENABLE)
    bool caps = is_caps_on;
    if (last_caps != caps || first_draw) {
        uint16_t text_width  = qp_textwidth(font_oled, "CAPS OFF"); // max expected length
        uint16_t text_height = font_oled->line_height;
        qp_rect(lcd, x, y, x + text_width, y + text_height - 1, 37, 40, 242, true); // fill with background

        // Draw new caps word state
        last_caps = caps;
        snprintf(buf, sizeof(buf), "CAPS %s", caps ? "ON" : "OFF");
        qp_drawtext_recolor(lcd, x, y, font_oled, buf,
                    caps ? 0 : 0,
                    caps ? 255 : 0,
                    caps ? 230 : 0,
                    37, 40, 242
        );
    }
#endif
    x = 8;
    y += font_oled->line_height + 11;

    if (first_draw) {
        qp_rect(lcd, x-3, y+4, (x + qp_textwidth(font_oled, "XXX") * 3) + 3, y + (font_oled->line_height * 4) + 14, 0, 0, 0, false);
        qp_drawtext_recolor(
            lcd, x, y, font_oled, "WPM",
            0,
            0,
            0,
            37, 40, 242
        );
    }

    x += (qp_textwidth(font_oled, "999") * 4) + 11;

    if (first_draw) {
        qp_rect(lcd, x-3, y+4, width - 8, y + (font_oled->line_height * 4) + 14, 0, 0, 0, false);
        qp_drawtext_recolor(
            lcd, x, y, font_oled, "WPM GRAPH",
            0,
            0,
            0,
            37, 40, 242
        );
    }
    x = 8;
    y += font_oled->line_height + 2;

#ifdef WPM_ENABLE
    uint16_t wpm = get_current_wpm();
    uint16_t text_width  = qp_textwidth(font_oled, "999"); // max expected digits
    uint16_t text_height = font_oled->line_height;
    if (last_wpm != wpm || first_draw) {
        // Clear previous WPM text area
        if (first_draw) {
            qp_rect(lcd, x, y, x + (text_width * 3), y + (text_height * 4), 0, 0, 0, true); // fill with background
        }

        qp_rect(lcd, x + text_width, y + (text_height * 3), x + (text_width * 2), y + (text_height * 4), 0, 0, 0, true); // fill with background
        //wpm_animation(wpm, last_wpm);

        draw_wpm_indicator(x, y, text_width, text_height, wpm);
        // Draw new WPM
        last_wpm = wpm;
        snprintf(buf, sizeof(buf), "%3u", last_wpm);
        qp_drawtext_recolor(lcd, x + text_width, y + (text_height * 3), font_oled, buf,
                    85,
                    180,
                    194,
                    0, 0, 0
        );

    }
    x += (text_width * 4) + 11;
    painter_render_wpm_graph(lcd, font_oled, x, y, width - x - 11, text_height * 4, false, &user_hsv);
    x += qp_textwidth(font_oled, "WPM: 999") + 4;
#endif

    first_draw = false;
    qp_flush(lcd);
}


void keyboard_post_init_keymap(void) {
#ifdef QUANTUM_PAINTER_ENABLE
    if (is_keyboard_left()) {
        wait_ms(LCD_WAIT_TIME);
        user_hsv.primary.h   = 200;  // base color (blue-ish)
        user_hsv.primary.s   = 255;
        user_hsv.primary.v   = 150;

        user_hsv.secondary.h = 40;   // active modifier color (yellow/orange)
        user_hsv.secondary.s = 255;
        user_hsv.secondary.v = 255;


        lcd = qp_ili9341_make_spi_device(LCD_WIDTH, LCD_HEIGHT, LCD_CS_PIN, LCD_DC_PIN, LCD_RST_PIN, LCD_SPI_DIVISOR, SPI_MODE);

        qp_init(lcd, LCD_ROTATION);

        qp_power(lcd, true);
        qp_clear(lcd);

        // load fonts
        font_oled = qp_load_font_mem(font_oled_font);

        bloodborne   = qp_load_image_mem(gfx_bloodborne);

        shift_icon   = qp_load_image_mem(gfx_shift_icon);
        control_icon = qp_load_image_mem(gfx_control_icon);
        alt_icon     = qp_load_image_mem(gfx_alt_icon);
        windows_icon = qp_load_image_mem(gfx_windows_icon);
    }
#endif // QUANTUM_PAINTER_ENABLE
}

void keyboard_post_init_user(void) {
    transaction_register_rpc(USER_SYNC_DPI, user_sync_dpi_recv);
    transaction_register_rpc(USER_SYNC_CAPS_WORD, user_sync_caps_word_recv);
    transaction_register_rpc(USER_SYNC_MODE, user_sync_mode_recv);
    transaction_register_rpc(USER_SYNC_KEYLOGGER, user_sync_keylogger_recv);

    keylogger_init();

    keyboard_post_init_keymap();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Process key for keylogger only when terminal is visible for security reasons
    if (display_mode == 1) {
        keylogger_process_key(keycode, record);
    }

    switch (keycode) {
        case QK_USER_15:
            if (record->event.pressed) {
                display_mode = (display_mode + 1) % 2;  // Toggle between 0 and 1
                return false;
            }
            break;
        case QK_USER_16:
            if (record->event.pressed && display_mode == 1) {
                keylogger_clear_buffer();
                // Mark all lines as dirty for redraw
                for (int i = 0; i < MAX_TERMINAL_LINES; i++) {
                    terminal_lines[i].dirty = true;
                }
                keylogger_set_dirty(true);
                return false;
            }
    }

    return true;
}

void housekeeping_task_user(void) {
#ifdef QUANTUM_PAINTER_ENABLE
    static uint32_t last_draw = 0;
    static uint32_t last_backlight_update = 0;
    static uint8_t current_backlight_level = BACKLIGHT_DEFAULT_LEVEL;

    // Only handle display updates on left side
    if (is_keyboard_left()) {
        if (timer_elapsed32(last_draw) > 33) { // throttle to ~30 FPS
            last_draw = timer_read32();
            ili9341_draw_user();
        }
    }

    // Throttle backlight updates to prevent flicker
    if (timer_elapsed32(last_backlight_update) < 50) { // Update at most every 50ms
        return;
    }
    last_backlight_update = timer_read32();

    uint32_t activity_time = last_input_activity_elapsed();
    uint8_t target_backlight_level;

    // Determine target backlight level based on activity time
    if (activity_time > QUANTUM_PAINTER_DISPLAY_TIMEOUT) {
        target_backlight_level = 0; // Timed out - completely off
        // also automatically clear keylogger
        keylogger_clear_buffer();
    } else if (activity_time > (QUANTUM_PAINTER_DISPLAY_TIMEOUT / 4)) {
        target_backlight_level = 1; // Low brightness (standby)
    } else if (activity_time > (QUANTUM_PAINTER_DISPLAY_TIMEOUT / 8)) {
        target_backlight_level = 5; // Medium brightness
    } else {
        target_backlight_level = BACKLIGHT_DEFAULT_LEVEL; // Full brightness
    }

    // Only update backlight if the level has changed
    if (target_backlight_level != current_backlight_level) {
        current_backlight_level = target_backlight_level;
        backlight_level_noeeprom(target_backlight_level);
    }
#endif
    static uint16_t interval = 0;

    if (timer_elapsed(interval) >= 100) {
        update_wpm_avg();
        interval = timer_read();
    }
    if (is_keyboard_master()) {
#if defined(POINTING_DEVICE_ENABLE)
        uint16_t dpi = pointing_device_get_cpi();
        uint16_t sniping = dilemma_get_pointer_sniping_dpi();

        if (dpi_state.dpi != dpi || dpi_state.sniping_dpi != sniping) {
            dpi_state.dpi = dpi;
            dpi_state.sniping_dpi = sniping;
            transaction_rpc_send(USER_SYNC_DPI, sizeof(dpi_state), &dpi_state);
        }
#endif
#if defined(CAPS_WORD_ENABLE)
        uint16_t caps = is_caps_word_on();

        if (is_caps_on != caps) {
            is_caps_on = caps;
            transaction_rpc_send(USER_SYNC_CAPS_WORD, sizeof(is_caps_on), &is_caps_on);
        }
#endif
        static uint8_t mode = 0xFF;
        static bool first_sync = true;
        static uint32_t last_sync_time = 0;
        static uint8_t sync_counter = 0;
        static uint8_t current_line_to_sync = 0;
        static char last_sent_lines[MAX_TERMINAL_LINES][TERMINAL_WIDTH + 1] = {0};
        static uint8_t last_current_line = 0xFF;

        if (mode != display_mode) {
            // Store the old mode before updating
            uint8_t old_mode = mode;
            mode = display_mode;

            // If switching away from terminal mode (1 → 0), clear the buffer
            if (old_mode == 1 && mode == 0) {
                keylogger_clear_buffer();

                // Clear last_sent_lines so we'll sync empty lines if we switch back
                for (int i = 0; i < MAX_TERMINAL_LINES; i++) {
                    last_sent_lines[i][0] = '\0';
                }
                last_current_line = 0xFF;

                // Force a sync of empty lines to clear the slave's terminal
                for (uint8_t line_num = 0; line_num < MAX_TERMINAL_LINES; line_num++) {
                    keylogger_sync_t sync_data;
                    sync_data.line[0] = '\0';  // Empty line
                    sync_data.line_num = line_num;
                    sync_data.current_line =0;
                    sync_data.sync_counter = sync_counter++;

                    transaction_rpc_send(USER_SYNC_KEYLOGGER, sizeof(sync_data), &sync_data);
                    wait_ms(1);  // Small delay to prevent overwhelming the bus
                }
            }

            // If switching to terminal mode (0 → 1), force a fresh sync
            if (old_mode == 0 && mode == 1) {
                first_sync = true;
                current_line_to_sync = 0;
            }

            transaction_rpc_send(USER_SYNC_MODE, sizeof(mode), &mode);
        }

        // BUFFERED KEYLOGGER SYNC - Throttled to prevent lag
        if (display_mode == 1) {
            uint8_t current_line = keylogger_get_current_line();
            uint32_t now = timer_read32();

            // Throttle sync to every 16ms (~60 FPS max for sync)
            if (timer_elapsed32(last_sync_time) > 16) {
                // If first sync, sync all lines initially
                if (first_sync) {
                    for (uint8_t line_num = 0; line_num < MAX_TERMINAL_LINES; line_num++) {
                        char current_line_text[TERMINAL_WIDTH + 1];
                        keylogger_get_line(line_num, current_line_text);

                        keylogger_sync_t sync_data;
                        strncpy(sync_data.line, current_line_text, TERMINAL_WIDTH);
                        sync_data.line_num = line_num;
                        sync_data.current_line = current_line;
                        sync_data.sync_counter = sync_counter++;

                        transaction_rpc_send(USER_SYNC_KEYLOGGER, sizeof(sync_data), &sync_data);

                        strncpy(last_sent_lines[line_num], current_line_text, TERMINAL_WIDTH);
                        last_sent_lines[line_num][TERMINAL_WIDTH] = '\0';

                        wait_ms(1);
                    }
                    last_current_line = current_line;
                    first_sync = false;
                } else {
                    // Round-robin: check one line per sync cycle
                    uint8_t line_num = current_line_to_sync;
                    char current_line_text[TERMINAL_WIDTH + 1];
                    keylogger_get_line(line_num, current_line_text);

                    bool is_current = (line_num == current_line);
                    bool line_changed = (strcmp(current_line_text, last_sent_lines[line_num]) != 0);
                    bool current_status_changed = (is_current != (line_num == last_current_line));

                    // Only sync if something changed
                    if (line_changed || current_status_changed) {
                        keylogger_sync_t sync_data;
                        strncpy(sync_data.line, current_line_text, TERMINAL_WIDTH);
                        sync_data.line_num = line_num;
                        sync_data.current_line = current_line;
                        sync_data.sync_counter = sync_counter++;

                        transaction_rpc_send(USER_SYNC_KEYLOGGER, sizeof(sync_data), &sync_data);

                        strncpy(last_sent_lines[line_num], current_line_text, TERMINAL_WIDTH);
                        last_sent_lines[line_num][TERMINAL_WIDTH] = '\0';
                    }

                    // Move to next line for next cycle
                    current_line_to_sync = (current_line_to_sync + 1) % MAX_TERMINAL_LINES;
                }

                last_current_line = current_line;
                last_sync_time = now;
            }
        }
    }
}
