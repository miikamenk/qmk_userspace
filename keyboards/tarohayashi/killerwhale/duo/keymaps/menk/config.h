#define OLED_BRIGHTNESS 128
#define OLED_TIMEOUT 5000

// Extend the keyboard's split transaction IDs to include our user RPCs.
// Keyboard-level config.h defines OLEDFORCE_SYNC; append USER_SYNC_G7 and
// USER_SYNC_CAPS_WORD.
#undef SPLIT_TRANSACTION_IDS_USER
#define SPLIT_TRANSACTION_IDS_USER OLEDFORCE_SYNC, USER_SYNC_G7, USER_SYNC_CAPS_WORD

// g7_sync_t (data + 24-sample graph) is ~36 bytes; default 32 is too small.
#define RPC_M2S_BUFFER_SIZE 48

// Caps Word: hold both shifts to toggle, single shift inverts the next key
// while caps word is on, never time out unless explicitly cancelled.
#define BOTH_SHIFTS_TURNS_ON_CAPS_WORD
#define CAPS_WORD_INVERT_ON_SHIFT
#define CAPS_WORD_IDLE_TIMEOUT 0
