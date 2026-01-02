#include "keylogger.h"
#include <string.h>
#include <ctype.h>

// Static keylogger state
static keylogger_state_t keylogger = {0};
static uint8_t line_lengths[MAX_TERMINAL_LINES] = {0};

// Initialize keylogger
void keylogger_init(void) {
    keylogger.buffer[0] = '\0';
    keylogger.index = 0;
    keylogger.dirty = false;
    keylogger.current_line = 0;

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


// Add a character to the keylogger buffer
void keylogger_add_char(char c) {
    // Get current line index and length
    uint8_t current_line = keylogger.current_line;
    uint8_t line_len = line_lengths[current_line];

    // Handle newline
    if (c == '\n') {
        // Add newline to continuous buffer
        if (keylogger.index >= KEYLOGGER_BUFFER_SIZE) {
            memmove(keylogger.buffer, keylogger.buffer + 1, KEYLOGGER_BUFFER_SIZE - 1);
            keylogger.index = KEYLOGGER_BUFFER_SIZE - 1;
        }

        keylogger.buffer[keylogger.index] = c;
        keylogger.index++;
        keylogger.buffer[keylogger.index] = '\0';

        // Move to next line
        if (current_line < MAX_TERMINAL_LINES - 1) {
            keylogger.current_line++;
            line_lengths[keylogger.current_line] = 0;
            keylogger.lines[keylogger.current_line][0] = '\0';
        } else {
            // Shift lines up if at bottom
            for (int i = 0; i < MAX_TERMINAL_LINES - 1; i++) {
                strcpy(keylogger.lines[i], keylogger.lines[i + 1]);
                line_lengths[i] = line_lengths[i + 1];
            }
            keylogger.lines[MAX_TERMINAL_LINES - 1][0] = '\0';
            line_lengths[MAX_TERMINAL_LINES - 1] = 0;
        }

        keylogger.dirty = true;
        return;
    }

    // Handle regular characters
    // Check if line is full
    if (line_len >= TERMINAL_WIDTH) {
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

    // Add character to current line
    keylogger.lines[current_line][line_len] = c;
    keylogger.lines[current_line][line_len + 1] = '\0';
    line_lengths[current_line] = line_len + 1;

    // Update continuous buffer
    if (keylogger.index >= KEYLOGGER_BUFFER_SIZE) {
        memmove(keylogger.buffer, keylogger.buffer + 1, KEYLOGGER_BUFFER_SIZE - 1);
        keylogger.index = KEYLOGGER_BUFFER_SIZE - 1;
    }

    keylogger.buffer[keylogger.index] = c;
    keylogger.index++;
    keylogger.buffer[keylogger.index] = '\0';

    keylogger.dirty = true;
}

static void keylogger_backspace(void) {
    uint8_t current_line = keylogger.current_line;
    uint8_t line_len = line_lengths[current_line];

    if (line_len > 0) {
        // Remove last character from current line
        keylogger.lines[current_line][line_len - 1] = '\0';
        line_lengths[current_line] = line_len - 1;
    } else if (current_line > 0) {
        // If current line is empty, go back to previous line
        keylogger.current_line = current_line - 1;
        // Don't remove the newline from buffer, just move cursor
    }

    // Update continuous buffer
    if (keylogger.index > 0) {
        keylogger.index--;
        keylogger.buffer[keylogger.index] = '\0';
    }

    keylogger.dirty = true;
}

// Clear the keylogger buffer
void keylogger_clear(void) {
    keylogger.buffer[0] = '\0';
    keylogger.index = 0;
    keylogger.current_line = 0;

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
            case KC_2: return '@';
            case KC_3: return '#';
            case KC_4: return '$';
            case KC_5: return '%';
            case KC_6: return '^';
            case KC_7: return '&';
            case KC_8: return '*';
            case KC_9: return '(';
            case KC_0: return ')';
            case KC_MINS: return '_';
            case KC_EQL: return '+';
            case KC_LBRC: return '{';
            case KC_RBRC: return '}';
            case KC_BSLS: return '|';
            case KC_SCLN: return ':';
            case KC_QUOT: return '"';
            case KC_GRV: return '~';
            case KC_COMM: return '<';
            case KC_DOT: return '>';
            case KC_SLSH: return '?';
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
        case KC_LBRC: return '[';
        case KC_RBRC: return ']';
        case KC_BSLS: return '\\';
        case KC_SCLN: return ';';
        case KC_QUOT: return '\'';
        case KC_GRV: return '`';
        case KC_COMM: return ',';
        case KC_DOT: return '.';
        case KC_SLSH: return '/';
        case KC_ENT: return '\n';
        case KC_TAB: return '\t';
        default: return 0;
    }
}

// Process keypress for keylogger
void keylogger_process_key(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) return;

    // Get modifier states
    bool shift = keyboard_report->mods & (MOD_BIT(KC_LSFT) | MOD_BIT(KC_RSFT));
    bool caps = host_keyboard_led_state().caps_lock;

    // Convert keycode to character
    char c = keycode_to_char(keycode, shift, caps);

    if (keycode == KC_BSPC) {  // Backspace
        keylogger_backspace();
    } else if (c == '\n') {  // Newline (Enter key)
        keylogger_add_char(c);
    } else if (c == '\t') {  // Tab
        // Add 4 spaces for tab
        for (int i = 0; i < 4; i++) {
            keylogger_add_char(' ');
        }
    } else if (c != 0) {  // Valid display character
        keylogger_add_char(c);
    }
}
