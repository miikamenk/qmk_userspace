#include "keylogger.h"
#include <string.h>
#include <ctype.h>

// Static keylogger state
static keylogger_state_t keylogger = {0};
static uint8_t line_lengths[MAX_TERMINAL_LINES] = {0};
static uint16_t last_keycode = 0;
static uint8_t key_repeat_count = 0;
static uint32_t last_key_time = 0;
static bool ctrl_pressed = false;
static bool win_pressed = false;
static bool alt_pressed = false;
static bool repeat_displayed = false;

// Initialize keylogger
void keylogger_init(void) {
    keylogger.buffer[0] = '\0';
    keylogger.index = 0;
    keylogger.dirty = false;
    keylogger.current_line = 0;

    last_keycode = 0;
    key_repeat_count = 0;
    last_key_time = 0;
    ctrl_pressed = false;
    win_pressed = false;
    alt_pressed = false;
    repeat_displayed = false;

    // Initialize all lines to empty
    for (int i = 0; i < MAX_TERMINAL_LINES; i++) {
        keylogger.lines[i][0] = '\0';
        line_lengths[i] = 0;
    }
}

void keylogger_set_buffer(const char* new_buffer, uint16_t new_index) {
    // Reset all lines
    for (int i = 0; i < MAX_TERMINAL_LINES; i++) {
        keylogger.lines[i][0] = '\0';
    }
    keylogger.current_line = 0;

    // Parse the new buffer into lines
    int line_index = 0;
    int char_in_line = 0;
    int buffer_len = strlen(new_buffer);

    for (int i = 0; i < buffer_len && line_index < MAX_TERMINAL_LINES; i++) {
        char c = new_buffer[i];

        if (c == '\n') {
            // End current line
            keylogger.lines[line_index][char_in_line] = '\0';
            line_index++;
            char_in_line = 0;

            // Don't exceed max lines
            if (line_index >= MAX_TERMINAL_LINES) {
                line_index = MAX_TERMINAL_LINES - 1;
                char_in_line = 0;
            }
        } else if (char_in_line < TERMINAL_WIDTH) {
            // Add character to current line
            keylogger.lines[line_index][char_in_line] = c;
            char_in_line++;
        } else {
            // Line is full, move to next line
            keylogger.lines[line_index][TERMINAL_WIDTH] = '\0';
            line_index++;
            char_in_line = 0;

            if (line_index < MAX_TERMINAL_LINES) {
                keylogger.lines[line_index][0] = c;
                keylogger.lines[line_index][1] = '\0';
                char_in_line = 1;
            }
        }
    }

    // Terminate last line if needed
    if (line_index < MAX_TERMINAL_LINES) {
        keylogger.lines[line_index][char_in_line] = '\0';
    }

    // Copy buffer
    strncpy(keylogger.buffer, new_buffer, KEYLOGGER_BUFFER_SIZE);
    keylogger.buffer[KEYLOGGER_BUFFER_SIZE] = '\0';
    keylogger.index = (new_index < KEYLOGGER_BUFFER_SIZE) ? new_index : KEYLOGGER_BUFFER_SIZE;

    // Set current line to the last non-empty line
    keylogger.current_line = line_index;
    keylogger.dirty = true;
}

// Get a specific line
void keylogger_get_line(uint8_t line_num, char *buffer) {
    if (line_num < MAX_TERMINAL_LINES) {
        strncpy(buffer, keylogger.lines[line_num], MAX_LINE_LENGTH);
        buffer[MAX_LINE_LENGTH] = '\0';
    } else {
        buffer[0] = '\0';
    }
}

// Set a specific line
void keylogger_set_line(uint8_t line_num, const char *line) {
    if (line_num < MAX_TERMINAL_LINES) {
        strncpy(keylogger.lines[line_num], line, MAX_LINE_LENGTH);
        keylogger.lines[line_num][MAX_LINE_LENGTH] = '\0';
        keylogger.dirty = true;
    }
}

// Get current line being edited
uint8_t keylogger_get_current_line(void) {
    return keylogger.current_line;
}


// Add a character to the keylogger buffer with modifier icons
void keylogger_add_char_with_modifiers(char c, bool ctrl, bool win, bool alt) {
    // Get current line index and length
    uint8_t current_line = keylogger.current_line;
    uint8_t line_len = line_lengths[current_line];

    // Calculate space needed for modifier icons
    int modifier_chars = 0;
    if (ctrl) modifier_chars++;
    if (win) modifier_chars++;
    if (alt) modifier_chars++;

    // Check if we have enough space for modifiers + character
    if (line_len + modifier_chars + 1 >= TERMINAL_WIDTH) {
        // Auto-advance to next line
        if (current_line < MAX_TERMINAL_LINES - 1) {
            keylogger.current_line = current_line + 1;
            current_line = keylogger.current_line;
            line_len = 0;
        } else {
            // Shift all lines up
            for (int i = 0; i < MAX_TERMINAL_LINES - 1; i++) {
                strcpy(keylogger.lines[i], keylogger.lines[i + 1]);
                line_lengths[i] = line_lengths[i + 1];
            }
            keylogger.lines[MAX_TERMINAL_LINES - 1][0] = '\0';
            line_lengths[MAX_TERMINAL_LINES - 1] = 0;
            current_line = MAX_TERMINAL_LINES - 1;
            line_len = 0;
        }
    }

    // Add modifier icons before the character
    int pos = line_len;
    if (ctrl) {
        keylogger.lines[current_line][pos] = '^'; // Ctrl symbol
        pos++;
    }
    if (win) {
        keylogger.lines[current_line][pos] = '#'; // Win symbol
        pos++;
    }
    if (alt) {
        keylogger.lines[current_line][pos] = '@'; // Alt symbol
        pos++;
    }

    // Add the actual character
    keylogger.lines[current_line][pos] = c;
    keylogger.lines[current_line][pos + 1] = '\0';
    line_lengths[current_line] = pos + 1;

    // Update continuous buffer (without modifier icons for raw log)
    if (keylogger.index >= KEYLOGGER_BUFFER_SIZE) {
        memmove(keylogger.buffer, keylogger.buffer + 1, KEYLOGGER_BUFFER_SIZE - 1);
        keylogger.index = KEYLOGGER_BUFFER_SIZE - 1;
    }

    keylogger.buffer[keylogger.index] = c;
    keylogger.index++;
    keylogger.buffer[keylogger.index] = '\0';

    keylogger.dirty = true;
}

// Original function for backward compatibility
void keylogger_add_char(char c) {
    keylogger_add_char_with_modifiers(c, false, false, false);
}

void keylogger_handle_repeat_key(uint16_t keycode, char c, bool ctrl, bool win, bool alt, uint32_t current_time) {
    // If this is the same key as last time with same modifiers
    if (keycode == last_keycode && ctrl == ctrl_pressed && win == win_pressed && alt == alt_pressed) {
        uint32_t time_diff = current_time - last_key_time;

        // If key is repeated quickly (within 500ms)
        if (time_diff < 500) {
            key_repeat_count++;

            // For first few repeats, just add characters normally
            if (key_repeat_count <= 3) {
                keylogger_add_char_with_modifiers(c, ctrl, win, alt);
                repeat_displayed = false;
            }
            // For more repeats, show compact notation
            else {
                // Remove previous compact notation if exists
                if (repeat_displayed) {
                    // Find and remove previous compact notation
                    uint8_t current_line = keylogger.current_line;
                    uint8_t line_len = line_lengths[current_line];

                    // Look for the pattern: digits followed by 'x' followed by modifiers and character
                    // We need to find where the previous compact notation starts
                    for (int i = line_len - 1; i >= 0; i--) {
                        if (keylogger.lines[current_line][i] == 'x') {
                            // Check backwards for digits
                            int digit_end = i - 1;
                            int digit_start = digit_end;
                            while (digit_start >= 0 && isdigit((unsigned char)keylogger.lines[current_line][digit_start])) {
                                digit_start--;
                            }
                            digit_start++; // Adjust to first digit

                            // Check if after 'x' we have modifiers and then our character
                            int check_pos = i + 1;
                            int modifier_count = 0;
                            while (check_pos < line_len &&
                                   (keylogger.lines[current_line][check_pos] == '^' ||
                                    keylogger.lines[current_line][check_pos] == '#' ||
                                    keylogger.lines[current_line][check_pos] == '@')) {
                                check_pos++;
                                modifier_count++;
                            }

                            // If we found a valid pattern, remove it
                            if (digit_start <= digit_end && check_pos < line_len) {
                                // Remove from digit_start to end of line
                                int chars_to_remove = line_len - digit_start;
                                for (int j = 0; j < chars_to_remove; j++) {
                                    keylogger.lines[current_line][digit_start + j] = '\0';
                                }
                                line_lengths[current_line] = digit_start;

                                // Also remove from continuous buffer
                                // Estimate how many characters to remove (digits + 'x' + modifiers + 1)
                                int estimated_removal = (digit_end - digit_start + 1) + 1 + modifier_count + 1;
                                keylogger.index -= estimated_removal;
                                if (keylogger.index < 0) keylogger.index = 0;
                                keylogger.buffer[keylogger.index] = '\0';
                                break;
                            }
                        }
                    }
                }
                // First time showing compact notation
                else {
                    // Remove last 3 individual characters
                    uint8_t current_line = keylogger.current_line;
                    uint8_t line_len = line_lengths[current_line];
                    int chars_to_remove = 3;

                    for (int i = 0; i < chars_to_remove; i++) {
                        if (line_len > 0) {
                            // Remove character and any preceding modifier icons
                            line_len--;
                            while (line_len > 0 && (keylogger.lines[current_line][line_len] == '^' ||
                                                   keylogger.lines[current_line][line_len] == '#' ||
                                                   keylogger.lines[current_line][line_len] == '@')) {
                                keylogger.lines[current_line][line_len] = '\0';
                                line_len--;
                            }
                            keylogger.lines[current_line][line_len] = '\0';
                        }
                    }
                    line_lengths[current_line] = line_len;

                    // Remove from continuous buffer too
                    keylogger.index -= 3;
                    if (keylogger.index < 0) keylogger.index = 0;
                    keylogger.buffer[keylogger.index] = '\0';
                }

                // Now add new compact notation
                char compact_str[16];
                snprintf(compact_str, sizeof(compact_str), "%dx", key_repeat_count);

                uint8_t current_line = keylogger.current_line;
                uint8_t line_len = line_lengths[current_line];

                // Add compact notation digits
                for (int i = 0; compact_str[i] != '\0'; i++) {
                    if (line_len >= TERMINAL_WIDTH) {
                        if (current_line < MAX_TERMINAL_LINES - 1) {
                            keylogger.current_line = current_line + 1;
                            current_line = keylogger.current_line;
                            line_len = 0;
                        }
                    }
                    keylogger.lines[current_line][line_len] = compact_str[i];
                    line_len++;
                }

                // Add modifier icons
                if (ctrl) {
                    if (line_len >= TERMINAL_WIDTH) {
                        if (current_line < MAX_TERMINAL_LINES - 1) {
                            keylogger.current_line = current_line + 1;
                            current_line = keylogger.current_line;
                            line_len = 0;
                        }
                    }
                    keylogger.lines[current_line][line_len] = '^';
                    line_len++;
                }
                if (win) {
                    if (line_len >= TERMINAL_WIDTH) {
                        if (current_line < MAX_TERMINAL_LINES - 1) {
                            keylogger.current_line = current_line + 1;
                            current_line = keylogger.current_line;
                            line_len = 0;
                        }
                    }
                    keylogger.lines[current_line][line_len] = '#';
                    line_len++;
                }
                if (alt) {
                    if (line_len >= TERMINAL_WIDTH) {
                        if (current_line < MAX_TERMINAL_LINES - 1) {
                            keylogger.current_line = current_line + 1;
                            current_line = keylogger.current_line;
                            line_len = 0;
                        }
                    }
                    keylogger.lines[current_line][line_len] = '@';
                    line_len++;
                }

                // Add the character
                if (line_len >= TERMINAL_WIDTH) {
                    if (current_line < MAX_TERMINAL_LINES - 1) {
                        keylogger.current_line = current_line + 1;
                        current_line = keylogger.current_line;
                        line_len = 0;
                    }
                }
                keylogger.lines[current_line][line_len] = c;
                keylogger.lines[current_line][line_len + 1] = '\0';
                line_lengths[current_line] = line_len + 1;

                // Update continuous buffer with compact notation
                for (int i = 0; compact_str[i] != '\0'; i++) {
                    if (keylogger.index >= KEYLOGGER_BUFFER_SIZE) {
                        memmove(keylogger.buffer, keylogger.buffer + 1, KEYLOGGER_BUFFER_SIZE - 1);
                        keylogger.index = KEYLOGGER_BUFFER_SIZE - 1;
                    }
                    keylogger.buffer[keylogger.index] = compact_str[i];
                    keylogger.index++;
                }

                // Add character to continuous buffer
                if (keylogger.index >= KEYLOGGER_BUFFER_SIZE) {
                    memmove(keylogger.buffer, keylogger.buffer + 1, KEYLOGGER_BUFFER_SIZE - 1);
                    keylogger.index = KEYLOGGER_BUFFER_SIZE - 1;
                }
                keylogger.buffer[keylogger.index] = c;
                keylogger.index++;
                keylogger.buffer[keylogger.index] = '\0';

                repeat_displayed = true;
            }
        } else {
            // Reset if too much time passed
            key_repeat_count = 1;
            keylogger_add_char_with_modifiers(c, ctrl, win, alt);
            repeat_displayed = false;
        }
    } else {
        // New key or different modifiers
        key_repeat_count = 1;
        keylogger_add_char_with_modifiers(c, ctrl, win, alt);
        repeat_displayed = false;
    }

    last_keycode = keycode;
    last_key_time = current_time;
    ctrl_pressed = ctrl;
    win_pressed = win;
    alt_pressed = alt;
}

// Clear the keylogger buffer
void keylogger_clear(void) {
    keylogger.buffer[0] = '\0';
    keylogger.index = 0;
    keylogger.current_line = 0;

    last_keycode = 0;
    key_repeat_count = 0;
    last_key_time = 0;
    ctrl_pressed = false;
    win_pressed = false;
    alt_pressed = false;
    repeat_displayed = false;

    for (int i = 0; i < MAX_TERMINAL_LINES; i++) {
        keylogger.lines[i][0] = '\0';
    }

    keylogger.dirty = true;
}

void keylogger_clear_buffer(void) {
    keylogger.buffer[0] = '\0';
    keylogger.index = 0;
    keylogger.dirty = false;
    keylogger.current_line = 0;

    last_keycode = 0;
    key_repeat_count = 0;
    last_key_time = 0;
    ctrl_pressed = false;
    win_pressed = false;
    alt_pressed = false;
    repeat_displayed = false;

    // Clear all lines
    for (int i = 0; i < MAX_TERMINAL_LINES; i++) {
        keylogger.lines[i][0] = '\0';
        line_lengths[i] = 0;
    }

    // Also clear the last_sent_lines on master if we're the master
    if (is_keyboard_master()) {
        // This will be handled by the master-side code
    }
}

// Get the current keylogger string
const char *keylogger_get_str(void) {
    return keylogger.buffer;
}

// Check if keylogger needs redraw
bool keylogger_is_dirty(void) {
    return keylogger.dirty;
}

// Mark keylogger as clean/dirty
void keylogger_set_dirty(bool dirty) {
    keylogger.dirty = dirty;
}

// Simple keycode to character conversion (basic US layout)
char keycode_to_char(uint16_t keycode, bool shift, bool caps) {
    // Handle numbers and symbols with shift
    if (shift) {
        switch (keycode) {
            case KC_1: return '!';
            case KC_2: return '"';
            case KC_3: return '#';
            case KC_4: return '*';
            case KC_5: return '%';
            case KC_6: return '&';
            case KC_7: return '/';
            case KC_8: return '(';
            case KC_9: return ')';
            case KC_0: return '=';
            case KC_SLSH: return '_';
            case KC_MINS: return '?';
            case KC_COMM: return ';';
            case KC_DOT: return ':';
        }
    }

    // Handle letters (with caps and shift)
    if (keycode >= KC_A && keycode <= KC_Z) {
        char base_char = 'a' + (keycode - KC_A);
        if (shift ^ caps) {
            return base_char - 32;
        }
        return base_char;
    }

    // Handle numbers without shift
    if (keycode >= KC_1 && keycode <= KC_0) {
        if (keycode == KC_0) return '0';
        return '1' + (keycode - KC_1);
    }

    // Handle other characters
    switch (keycode) {
        case KC_SPC: return ' ';
        case KC_MINS: return '-';
        case KC_EQL: return '=';
        case KC_BSLS: return '\'';
        case KC_SCLN: return 'o';
        case KC_QUOT: return 'a';
        case KC_COMM: return ',';
        case KC_DOT: return '.';
        case KC_SLSH: return '-';
        case KC_ENT: return '\n';
        case KC_TAB: return '\t';
        default: return 0;
    }
}

// Process keypress for keylogger
void keylogger_process_key(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        // Handle key release for modifier keys
        switch (keycode) {
            case KC_LCTL:
            case KC_RCTL:
                ctrl_pressed = false;
                break;
            case KC_LGUI:
            case KC_RGUI:
                win_pressed = false;
                break;
            case KC_LALT:
            case KC_RALT:
                alt_pressed = false;
                break;
        }
        return;
    }

    // Get current time for repeat tracking
    uint32_t current_time = timer_read32();

    // Get modifier states
    bool shift = keyboard_report->mods & (MOD_BIT(KC_LSFT) | MOD_BIT(KC_RSFT));
    bool caps = host_keyboard_led_state().caps_lock;

    // Update modifier key states
    switch (keycode) {
        case KC_LCTL:
        case KC_RCTL:
            ctrl_pressed = true;
            return; // Don't add to buffer
        case KC_LGUI:
        case KC_RGUI:
            win_pressed = true;
            return; // Don't add to buffer
        case KC_LALT:
        case KC_RALT:
            alt_pressed = true;
            return; // Don't add to buffer
    }

    // Convert keycode to character
    char c = keycode_to_char(keycode, shift, caps);

    if (keycode == KC_BSPC) {  // Backspace
        keylogger_add_char_with_modifiers('<', ctrl_pressed, win_pressed, alt_pressed);
    } else if (c == '\n') {  // Newline (Enter key)
        keylogger_add_char_with_modifiers(c, ctrl_pressed, win_pressed, alt_pressed);
        // Reset repeat tracking on newline
        key_repeat_count = 0;
        last_keycode = 0;
        repeat_displayed = false;
    } else if (c == '\t') {  // Tab
        // Add 4 spaces for tab with modifiers
        for (int i = 0; i < 4; i++) {
            keylogger_add_char_with_modifiers(' ', ctrl_pressed, win_pressed, alt_pressed);
        }
        // Reset repeat tracking on tab
        key_repeat_count = 0;
        last_keycode = 0;
        repeat_displayed = false;
    } else if (c != 0) {  // Valid display character
        keylogger_handle_repeat_key(keycode, c, ctrl_pressed, win_pressed, alt_pressed, current_time);
    }
}

void keylogger_get_modifier_states(bool *ctrl, bool *win, bool *alt) {
    if (ctrl) *ctrl = ctrl_pressed;
    if (win) *win = win_pressed;
    if (alt) *alt = alt_pressed;
}
