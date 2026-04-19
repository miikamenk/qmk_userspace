#define OLED_BRIGHTNESS 128
#define OLED_TIMEOUT 5000

// Extend the keyboard's split transaction IDs to include our G7 sync RPC.
// Keyboard-level config.h defines OLEDFORCE_SYNC; append USER_SYNC_G7.
#undef SPLIT_TRANSACTION_IDS_USER
#define SPLIT_TRANSACTION_IDS_USER OLEDFORCE_SYNC, USER_SYNC_G7

// g7_sync_t (data + 24-sample graph) is ~36 bytes; default 32 is too small.
#define RPC_M2S_BUFFER_SIZE 48
