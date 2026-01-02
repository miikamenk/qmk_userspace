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
#    include "gfx/dark-souls.qgf.c"
#    include "gfx/font_oled.qff.c"
#    include "gfx/fonts.qff.c"
#    include "gfx/mew.qgf.c"


typedef struct {
    hsv_t primary;
    hsv_t secondary;
} dual_hsv_t;

static dual_hsv_t user_hsv;

painter_device_t lcd;
painter_font_handle_t         font_oled;
painter_font_handle_t         font_menu;
static painter_image_handle_t shift_icon, control_icon, alt_icon,  windows_icon;
static painter_image_handle_t dark_souls;
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
    snprintf(buf, sizeof(buf), sniping ? "sdpi:%4u" : " dpi:%4u", dpi);
    qp_drawtext(lcd, x, y, font, buf);

    // Calculate bar position and dimensions
    uint16_t text_width = qp_textwidth(font, "sdpi:9999");
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
                                     2000, 2200, 2400, 2600, 2800, 3000, 3200};
    const uint16_t normal_step_count = 14;

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
                        255, 90, 90,  // Red color for sniper mode
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
                                 last_mods & shift ? user_hsv->primary.v : disabled_val, 0, 0, 0)) {
            x += shift_icon->width + 2;
        }

        if (qp_drawimage_recolor(device, x, y, windows_icon,
                                 last_mods & gui ? user_hsv->secondary.h : user_hsv->primary.h,
                                 last_mods & gui ? user_hsv->secondary.s : user_hsv->primary.s,
                                 last_mods & gui ? user_hsv->primary.v : disabled_val, 0, 0, 0)) {
            x += windows_icon->width + 2;
        }

        if (qp_drawimage_recolor(device, x, y, alt_icon,
                                 last_mods & alt ? user_hsv->secondary.h : user_hsv->primary.h,
                                 last_mods & alt ? user_hsv->secondary.s : user_hsv->primary.s,
                                 last_mods & alt ? user_hsv->primary.v : disabled_val, 0, 0, 0)) {
            x += alt_icon->width + 2;
        }

        if (qp_drawimage_recolor(device, x, y, control_icon,
                                 last_mods & ctrl ? user_hsv->secondary.h : user_hsv->primary.h,
                                 last_mods & ctrl ? user_hsv->secondary.s : user_hsv->primary.s,
                                 last_mods & ctrl ? user_hsv->primary.v : disabled_val, 0, 0, 0)) {
            x += control_icon->width + 2;
        }
    }
}


// removed for now, keeping here if I add a different wpm animation
// void wpm_animation(uint16_t wpm, uint16_t last_wpm) {
//     // Clean up animations when transitioning between states
//     if (wpm < 20) {
//         // Coming from higher WPM states to sleep
//         if (mew_twirl_token != INVALID_DEFERRED_TOKEN) {
//             qp_stop_animation(mew_twirl_token);
//             mew_twirl_token = INVALID_DEFERRED_TOKEN;
//         }
//         if (mew_hop_token != INVALID_DEFERRED_TOKEN) {
//             qp_stop_animation(mew_hop_token);
//             mew_hop_token = INVALID_DEFERRED_TOKEN;
//         }
//
//         // Start sleep animation if not already running
//         if (mew_sleep_token == INVALID_DEFERRED_TOKEN) {
//             // Clear the animation area (212,0 to 236,60)
//             qp_rect(lcd, 212, 0, 236, 60, 0, 0, 0, true);
//             // Create and start sleep animation
//             mew_sleep_token = qp_animate(lcd, 212, 28, mew_sleep);
//         }
//     }
//     else if (wpm >= 20 && wpm <= 60) {
//         // TWIRL state
//         // Was in sleep, stop it
//         if (mew_sleep_token != INVALID_DEFERRED_TOKEN) {
//             qp_stop_animation(mew_sleep_token);
//             mew_sleep_token = INVALID_DEFERRED_TOKEN;
//         }
//         // Was in hop, stop it
//         if (mew_hop_token != INVALID_DEFERRED_TOKEN) {
//             qp_stop_animation(mew_hop_token);
//             mew_hop_token = INVALID_DEFERRED_TOKEN;
//         }
//
//         // Start twirl animation if not already running
//         if (mew_twirl_token == INVALID_DEFERRED_TOKEN) {
//             // Clear the animation area
//             qp_rect(lcd, 212, 0, 236, 60, 0, 0, 0, true);
//
//             mew_twirl_token = qp_animate(lcd, 212, 20, mew_twirl);
//         }
//     }
//     else { // wpm > 60
//         // HOP state
//         // Was in sleep, stop it
//         if (mew_sleep_token != INVALID_DEFERRED_TOKEN) {
//             qp_stop_animation(mew_sleep_token);
//             mew_sleep_token = INVALID_DEFERRED_TOKEN;
//         }
//         // Was in twirl, stop it
//         if (mew_twirl_token != INVALID_DEFERRED_TOKEN) {
//             qp_stop_animation(mew_twirl_token);
//             mew_twirl_token = INVALID_DEFERRED_TOKEN;
//         }
//
//         // Start hop animation if not already running
//         if (mew_hop_token == INVALID_DEFERRED_TOKEN) {
//             // Clear the animation area
//             qp_rect(lcd, 212, 0, 236, 60, 0, 0, 0, true);
//
//             mew_hop_token = qp_animate(lcd, 212, 0, mew_hop);
//         }
//     }
// }

void draw_interface(uint8_t mode) {
    qp_rect(lcd,   5,  65, 218,  73, 140, 173, 230, true);
    qp_rect(lcd,   9,  76, 229,  84, 140, 173, 230, true);
    switch (mode) {
        case 0:
            // window 2 title
            qp_drawtext_recolor(lcd, 6, 65, font_oled, "Terminal",
                        0,
                        0,
                        0,
                        140, 173, 230
            );
            // window 3 title
            qp_drawtext_recolor(lcd, 10, 77, font_oled, "Dark Souls 3",
                        0,
                        0,
                        0,
                        140, 173, 230
            );
            qp_drawimage(lcd, 9, 86, dark_souls);
            break;
        case 1:
            // window 2 title
            qp_drawtext_recolor(lcd, 6, 65, font_oled, "Dark Souls 3",
                        0,
                        0,
                        0,
                        140, 173, 230
            );
            // window 3 title
            qp_drawtext_recolor(lcd, 10, 77, font_oled, "Terminal",
                        0,
                        0,
                        0,
                        140, 173, 230
            );
            qp_rect(lcd,  9,  86, 239, 251,  85, 237,  59, true);
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
    qp_rect(lcd, 9, 86, 239, 251, 85, 237, 59, true);

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
            uint16_t y = 87 + (i * (font_oled->line_height + 4));

            // Clear the line area (from x=9 to x=239, line height + spacing)
            qp_rect(lcd, 9, y - 2, 239, y + font_oled->line_height + 2,
                   85, 237, 59, true);

            if (terminal_lines[i].length > 0) {
                // Draw the line
                qp_drawtext_recolor(lcd, 10, y, font_oled, terminal_lines[i].text,
                                   0, 0, 0,        // Black text
                                   85, 237, 59);   // Green background

                // Draw cursor on current line
                if (terminal_lines[i].is_current) {
                    // Draw a simple cursor at the end of the line
                    uint16_t cursor_x = 10 + qp_textwidth(font_oled, terminal_lines[i].text);
                    qp_rect(lcd, cursor_x, y + font_oled->line_height - 2,
                           cursor_x + 2, y + font_oled->line_height,
                           0, 0, 0, true);
                }
            }

            terminal_lines[i].dirty = false;
        }
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
    static uint8_t       last_rgb   = 0xFF;
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
        qp_line(lcd,   0,  53, 218,  53, 140,  86, 255); // top
        qp_line(lcd,   0,  53,   0, 196, 140,  86, 255); // left
        qp_line(lcd,   1,  63, 219,  63, 140, 226, 150); // bar bottom
        qp_line(lcd, 219,  53, 219,  64, 140, 226, 150); // right
        qp_line(lcd,   0, 195,   3, 195, 140, 226, 150); // window bottom
        qp_rect(lcd,   1,  56,   4, 193, 255,   0, 204, true);
        qp_rect(lcd,   1,  54, 209,  62, 140, 173, 230, true);
        qp_drawtext_recolor(lcd, 2, 54, font_oled, "Internet Explorer",
                    0,
                    0,
                    0,
                    140, 173, 230
        );
        // window #2
        qp_line(lcd,   4,  64, 227,  64, 140,  86, 255); // top
        qp_line(lcd,   4,  64,   4, 228, 140,  86, 255); // left
        qp_line(lcd,   5,  74, 228,  74, 140,  89, 150); // bar bottom
        qp_line(lcd, 228,  64, 228,  73, 140, 226, 150); // right
        qp_line(lcd,   4, 228,   7, 228, 140, 226, 150); // window bottom
        qp_rect(lcd,   5,  67,   8, 229, 255,   0, 204, true);
        qp_rect(lcd,   5,  65, 218,  73, 140, 173, 230, true);
        // window #3
        qp_line(lcd,   8,  75, 238,  75, 140,  86, 255); // top
        qp_line(lcd,   8,  75,   8, 252, 140,  86, 255); // left
        qp_line(lcd,   9,  85, 239,  85, 140, 226, 150); // bar bottom
        qp_line(lcd, 239,  75, 239, 253, 140, 226, 150); // right
        qp_line(lcd,   8, 253, 239, 253, 140, 226, 150); // window bottom
        qp_rect(lcd,   9,  76, 229,  84, 140, 173, 230, true);
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

    uint16_t x = 4;
    uint16_t y = height - font_oled->line_height - 4;
    char buf[32];

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


    x = 55;
#ifdef WPM_ENABLE
    uint16_t wpm = get_current_wpm();
    if (last_wpm != wpm || first_draw) {
        // Clear previous WPM text area
        uint16_t text_width  = qp_textwidth(font_oled, "WPM: 999"); // max expected digits
        uint16_t text_height = font_oled->line_height;
        qp_rect(lcd, x, y, x + text_width, y + text_height - 1, 0, 0, 0, true); // fill with background

        //wpm_animation(wpm, last_wpm);

        // Draw new WPM
        last_wpm = wpm;
        snprintf(buf, sizeof(buf), "WPM: %u", last_wpm);
        qp_drawtext_recolor(lcd, x, y, font_oled, buf,
                    user_hsv.secondary.h,
                    user_hsv.secondary.s,
                    user_hsv.secondary.v,
                    0, 0, 0
        );
    }
    x += qp_textwidth(font_oled, "WPM: 999") + 4;
#endif

#if defined (CAPS_WORD_ENABLE)
    bool caps = is_caps_on;
    if (last_caps != caps || first_draw) {
        uint16_t text_width  = qp_textwidth(font_oled, "CAPS OFF"); // max expected length
        uint16_t text_height = font_oled->line_height;
        qp_rect(lcd, x, y, x + text_width, y + text_height - 1, 0, 0, 0, true); // fill with background

        // Draw new caps word state
        last_caps = caps;
        snprintf(buf, sizeof(buf), "CAPS %s", caps ? "ON" : "OFF");
        qp_drawtext_recolor(lcd, x, y, font_oled, buf,
                    caps ? user_hsv.secondary.h : user_hsv.primary.h,
                    caps ? user_hsv.secondary.s : user_hsv.primary.s,
                    caps ? user_hsv.secondary.v : user_hsv.primary.v,
                    0, 0, 0
        );
    }
#endif
    y = 6;

#if defined(POINTING_DEVICE_ENABLE)
    uint16_t dpi = dpi_state.dpi;
    uint16_t sniping_dpi = dpi_state.sniping_dpi;
    if (last_dpi != dpi) {
        last_dpi = dpi;
        draw_dpi_bar(120, y, dpi, width, font_oled, false);
    }
    y += font_oled->line_height + 4;
    if (last_sdpi != sniping_dpi) {
        last_sdpi = sniping_dpi;
        draw_dpi_bar(120, y, sniping_dpi, width, font_oled, true);
    }
#endif

    x = 4;
    y = 6;

    uint8_t layer = get_highest_layer(layer_state);

    if (layer != last_layer) {
        last_layer = layer;

        // Clear the area first (background)
        uint16_t text_width = qp_textwidth(font_menu, "03 MOUSE"); // max possible width
        uint16_t text_height = font_menu->line_height;
        qp_rect(
            lcd,
            x,
            y,
            x + text_width,
            y + text_height - 1,
            0, 0, 0, true
        );

        // Draw the new text using primary HSV color
        switch (layer) {
            case 1:
                qp_drawtext_recolor(
                    lcd, x, y, font_menu, "01 FUNC",
                    user_hsv.primary.h,
                    user_hsv.primary.s,
                    user_hsv.primary.v,
                    0, 0, 0
                );
                break;

            case 2:
                qp_drawtext_recolor(
                    lcd, x, y, font_menu, "02 NAV",
                    user_hsv.primary.h,
                    user_hsv.primary.s,
                    user_hsv.primary.v,
                    0, 0, 0
                );
                break;

            case 3:
                qp_drawtext_recolor(
                    lcd, x, y, font_menu, "03 MOUSE",
                    user_hsv.primary.h,
                    user_hsv.primary.s,
                    user_hsv.primary.v,
                    0, 0, 0
                );
                break;

            default:
                qp_drawtext_recolor(
                    lcd, x, y, font_menu, "00 BASE",
                    user_hsv.primary.h,
                    user_hsv.primary.s,
                    user_hsv.primary.v,
                    0, 0, 0
                );
                break;
        }
    }

    y += font_menu->line_height + 10;
    x = 4;


#if defined(RGB_MATRIX_ENABLE)
    if (last_rgb != rgb_matrix_get_mode()) {
        last_rgb = rgb_matrix_get_mode();
        uint16_t text_width  = qp_textwidth(font_oled, "RGB: XXXXXXXXXXXXXXXXXXXXXXXXX"); // max expected digits
        uint16_t text_height = font_oled->line_height;
        qp_rect(lcd, x, y, x + text_width, y + text_height - 2, 0, 0, 0, true); // fill with background
        snprintf(buf, sizeof(buf), "RGB: %s", rgb_matrix_get_mode_name(last_rgb));
        qp_drawtext_recolor(lcd, x, y, font_oled, buf,
                    user_hsv.secondary.h,
                    user_hsv.secondary.s,
                    user_hsv.secondary.v,
                    0, 0, 0
        );
    }
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
        font_menu = qp_load_font_mem(font_gridlitepbsmenu);

        dark_souls   = qp_load_image_mem(gfx_dark_souls);

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
                    sync_data.current_line = 0;
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
