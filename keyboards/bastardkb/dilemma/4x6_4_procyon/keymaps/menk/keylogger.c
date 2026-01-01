#include "keylogger.h"
#include <string.h>

// Static keylogger state
static keylogger_state_t keylogger = {0};

// Initialize keylogger
void keylogger_init(void) {
    keylogger.buffer[0] = '\0';
    keylogger.index = 0;
    keylogger.dirty = false;
}

void keylogger_set_buffer(const char* new_buffer, uint16_t new_index) {
    // Copy the new buffer
    strncpy(keylogger.buffer, new_buffer, KEYLOGGER_BUFFER_SIZE);
    keylogger.buffer[KEYLOGGER_BUFFER_SIZE] = '\0';  // Ensure null termination

    // Update index
    keylogger.index = (new_index < KEYLOGGER_BUFFER_SIZE) ? new_index : KEYLOGGER_BUFFER_SIZE;

    // Mark as dirty for redraw
    keylogger.dirty = true;
}

// Add a character to the keylogger buffer
void keylogger_add_char(char c) {
    // Skip if buffer is full, shift left
    if (keylogger.index >= KEYLOGGER_BUFFER_SIZE) {
        memmove(keylogger.buffer, keylogger.buffer + 1, KEYLOGGER_BUFFER_SIZE - 1);
        keylogger.index = KEYLOGGER_BUFFER_SIZE - 1;
    }

    // Add character
    keylogger.buffer[keylogger.index] = c;
    keylogger.index++;
    keylogger.buffer[keylogger.index] = '\0';

    keylogger.dirty = true;
}

// Clear the keylogger buffer
void keylogger_clear(void) {
    keylogger.buffer[0] = '\0';
    keylogger.index = 0;
    keylogger.dirty = true;
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
            case KC_2: return '@';  // Fixed: should be @ not "
            case KC_3: return '#';
            case KC_4: return '$';
            case KC_5: return '%';
            case KC_6: return '^';
            case KC_7: return '&';
            case KC_8: return '*';
            case KC_9: return '(';
            case KC_0: return ')';
            case KC_MINS: return '_';
            case KC_EQL: return '+';   // Fixed: KC_EQL not KC_EQUAL
            case KC_LBRC: return '{';
            case KC_RBRC: return '}';  // Fixed: KC_RBRC not KC_RBRACKET
            case KC_BSLS: return '|';
            case KC_SCLN: return ':';
            case KC_QUOT: return '"';  // Fixed: " not ~ (and KC_QUOT not KC_GRAVE)
            case KC_GRV: return '~';   // Fixed: ~ and KC_GRV not KC_GRAVE
            case KC_COMM: return '<';
            case KC_DOT: return '>';
            case KC_SLSH: return '?';
        }
    }

    // Handle letters (with caps and shift)
    if (keycode >= KC_A && keycode <= KC_Z) {
        char base_char = 'a' + (keycode - KC_A);
        if (shift ^ caps) {  // Uppercase if shift XOR caps is true
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
        case KC_BSPC: return '\b';
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

    if (c == '\b') {  // Backspace
        if (keylogger.index > 0) {
            keylogger.index--;
            keylogger.buffer[keylogger.index] = '\0';
            keylogger.dirty = true;
        }
    } else if (c == '\n' || c == '\t') {  // Newline or tab
        keylogger_add_char(' ');  // Replace with space for display
    } else if (c != 0) {  // Valid display character
        keylogger_add_char(c);
    }
}
