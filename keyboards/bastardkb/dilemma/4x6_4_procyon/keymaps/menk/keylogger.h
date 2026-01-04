#pragma once

#include "quantum.h"
#include "action.h"

// Keylogger configuration
#define KEYLOGGER_BUFFER_SIZE 645
#define TERMINAL_WIDTH 38
#define MAX_TERMINAL_LINES 14
#define MAX_LINE_LENGTH TERMINAL_WIDTH

// Keylogger buffer structure
typedef struct {
    char buffer[KEYLOGGER_BUFFER_SIZE + 1];
    uint16_t index;
    bool dirty;
    uint8_t current_line;  // Current line being edited
    char lines[MAX_TERMINAL_LINES][MAX_LINE_LENGTH + 1];  // Line buffer
} keylogger_state_t;

// Function declarations
void keylogger_init(void);
void keylogger_set_buffer(const char* new_buffer, uint16_t new_index);
void keylogger_add_char(char c);
void keylogger_clear(void);
void keylogger_clear_buffer(void);
const char *keylogger_get_str(void);
bool keylogger_is_dirty(void);
void keylogger_set_dirty(bool dirty);
void keylogger_process_key(uint16_t keycode, keyrecord_t *record);
char keycode_to_char(uint16_t keycode, bool shift, bool caps);
void keylogger_get_line(uint8_t line_num, char *buffer);
void keylogger_set_line(uint8_t line_num, const char *line);
uint8_t keylogger_get_current_line(void);
void keylogger_add_char_with_modifiers(char c, bool ctrl, bool win, bool alt);
void keylogger_get_modifier_states(bool *ctrl, bool *win, bool *alt);
