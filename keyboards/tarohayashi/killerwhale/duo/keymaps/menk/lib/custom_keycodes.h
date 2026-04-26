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
// WORD_NAV: hold to make the trackball drive context-aware word motion.
// Horizontal stroke = word-jump back/forward, vertical stroke = line up/down.
// The actual keystrokes depend on the active context (cycled by CTX_CYCLE):
//   GUI  -> Ctrl/Opt + Left/Right + Up/Down
//   TERM -> Alt + B / Alt + F + Up/Down       (readline word-jump)
//   NVIM -> b / w + k / j                     (normal-mode motions)
// Hold physical Shift during the gesture to get selection variants.
// CHAR_NAV: hold to make the trackball drive bare arrow keys (per-character
// nav). Lower threshold than word mode so small strokes register.
// CTX_CYCLE: tap to advance WORD_NAV's context GUI -> TERM -> NVIM -> GUI.
// SELWFWD / SELWBWD: tap aliases for the getreuer SELECT_WORD /
// SELECT_WORD_BACK community keycodes (kept Vial-bindable).
#define WORD_NAV  QK_USER_27
#define CHAR_NAV  QK_USER_28
#define CTX_CYCLE QK_USER_29
#define SELWFWD   QK_USER_30
#define SELWBWD   QK_USER_31
