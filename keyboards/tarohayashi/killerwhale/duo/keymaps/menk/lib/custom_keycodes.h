// Copyright 2021 Hayashi (@w_vwbw)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once
#include "quantum.h"

// 追加するキーコード
enum TH_keycodes { CMD_CTL = QK_KB_0, UNDO, REDO, COPY, CUT, PASTE, SC_UP, SC_DOWN, SC_RESET, CAPTCHA, SAVE, SAVEAS, NEXTTAB, PREVTAB, CLOSETAB, RSTRTAB, L_SPD_I, L_SPD_D, R_SPD_I, R_SPD_D, L_ANG_I, L_ANG_D, L_INV, L_CHMOD, R_ANG_I, R_ANG_D, R_INV, R_CHMOD, INV_SCRL, MOD_SCRL, AUTO_MOUSE, OLED_MOD };

// QK_USER_x aliases. The Vial-style configurator only enumerates QK_USER_0..31,
// so anything we want to bind from that app must live in this range. The
// existing keymap already uses QK_USER_0..26, leaving 27..31 free.
//
// WORD_NAV / WORD_SEL: hold-to-engage modes that turn the trackball into a
// horizontal word-jump (Ctrl/Opt + Left/Right) or word-selection driver
// (getreuer select_word).
// CWTOGG / SELWFWD / SELWBWD: tap aliases for QMK's CW_TOGG and the getreuer
// SELECT_WORD / SELECT_WORD_BACK community keycodes.
#define WORD_NAV QK_USER_27
#define WORD_SEL QK_USER_28
#define CWTOGG   QK_USER_29
#define SELWFWD  QK_USER_30
#define SELWBWD  QK_USER_31
