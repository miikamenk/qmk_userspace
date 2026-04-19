// Copyright 2021 Hayashi (@w_vwbw)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "custom_oled.h"
#include "os_detection.h"
#include "common_killerwhale.h"
#include "glcdfont.c"
#include "custom_keycodes.h"
#include "moominhouse.h"
#include "transactions.h"
#include <string.h>

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
