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
#pragma once
#define DYNAMIC_KEYMAP_LAYER_COUNT 8
#define SPLIT_TRANSPORT_MIRROR
#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_LED_STATE_ENABLE
#define SPLIT_MODS_ENABLE
#define SPLIT_WPM_ENABLE
#define SPLIT_POINTING_ENABLE
#define SPLIT_ACTIVITY_ENABLE
#define SPLIT_TRANSACTION_IDS_USER USER_SYNC_DPI, USER_SYNC_CAPS_WORD
#define RGB_MATRIX_MODE_NAME_ENABLE

#define ENCODER_RESOLUTION 4

#define SPI_SCK_PIN GP22
#define SPI_MOSI_PIN GP23
#define SPI_MISO_PIN NO_PIN // Unused
#define LCD_DC_PIN GP11
#define LCD_CS_PIN GP12
#define LCD_RST_PIN GP13
#define BACKLIGHT_PIN GP18
#define BACKLIGHT_PWM_DRIVER PWMD1
#define BACKLIGHT_PWM_CHANNEL RP2040_PWM_CHANNEL_A
#define BACKLIGHT_LEVELS 12
#define BACKLIGHT_DEFAULT_ON true
#define BACKLIGHT_DEFAULT_LEVEL 7l

#define CAPS_WORD_INVERT_ON_SHIFT
#define BOTH_SHIFTS_TURNS_ON_CAPS_WORD
#define CAPS_WORD_IDLE_TIMEOUT 0

#define RGB_MATRIX_DEFAULT_VAL 128

#define SPI_DRIVER SPID0
#define LCD_SPI_DIVISOR 4
#define LCD_WAIT_TIME 150
#define LCD_ROTATION QP_ROTATION_0
#define LCD_OFFSET_X 0
#define LCD_OFFSET_Y 0
#define LCD_WIDTH 240 // Set according to your display specs
#define LCD_HEIGHT 320 // Set according to your display specs
#define SPI_MODE 0 // Set according to your display specs
#define QUANTUM_PAINTER_SUPPORTS_NATIVE_COLORS TRUE
#define QUANTUM_PAINTER_PIXDATA_BUFFER_SIZE 240000
#define QUANTUM_PAINTER_TASK_THROTTLE 10
// Timeout configuration, default 30000 (30 sek). 0 = No timeout. Beware of image retention.
#define QUANTUM_PAINTER_DISPLAY_TIMEOUT 120000
