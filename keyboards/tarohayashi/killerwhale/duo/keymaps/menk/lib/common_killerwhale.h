// Copyright 2021 Hayashi (@w_vwbw)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once
#include "quantum.h"
#include "config.h"

// ジョイスティックの状態
enum jsmode { NO_JOYSTICK = 0, JOYSTICK_LEFT, JOYSTICK_RIGHT };

// WORD_NAV target context. Cycled by CTX_CYCLE; selects which keystrokes the
// trackball gesture emits so the same physical motion does the right thing in
// a desktop text field, a terminal (readline), or neovim normal mode.
typedef enum { WN_CTX_GUI = 0, WN_CTX_TERM, WN_CTX_NVIM, WN_CTX_COUNT } wn_ctx_t;

enum {
    STATE_OFF     = 0,
    STATE_LAYER   = 1,
    STATE_INFO    = 2,
    STATE_ANIMONE = 3,
    STATE_ANIMTWO = 4,
    STATE_G7      = 5,
};

// 保存される設定の定義
typedef union {
    uint32_t raw;
    struct {
        uint8_t spd_l : 3;     // ポインター速度 最大値7
        uint8_t angle_l : 5;   // ポインター角度 最大値29
        uint8_t pd_mode_l : 2; // カーソル移動/スクロールモード/キー入力モード
        bool    inv_l : 1;     // トラックボール / ジョイスティック左右反転ON/OFF

        uint8_t spd_r : 3;     // ポインター速度 最大値7
        uint8_t angle_r : 5;   // ポインター角度 最大値59
        uint8_t pd_mode_r : 2; // カーソル移動/スクロールモード/キー入力モード
        bool    inv_r : 1;     // トラックボール / ジョイスティック左右反転

        bool    inv_sc : 1;            // スクロールの反転ON/OFF
        bool    auto_mouse : 1;        // オートマウスON/OFF
        bool    rgb_layers : 1;        // RGBレイヤーON/OFF
        uint8_t oled_state_master : 4; // oled表示
        uint8_t oled_state_slave : 4;  // oled表示
    };
} kw_config_t;

// 外部参照用
extern kw_config_t kw_config;

// インターフェース
void     reset_joystick(void);        // ジョイスティックを初期化
uint8_t  get_joystick_attached(void); // ジョイスティックの有無
uint16_t get_joystick_offset_min(void);
uint16_t get_joystick_offset_max(void);
void     set_joystick_offset_min(uint16_t min);
void     set_joystick_offset_max(uint16_t max);
void     cycle_mode_l(void); // モード変更
void     cycle_mode_r(void);
void     is_scroll_mode(bool is_force_scrolling); // 一時的モード変更
void     is_cursor_mode(bool is_force_scrolling);
void     is_key_mode(bool is_force_key_input);
void     is_game_mode(bool is_force_gaming);
void     is_slow_mode(bool is_slow_mode);
void     is_word_nav_mode(bool is_force_word_nav); // hold = trackball does word-level motion
void     is_char_nav_mode(bool is_force_char_nav); // hold = trackball does bare-arrow motion

void     wn_ctx_cycle(void); // CTX_CYCLE: advance GUI -> TERM -> NVIM -> GUI
wn_ctx_t wn_ctx_get(void);   // current word-nav target context

bool get_dpad_exclusion(void);
void toggle_dpad_exclusion(void);
