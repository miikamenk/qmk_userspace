#pragma once

#include "quantum.h"
#include "action.h"

// Keylogger configuration
#define KEYLOGGER_BUFFER_SIZE 324
#define TERMINAL_WIDTH 36
#define MAX_TERMINAL_LINES 9

// Keylogger buffer structure
typedef struct {
    char buffer[KEYLOGGER_BUFFER_SIZE + 1];
    uint16_t index;
    bool dirty;
} keylogger_state_t;

// Function declarations
void keylogger_init(void);
void keylogger_set_buffer(const char* new_buffer, uint16_t new_index);
void keylogger_add_char(char c);
void keylogger_clear(void);
const char *keylogger_get_str(void);
bool keylogger_is_dirty(void);
void keylogger_set_dirty(bool dirty);
void keylogger_process_key(uint16_t keycode, keyrecord_t *record);
char keycode_to_char(uint16_t keycode, bool shift, bool caps);
