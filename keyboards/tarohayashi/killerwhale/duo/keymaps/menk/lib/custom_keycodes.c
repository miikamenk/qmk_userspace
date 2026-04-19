// Copyright 2021 Hayashi (@w_vwbw)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "custom_keycodes.h"
#include "os_detection.h"
#include "lib/common_killerwhale.h"
#include "custom_oled.h"

static uint16_t startup_timer;

// ---- Helpers ----

static bool host_is_apple(void) {
    os_variant_t os = detected_host_os();
    return os == OS_MACOS || os == OS_IOS;
}

// Primary modifier key: CMD on Apple, CTRL elsewhere.
static uint16_t os_cmd_key(void) {
    return host_is_apple() ? KC_LGUI : KC_LCTL;
}

static void tap_os_shortcut(uint16_t tap_kc) {
    uint16_t mod = os_cmd_key();
    register_code(mod);
    tap_code(tap_kc);
    unregister_code(mod);
}

static void tap_os_shift_shortcut(uint16_t tap_kc) {
    uint16_t mod = os_cmd_key();
    register_code(KC_LSFT);
    register_code(mod);
    tap_code(tap_kc);
    unregister_code(mod);
    unregister_code(KC_LSFT);
}

// Hold-to-enter-mode with tap-fallback on quick release.
static void handle_mode_tap_fallback(keyrecord_t *record, uint16_t keycode, uint16_t tap_kc, void (*mode_fn)(bool)) {
    if (record->event.pressed) {
        startup_timer = timer_read();
        mode_fn(true);
    } else {
        mode_fn(false);
        if (timer_elapsed(startup_timer) < TERM_TEMP) {
            tap_code(tap_kc);
        }
    }
    oled_tempch(record->event.pressed, keycode);
}

// Hold a pointer button + slow mode while pressed.
static void handle_btn_slow(keyrecord_t *record, uint16_t keycode, uint16_t btn_kc) {
    if (record->event.pressed) {
        register_code(btn_kc);
        is_slow_mode(true);
    } else {
        is_slow_mode(false);
        unregister_code(btn_kc);
    }
    oled_tempch(record->event.pressed, keycode);
}

// Joystick axis: set on press, zero on release.
static void handle_joy_axis(keyrecord_t *record, uint8_t axis, int16_t value) {
    joystick_set_axis(axis, record->event.pressed ? value : 0);
}

// Cycle a 0..4 counter. Macro because oled_state fields are bitfields
// (can't take their address).
#define CYCLE_OLED_STATE(field) do { if (++(field) > 4) (field) = 0; } while (0)

// ---- Main keycode dispatch ----

bool process_record_addedkeycodes(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case CMD_CTL:
            if (record->event.pressed) register_code(os_cmd_key());
            else                       unregister_code(os_cmd_key());
            oled_tempch(record->event.pressed, keycode);
            return false;

        // ---- OS-aware shortcuts: CMD/CTRL + key ----
        case UNDO:     if (record->event.pressed) { tap_os_shortcut(KC_Z);    oled_interrupt(keycode); } return false;
        case COPY:     if (record->event.pressed) { tap_os_shortcut(KC_C);    oled_interrupt(keycode); } return false;
        case CUT:      if (record->event.pressed) { tap_os_shortcut(KC_X);    oled_interrupt(keycode); } return false;
        case PASTE:    if (record->event.pressed) { tap_os_shortcut(KC_V);    oled_interrupt(keycode); } return false;
        case SC_UP:    if (record->event.pressed) { tap_os_shortcut(KC_PPLS); oled_interrupt(keycode); } return false;
        case SC_DOWN:  if (record->event.pressed) { tap_os_shortcut(KC_PMNS); oled_interrupt(keycode); } return false;
        case SC_RESET: if (record->event.pressed) { tap_os_shortcut(KC_0);    oled_interrupt(keycode); } return false;
        case SAVE:     if (record->event.pressed) { tap_os_shortcut(KC_S);    oled_interrupt(keycode); } return false;
        case CLOSETAB: if (record->event.pressed) { tap_os_shortcut(KC_W);    oled_interrupt(keycode); } return false;

        // ---- OS-aware shortcuts: CMD/CTRL + SHIFT + key ----
        case REDO:    if (record->event.pressed) { tap_os_shift_shortcut(KC_Z); oled_interrupt(keycode); } return false;
        case SAVEAS:  if (record->event.pressed) { tap_os_shift_shortcut(KC_S); oled_interrupt(keycode); } return false;
        case RSTRTAB: if (record->event.pressed) { tap_os_shift_shortcut(KC_T); oled_interrupt(keycode); } return false;

        // Screenshot: always SHIFT+GUI, but key differs per OS.
        case CAPTCHA:
            if (record->event.pressed) {
                register_code(KC_LSFT);
                register_code(KC_LGUI);
                tap_code(host_is_apple() ? KC_4 : KC_S);
                unregister_code(KC_LGUI);
                unregister_code(KC_LSFT);
                oled_interrupt(keycode);
            }
            return false;

        // ---- Tab navigation: CTRL-only on all OSes ----
        case NEXTTAB:
            if (record->event.pressed) {
                register_code(KC_LCTL);
                tap_code(KC_TAB);
                unregister_code(KC_LCTL);
                oled_interrupt(keycode);
            }
            return false;
        case PREVTAB:
            if (record->event.pressed) {
                register_code(KC_LSFT);
                register_code(KC_LCTL);
                tap_code(KC_TAB);
                unregister_code(KC_LCTL);
                unregister_code(KC_LSFT);
                oled_interrupt(keycode);
            }
            return false;

        // ---- Pointer speed ----
        case L_SPD_I:
            if (record->event.pressed) {
                if (kw_config.spd_l + 1 >= SPD_OPTION_MAX) kw_config.spd_l = SPD_OPTION_MAX - 1;
                else                                      kw_config.spd_l++;
                eeconfig_update_kb(kw_config.raw);
                pointing_device_set_cpi_on_side(true, 400 + kw_config.spd_l * 200);
                oled_interrupt(keycode);
            }
            return false;
        case R_SPD_I:
            if (record->event.pressed) {
                if (kw_config.spd_r + 1 >= SPD_OPTION_MAX) kw_config.spd_r = SPD_OPTION_MAX - 1;
                else                                      kw_config.spd_r++;
                eeconfig_update_kb(kw_config.raw);
                pointing_device_set_cpi_on_side(false, 400 + kw_config.spd_r * 200);
                oled_interrupt(keycode);
            }
            return false;
        case L_SPD_D:
            if (record->event.pressed) {
                if (kw_config.spd_l > 0) kw_config.spd_l--;
                eeconfig_update_kb(kw_config.raw);
                pointing_device_set_cpi_on_side(true, 400 + kw_config.spd_l * 200);
                oled_interrupt(keycode);
            }
            return false;
        case R_SPD_D:
            if (record->event.pressed) {
                if (kw_config.spd_r > 0) kw_config.spd_r--;
                eeconfig_update_kb(kw_config.raw);
                pointing_device_set_cpi_on_side(false, 400 + kw_config.spd_r * 200);
                oled_interrupt(keycode);
            }
            return false;

        // ---- Pointer angle ----
        case L_ANG_I:
            if (record->event.pressed) {
                kw_config.angle_l = (kw_config.angle_l + 1) % ANGLE_OPTION_MAX;
                eeconfig_update_kb(kw_config.raw);
                oled_interrupt(keycode);
            }
            return false;
        case R_ANG_I:
            if (record->event.pressed) {
                kw_config.angle_r = (kw_config.angle_r + 1) % ANGLE_OPTION_MAX;
                eeconfig_update_kb(kw_config.raw);
                oled_interrupt(keycode);
            }
            return false;
        case L_ANG_D:
            if (record->event.pressed) {
                kw_config.angle_l = (ANGLE_OPTION_MAX + kw_config.angle_l - 1) % ANGLE_OPTION_MAX;
                eeconfig_update_kb(kw_config.raw);
                oled_interrupt(keycode);
            }
            return false;
        case R_ANG_D:
            if (record->event.pressed) {
                kw_config.angle_r = (ANGLE_OPTION_MAX + kw_config.angle_r - 1) % ANGLE_OPTION_MAX;
                eeconfig_update_kb(kw_config.raw);
                oled_interrupt(keycode);
            }
            return false;

        // ---- Axis inversion ----
        case L_INV:
            if (record->event.pressed) {
                kw_config.inv_l = !kw_config.inv_l;
                eeconfig_update_kb(kw_config.raw);
                oled_interrupt(keycode);
            }
            return false;
        case R_INV:
            if (record->event.pressed) {
                kw_config.inv_r = !kw_config.inv_r;
                eeconfig_update_kb(kw_config.raw);
                oled_interrupt(keycode);
            }
            return false;
        case INV_SCRL:
            if (record->event.pressed) {
                kw_config.inv_sc = !kw_config.inv_sc;
                eeconfig_update_kb(kw_config.raw);
                oled_interrupt(keycode);
            }
            return false;

        // ---- Mode cycling & toggles ----
        case L_CHMOD:
            if (record->event.pressed) { cycle_mode_l(); oled_interrupt(keycode); }
            return false;
        case R_CHMOD:
            if (record->event.pressed) { cycle_mode_r(); oled_interrupt(keycode); }
            return false;
        case AUTO_MOUSE:
            if (record->event.pressed) {
                kw_config.auto_mouse = !kw_config.auto_mouse;
                set_auto_mouse_enable(kw_config.auto_mouse);
                eeconfig_update_kb(kw_config.raw);
                oled_interrupt(keycode);
            }
            return false;
        case OLED_MOD:
            if (record->event.pressed) {
                CYCLE_OLED_STATE(kw_config.oled_state_master);
                CYCLE_OLED_STATE(kw_config.oled_state_slave);
                oled_clear();
                eeconfig_update_kb(kw_config.raw);
                oled_interrupt(keycode);
            }
            return false;
        case QK_USER_26:
            if (record->event.pressed) {
                CYCLE_OLED_STATE(kw_config.oled_state_slave);
                oled_clear();
                eeconfig_update_kb(kw_config.raw);
                oled_interrupt(keycode);
            }
            return false;

        // ---- Temporary scroll/slow/cursor/key modes ----
        case MOD_SCRL:
            is_scroll_mode(record->event.pressed);
            oled_tempch(record->event.pressed, keycode);
            return false;

        // Scroll + tap-fallback
        case QK_USER_0: handle_mode_tap_fallback(record, keycode, KC_ESC,  is_scroll_mode); return false;
        case QK_USER_1: handle_mode_tap_fallback(record, keycode, KC_TAB,  is_scroll_mode); return false;
        case QK_USER_2: handle_mode_tap_fallback(record, keycode, KC_LNG1, is_scroll_mode); return false;
        case QK_USER_3: handle_mode_tap_fallback(record, keycode, KC_LNG2, is_scroll_mode); return false;

        case QK_USER_4:
            is_slow_mode(record->event.pressed);
            oled_tempch(record->event.pressed, keycode);
            return false;

        // Slow + tap-fallback
        case QK_USER_5: handle_mode_tap_fallback(record, keycode, KC_ESC,  is_slow_mode); return false;
        case QK_USER_6: handle_mode_tap_fallback(record, keycode, KC_TAB,  is_slow_mode); return false;
        case QK_USER_7: handle_mode_tap_fallback(record, keycode, KC_LNG1, is_slow_mode); return false;
        case QK_USER_8: handle_mode_tap_fallback(record, keycode, KC_LNG2, is_slow_mode); return false;

        // Pointer buttons + slow
        case QK_USER_9:  handle_btn_slow(record, keycode, KC_BTN1); return false;
        case QK_USER_10:
            handle_btn_slow(record, keycode, KC_BTN2);
            if (record->event.pressed) oled_interrupt(QK_USER_4);
            return false;
        case QK_USER_11: handle_btn_slow(record, keycode, KC_BTN3); return false;

        case QK_USER_12:
            is_cursor_mode(record->event.pressed);
            oled_tempch(record->event.pressed, keycode);
            return false;
        case QK_USER_13:
            is_key_mode(record->event.pressed);
            oled_tempch(record->event.pressed, keycode);
            return false;

        case QK_USER_14:
            if (record->event.pressed) { toggle_dpad_exclusion(); oled_interrupt(keycode); }
            return false;
        case QK_USER_15:
            if (record->event.pressed) {
                kw_config.rgb_layers = !kw_config.rgb_layers;
                eeconfig_update_kb(kw_config.raw);
                oled_interrupt(keycode);
            }
            return false;
        case QK_USER_16:
            if (record->event.pressed) { reset_joystick(); oled_interrupt(keycode); }
            return false;

        // ---- Gamepad d-pad ----
        case QK_USER_17: handle_joy_axis(record, 1, -511); return false;
        case QK_USER_18: handle_joy_axis(record, 1,  511); return false;
        case QK_USER_19: handle_joy_axis(record, 0, -511); return false;
        case QK_USER_20: handle_joy_axis(record, 0,  511); return false;

        case QK_USER_21:
            if (get_joystick_attached()) {
                is_game_mode(record->event.pressed);
                oled_tempch(record->event.pressed, keycode);
            }
            return false;

        // ---- Joystick offset tuning ----
        case QK_USER_22:
            if (get_joystick_offset_min() > 4)   set_joystick_offset_min(get_joystick_offset_min() - 5);
            oled_interrupt(keycode);
            return false;
        case QK_USER_23:
            if (get_joystick_offset_min() < 196) set_joystick_offset_min(get_joystick_offset_min() + 5);
            oled_interrupt(keycode);
            return false;
        case QK_USER_24:
            if (get_joystick_offset_max() > 4)   set_joystick_offset_max(get_joystick_offset_max() - 5);
            oled_interrupt(keycode);
            return false;
        case QK_USER_25:
            if (get_joystick_offset_max() < 196) set_joystick_offset_max(get_joystick_offset_max() + 5);
            oled_interrupt(keycode);
            return false;
    }

    if (record->event.pressed) {
        oled_interrupt(keycode);
    }
    return true;
}
