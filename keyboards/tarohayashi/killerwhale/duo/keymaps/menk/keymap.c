// Copyright 2021 Hayashi (@w_vwbw)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "lib/custom_keycodes.h"
#include "g7_sync.h"
#include "transactions.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "timer.h"
#include "oled_driver.h" // adjust if your project uses a different oled header

// Caps Word slave-side mirror; updated via RPC from master each time the
// master's caps word state changes. The slave OLED reads this to draw a flag.
bool kw_caps_word_on = false;

// VIA custom value command hook - receives G7 glucose data from host over raw HID.
// Host packet format: [0x07 (id_custom_set_value), G7_VIA_CHANNEL (0xA0), sub_cmd, payload...]
void via_custom_value_command_kb(uint8_t *data, uint8_t length) {
    if (length >= 3 && data[1] == G7_VIA_CHANNEL) {
        g7_process_hid(&data[2], length - 2);
    }
}

#ifdef CAPS_WORD_ENABLE
// Mirror dilemma 4x6_4 caps_word behavior: continue caps word for letters and
// punctuation that's part of a typical identifier, plus digits / dashes /
// underscores / dots / commas / backspace.
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

// Slave-side receiver: store the master's caps word state for OLED display.
static void user_sync_caps_word_recv(uint8_t in_buflen, const void *in_data,
                                     uint8_t out_buflen, void *out_data) {
    if (in_buflen == sizeof(kw_caps_word_on)) {
        memcpy(&kw_caps_word_on, in_data, in_buflen);
    }
}

void keyboard_post_init_user(void) {
    transaction_register_rpc(USER_SYNC_CAPS_WORD, user_sync_caps_word_recv);
}

#ifdef CAPS_WORD_ENABLE
// Push caps word state to the slave when it changes. Called from
// housekeeping_task_kb (in custom_oled.c) on the master only.
void kw_caps_word_sync_task(void) {
    static bool last_sent = false;
    bool        cur       = is_caps_word_on();
    if (cur != last_sent) {
        if (transaction_rpc_send(USER_SYNC_CAPS_WORD, sizeof(cur), &cur)) {
            last_sent       = cur;
            kw_caps_word_on = cur;
        }
    }
}
#endif

// clang-format off
// レイヤー名
enum layer_number {
    BASE = 0,
    ONOFF, OFFON, ONON,                       // トグルスイッチで変更するレイヤー
    LOWER, UPPER, UTIL,                       // 長押しで変更するレイヤー
    MOUSE, BALL_SETTINGS, LIGHT_SETTINGS // 自動マウスレイヤー切り替えや設定用のレイヤー
};

// キーマップの設定
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [BASE] = LAYOUT(
        // 左手
        // 天面スイッチ
        KC_ESC,  KC_1,         KC_2,            KC_3, LT(BALL_SETTINGS, KC_4), LT(LIGHT_SETTINGS, KC_5),
        KC_TAB,  KC_Q,         KC_W,            KC_E, KC_R, KC_T,
        CMD_CTL, KC_A,         LT(MOUSE, KC_S), KC_D, KC_F, KC_G,
                 LSFT_T(KC_Z), KC_X,            KC_C, KC_V, KC_B,
                               MOD_SCRL,
        // 側面スイッチ
        KC_LNG2, KC_SPC,
        // 十字キーorジョイスティック                // ジョイスティックスイッチ
        KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT,         L_CHMOD,
        // 追加スイッチ                             // トグルスイッチ
        MS_BTN2, MS_BTN1,                    MO(ONOFF),
        // 右手
        LT(LIGHT_SETTINGS, KC_6), LT(BALL_SETTINGS, KC_7), KC_8, KC_9, KC_0, KC_BSPC,
        KC_Y, KC_U, KC_I,    KC_O,             KC_P,    KC_ENT,
        KC_H, KC_J, KC_K,    LT(MOUSE, KC_L),  KC_SCLN, KC_RSFT,
        KC_N, KC_M, KC_COMM, KC_DOT,           KC_SLSH,
                             MOD_SCRL,
        KC_SPACE, KC_LNG1,
        KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT,         R_CHMOD,
        MS_BTN1, MS_BTN2,                    MO(OFFON)
    ),
    [ONOFF] = LAYOUT(
        // 左手
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,
                 _______, _______, _______, _______, _______,
                          _______,
        _______, _______,
        _______, _______, _______, _______,          _______,
        _______, _______,                            _______,
        // 右手
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,
                                   _______,
        _______, _______,
        _______, _______, _______, _______,          _______,
        _______, _______,                            _______
    ),
    [OFFON] = LAYOUT(
        // 左手
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,
                 _______, _______, _______, _______, _______,
                          _______,
        _______, _______,
        _______, _______, _______, _______,          _______,
        _______, _______,                            _______,
        // 右手
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,
                                   _______,
        _______, _______,
        _______, _______, _______, _______,          _______,
        _______, _______,                            _______
    ),
    [ONON] = LAYOUT(
        // 左手
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,
                 _______, _______, _______, _______, _______,
                          _______,
        _______, _______,
        _______, _______, _______, _______,          _______,
        _______, _______,                            _______,
        // 右手
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,
                                   _______,
        _______, _______,
        _______, _______, _______, _______,          _______,
        _______, _______,                            _______
    ),
    [MOUSE] = LAYOUT(
        // 左手
        _______, _______, _______, _______, _______,    _______,
        _______, _______, _______, _______, _______,    _______,
        _______, _______, _______, MS_BTN2, MS_BTN1, MOD_SCRL,
                 QK_USER_4, _______, _______, _______, _______,
                          MOD_SCRL,
        _______, _______,
        _______, _______, _______, _______,          _______,
        _______, _______,                            _______,
        // 右手
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,
        MOD_SCRL, MS_BTN1, MS_BTN2, _______, _______, _______,
        _______, _______, _______, _______, QK_USER_4,
                                   MOD_SCRL,
        _______, _______,
        _______, _______, _______, _______,          _______,
        _______, _______,                            _______
    ),
    [BALL_SETTINGS] = LAYOUT(
        // 左手
        XXXXXXX,    XXXXXXX, XXXXXXX, QK_USER_14, _______, L_CHMOD,
        XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, L_SPD_I, XXXXXXX,
        AUTO_MOUSE, XXXXXXX, XXXXXXX, L_ANG_D, L_INV,   L_ANG_I,
                    XXXXXXX, XXXXXXX,XXXXXXX, L_SPD_D, XXXXXXX,
                             INV_SCRL,
        XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,
        XXXXXXX, INV_SCRL,                           XXXXXXX,
        // 右手
        R_CHMOD, _______, QK_USER_14, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, R_SPD_I,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        R_ANG_D, R_INV,   R_ANG_I, XXXXXXX, XXXXXXX, AUTO_MOUSE,
        XXXXXXX, R_SPD_D,   XXXXXXX, XXXXXXX, XXXXXXX,
                                     INV_SCRL,
        XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,
        XXXXXXX, XXXXXXX,                            XXXXXXX
    ),
    [LIGHT_SETTINGS] = LAYOUT(
        // 左手
        XXXXXXX, XXXXXXX, XXXXXXX, UG_NEXT, UG_PREV, _______,
        XXXXXXX, UG_SPDU, UG_VALU, UG_SATU, UG_HUEU, UG_TOGG,
        OLED_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                 UG_SPDD, UG_VALD, UG_SATD, UG_HUED, XXXXXXX,
                          QK_USER_15,
        UG_NEXT, UG_PREV,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,
        XXXXXXX, XXXXXXX,                            XXXXXXX,
        // 右手
        _______, UG_NEXT, UG_PREV, XXXXXXX, XXXXXXX, XXXXXXX,
        UG_TOGG, UG_HUEU, UG_SATU, UG_VALU, UG_SPDU, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, OLED_MOD,
        XXXXXXX, UG_HUED, UG_SATD, UG_VALD, UG_SPDD,
                                   QK_USER_15,
        UG_PREV, UG_NEXT,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,
        XXXXXXX, XXXXXXX,                            XXXXXXX
    )
};

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [BASE] =   {
        ENCODER_CCW_CW(KC_ESC, KC_TAB),
        ENCODER_CCW_CW(REDO, UNDO),
        ENCODER_CCW_CW(MS_WHLU, MS_WHLD),
        ENCODER_CCW_CW(MS_WHLU, MS_WHLD),
        ENCODER_CCW_CW(KC_DEL, KC_BSPC),
        ENCODER_CCW_CW(KC_UP, KC_DOWN),
        ENCODER_CCW_CW(MS_WHLU, MS_WHLD),
        ENCODER_CCW_CW(MS_WHLU, MS_WHLD)
    },
    [LIGHT_SETTINGS] =   {
        ENCODER_CCW_CW(UG_SPDU, UG_SPDD),
        ENCODER_CCW_CW(UG_VALU, UG_VALD),
        ENCODER_CCW_CW(UG_SATU, UG_SATD),
        ENCODER_CCW_CW(UG_HUEU, UG_HUED),
        ENCODER_CCW_CW(UG_SPDU, UG_SPDD),
        ENCODER_CCW_CW(UG_VALU, UG_VALD),
        ENCODER_CCW_CW(UG_SATU, UG_SATD),
        ENCODER_CCW_CW(UG_HUEU, UG_HUED)
    },
};
// clang-format on
