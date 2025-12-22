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

#define DEBUG_MATRIX_SCAN_RATE
#define SERIAL_DEBUG
#define QUANTUM_PAINTER_DEBUG
#define QUANTUM_PAINTER_DEBUG_ENABLE_FLUSH_TASK_OUTPUT

#define DYNAMIC_KEYMAP_LAYER_COUNT 8
#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_LED_STATE_ENABLE

#define ENCODER_RESOLUTION 4

#define SPI_SCK_PIN GP22
#define SPI_MOSI_PIN GP23
#define SPI_MISO_PIN GP18 // Unused
#define LCD_RST_PIN GP13
#define LCD_DC_PIN GP11
#define LCD_CS_PIN GP12
#define BACKLIGHT_PIN GP18
#define BACKLIGHT_PWM_DRIVER PWMD1
#define BACKLIGHT_PWM_CHANNEL RP2040_PWM_CHANNEL_A
#define BACKLIGHT_LEVELS 12
#define BACKLIGHT_DEFAULT_ON true
#define BACKLIGHT_DEFAULT_LEVEL 7

#define SPI_DRIVER SPID0
#define LCD_SPI_DIVISOR 4
#define LCD_WAIT_TIME 250
#define LCD_ROTATION QP_ROTATION_0
#define LCD_OFFSET_X 0
#define LCD_OFFSET_Y 0
#define LCD_WIDTH 240 // Set according to your display specs
#define LCD_HEIGHT 320 // Set according to your display specs
#define SPI_MODE 0 // Set according to your display specs
// Timeout configuration, default 30000 (30 sek). 0 = No timeout. Beware of image retention.
#define QUANTUM_PAINTER_DISPLAY_TIMEOUT 30000
