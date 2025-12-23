/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 * Copyright 2023 casuanoob <casuanoob@hotmail.com> (@casuanoob)
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
       KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,       KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LCTL,    PT_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M, KC_COMM,  KC_DOT, PT_SLSH, KC_LALT,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                         KC_LALT, KC_BSPC,  KC_SPC,   LOWER,      RAISE,  KC_ENT, KC_DEL,  KC_MUTE
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
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DPI_MOD, S_D_MOD,    S_D_MOD, DPI_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
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

#ifdef POINTING_DEVICE_ENABLE
#    ifdef DILEMMA_AUTO_SNIPING_ON_LAYER
    layer_state_t layer_state_set_user(layer_state_t state) {
        dilemma_set_pointer_sniping_enabled(layer_state_cmp(state, DILEMMA_AUTO_SNIPING_ON_LAYER));
        return state;
    }
#    endif // DILEMMA_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLEE

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


typedef struct {
    hsv_t primary;
    hsv_t secondary;
} dual_hsv_t;

static dual_hsv_t user_hsv;

painter_device_t lcd;
painter_font_handle_t font_oled;
painter_font_handle_t font_menu;
static painter_image_handle_t shift_icon, control_icon, alt_icon,  windows_icon;
static painter_image_handle_t apple_logo, windows_logo, linux_logo;
static painter_image_handle_t dark_souls;
#endif // QUANTUM_PAINTER_ENABLE


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

void painter_render_os_detection(painter_device_t device, uint16_t x, uint16_t y,
                                 bool force_redraw, dual_hsv_t *curr_hsv) {
#ifdef OS_DETECTION_ENABLE
    static os_variant_t last_detected_os = {0};
    os_variant_t        current_detected_os = detected_host_os();

    if (force_redraw || last_detected_os != current_detected_os) {
        last_detected_os = current_detected_os;

        // Draw OS logo based on detected OS
        switch (current_detected_os) {
            case OS_WINDOWS:
                qp_drawimage_recolor(device, x, y, windows_logo,
                                     curr_hsv->secondary.h, curr_hsv->secondary.s, curr_hsv->secondary.v,
                                     0, 0, 0);
                x += windows_logo->width + 2;
                break;

            case OS_MACOS:
                qp_drawimage_recolor(device, x, y, apple_logo,
                                     curr_hsv->secondary.h, curr_hsv->secondary.s, curr_hsv->secondary.v,
                                     0, 0, 0);
                x += apple_logo->width + 2;
                break;

            case OS_LINUX:
                qp_drawimage_recolor(device, x, y, linux_logo,
                                     curr_hsv->secondary.h, curr_hsv->secondary.s, curr_hsv->secondary.v,
                                     0, 0, 0);
                x += linux_logo->width + 2;
                break;

            default:
                qp_drawimage_recolor(device, x, y, linux_logo,
                                     curr_hsv->secondary.h, curr_hsv->secondary.s, curr_hsv->secondary.v,
                                     0, 0, 0);
                x += linux_logo->width + 2;
                break;
        }
    }
#endif
}

void draw_dpi_bar(uint16_t x, uint16_t y, uint16_t dpi, uint16_t width, painter_font_handle_t font) {
    static const uint16_t dpi_steps[] = {1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400,
                                         2600, 2800, 3000, 3200, 3400, 4000, 6000, 8000};
    const size_t step_count = sizeof(dpi_steps) / sizeof(dpi_steps[0]);

    char buf[16];
    snprintf(buf, sizeof(buf), "DPI: %u", dpi);

    // Draw DPI text
    qp_drawtext(lcd, x, y, font, buf);

    // Start the bar right after the text
    uint16_t bar_x = x + qp_textwidth(font, buf) + 4; // 4px spacing
    uint16_t bar_y = y + (font->line_height / 2) - 2; // vertical center
    uint16_t bar_height = 4;
    uint16_t bar_width  = width - bar_x - 4; // padding 4px to the end

    // Draw background (empty bar)
    qp_rect(lcd, bar_x, bar_y, bar_x + bar_width, bar_y + bar_height, 40, 40, 40, true); // dark gray

    // Draw filled steps
    for (size_t i = 0; i < step_count; i++) {
        // Calculate start and end for this step
        uint16_t step_start = bar_x + (bar_width * i) / step_count;
        uint16_t step_end   = bar_x + (bar_width * (i + 1)) / step_count - 1;

        if (dpi >= dpi_steps[i]) {
            qp_rect(lcd, step_start, bar_y, step_end, bar_y + bar_height, 90, 255, 255, true); // green fill
        }
    }
}


void ili9341_draw_user(void) {
    static layer_state_t last_layer = 0xFF;
    static bool          first_draw = true;
#if defined(WPM_ENABLE)
    static uint16_t      last_wpm   = 0;
#endif
#if defined(RGB_MATRIX_ENABLE)
    static uint8_t       last_rgb   = 0xFF;
#endif

#if defined(POINTING_DEVICE_ENABLE)
    static uint16_t last_dpi = 0xFFFF;
#endif

    uint16_t width, height;
    qp_get_geometry(lcd, &width, &height, NULL, NULL, NULL);

    if (first_draw) {
        qp_rect(lcd, 0, 0, width, height, 0, 0, 0, true);
        qp_drawimage(lcd, 0, 53, dark_souls);
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
        &user_hsv,      // your HSV theme struct
        80                 // disabled_val (dimmed brightness)
    );


#if defined(WPM_ENABLE)
    uint16_t wpm = get_current_wpm();
    if (last_wpm != wpm) {
        x = 55;
        // Clear previous WPM text area
        uint16_t text_width  = qp_textwidth(font_oled, "WPM: 999"); // max expected digits
        uint16_t text_height = font_oled->line_height;
        qp_rect(lcd, x, y, x + text_width, y + text_height - 1, 0, 0, 0, true); // fill with background

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
#endif


#if defined(POINTING_DEVICE_ENABLE)
    uint16_t dpi = pointing_device_get_cpi();
    if (last_dpi != dpi) {
        last_dpi = dpi;
        draw_dpi_bar(115, y, dpi, width, font_oled);
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
    painter_render_os_detection(lcd, 180, y, false, &user_hsv);

    y += font_menu->line_height + 10;
    x = 4;


#if defined(RGB_MATRIX_ENABLE)
    if (last_rgb != rgb_matrix_get_mode()) {
        last_rgb = rgb_matrix_get_mode();
        snprintf(buf, sizeof(buf), "RGB: %s", rgb_matrix_get_mode_name(last_rgb));
        qp_drawtext(lcd, x, y, font_oled, buf);
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

        windows_logo = qp_load_image_mem(gfx_windows_logo);
        apple_logo   = qp_load_image_mem(gfx_apple_logo);
        linux_logo   = qp_load_image_mem(gfx_linux_logo);
        dark_souls   = qp_load_image_mem(gfx_dark_souls);

        shift_icon   = qp_load_image_mem(gfx_shift_icon);
        control_icon = qp_load_image_mem(gfx_control_icon);
        alt_icon     = qp_load_image_mem(gfx_alt_icon);
        windows_icon = qp_load_image_mem(gfx_windows_icon);
    }
#endif // QUANTUM_PAINTER_ENABLE
}

void keyboard_post_init_user(void) {
    keyboard_post_init_keymap();
}

void housekeeping_task_user(void) {
#ifdef QUANTUM_PAINTER_ENABLE
    static uint32_t last_draw = 0;

    // Only handle display updates on left side
    if (is_keyboard_left()) {
        if (timer_elapsed32(last_draw) > 33) { // throttle to ~30 FPS
            last_draw = timer_read32();
            ili9341_draw_user();
        }
    }

    // Handle backlight on both halves for consistency
    // This ensures both halves behave the same way regardless of which is master
    uint32_t activity_time = last_input_activity_elapsed();

    if (is_backlight_enabled() && (activity_time > QUANTUM_PAINTER_DISPLAY_TIMEOUT)) {
        // Timeout reached - turn backlight off
        backlight_level_noeeprom(0);
    } else if (activity_time > (QUANTUM_PAINTER_DISPLAY_TIMEOUT / 4)) {
        // Low brightness for standby
        backlight_level_noeeprom(1);
    } else if (activity_time > (QUANTUM_PAINTER_DISPLAY_TIMEOUT / 8)) {
        // Medium brightness
        backlight_level_noeeprom(5);
    } else if (activity_time < QUANTUM_PAINTER_DISPLAY_TIMEOUT) {
        // Normal operation
        backlight_level_noeeprom(BACKLIGHT_DEFAULT_LEVEL);
    }
#endif
}
