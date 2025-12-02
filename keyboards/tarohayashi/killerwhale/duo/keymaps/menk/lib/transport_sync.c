// Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "transport_sync.h"
#include "_wait.h"
#include "transactions.h"
#include <string.h>
#include "split_util.h"
#ifdef WPM_ENABLE
extern uint8_t wpm_graph_samples[WPM_GRAPH_SAMPLES];
#endif // WPM_ENABLE
#ifdef KILLERWHALE_ENABLE
#    include "common_killerwhale.h"
extern kw_config_t kw_config;
#endif
#ifndef FORCED_SYNC_THROTTLE_MS
#    define FORCED_SYNC_THROTTLE_MS 100
#endif // FORCED_SYNC_THROTTLE_MS

bool has_first_run = false;

typedef enum PACKED extended_id_t {
    RPC_ID_EXTENDED_WPM_GRAPH_DATA = 0,
    RPC_ID_EXTENDED_KILLERWHALE_CONFIG,
    NUM_EXTENDED_IDS,
} extended_id_t;

_Static_assert(sizeof(extended_id_t) == 1, "extended_id_t is not 1 byte!");

#define RPC_EXTENDED_TRANSACTION_OVERHEAD (sizeof(extended_id_t) + sizeof(uint8_t))
#define RPC_EXTENDED_TRANSACTION_BUFFER_SIZE (RPC_M2S_BUFFER_SIZE - RPC_EXTENDED_TRANSACTION_OVERHEAD)

typedef struct PACKED extended_msg_t {
    extended_id_t id;
    uint8_t       size;
    uint8_t       data[RPC_EXTENDED_TRANSACTION_BUFFER_SIZE];
} extended_msg_t;

_Static_assert(sizeof(extended_msg_t) == RPC_M2S_BUFFER_SIZE, "extended_rpc_t is larger than split buffer size!");

typedef void (*handler_fn_t)(const uint8_t *data, uint8_t size);

void recv_wpm_graph_data(const uint8_t *data, uint8_t size) {
#ifdef WPM_ENABLE
    if (memcmp(data, wpm_graph_samples, size) != 0) {
        memcpy(wpm_graph_samples, data, size);
    }
#endif
}

void recv_killerwhale_config(const uint8_t *data, uint8_t size) {
    if (memcmp(data, &kw_config, size) != 0) {
        memcpy(&kw_config, data, size);

        xprintf("Handler for message ID %d is NULL\n", kw_config.oled_state_slave);
        eeconfig_update_kb(kw_config.raw);
    }
}

static const handler_fn_t handlers[NUM_EXTENDED_IDS] = {[RPC_ID_EXTENDED_WPM_GRAPH_DATA] = recv_wpm_graph_data, [RPC_ID_EXTENDED_KILLERWHALE_CONFIG] = recv_killerwhale_config};

/**
 * @brief Handles extended messages received via the split keyboard transport mechanism.
 *
    if (((uintptr_t)initiator2target_buffer % _Alignof(extended_msg_t)) != 0) {
        xprintf("Buffer is not properly aligned for extended_msg_t\n");
        return;
    }
    memcpy(&msg, initiator2target_buffer, initiator2target_buffer_size);
 * validating the message size, and invoking the appropriate handler function.
 *
 * @param initiator2target_buffer_size The size of the buffer containing the message from the initiator.
 * @param initiator2target_buffer Pointer to the buffer containing the message data from the initiator.
 * @param target2initiator_buffer_size The size of the buffer for the response message to the initiator.
 * @param target2initiator_buffer Pointer to the buffer for the response message to the initiator.
 */
void extended_message_handler(uint8_t initiator2target_buffer_size, const void *initiator2target_buffer, uint8_t target2initiator_buffer_size, void *target2initiator_buffer) {
    extended_msg_t msg = {0};
    memcpy(&msg, initiator2target_buffer, initiator2target_buffer_size);
    if (msg.id >= NUM_EXTENDED_IDS) {
        xprintf("Invalid extended message ID: %d\n", msg.id);
        return;
    }
    if (msg.size > RPC_EXTENDED_TRANSACTION_BUFFER_SIZE) {
        xprintf("Invalid extended message size: %d (ID: %d)\n", msg.size, msg.id);
        return;
    }

    handler_fn_t handler = handlers[msg.id];
    if (handler == NULL) {
        xprintf("Handler for message ID %d is NULL\n", msg.id);
        return;
    }
    // xprintf("Extended Transaction received:\nID: %d, Size: %d, data:\n  ", msg.id, msg.size);
    // for (uint8_t i = 0; i < msg.size; ++i) {
    //     xprintf("%d ", msg.data[i]);
    // }
    // xprintf("\n");

    handler(msg.data, msg.size);
}

/**
 * @brief Sends an extended message to the other half of the split keyboard.
 *
 * This function constructs an extended message with the given ID and data,
 * and sends it using the transaction RPC mechanism. It ensures that the
 * message size does not exceed the buffer size.
 *
 * @param id The ID of the extended message to send.
 * @param data Pointer to the data to include in the message.
 * @param size The size of the data in bytes.
 * @return true if the message was successfully sent, false otherwise.
 */
bool send_extended_message_handler(enum extended_id_t id, const void *data, uint8_t size) {
    if (size > RPC_EXTENDED_TRANSACTION_BUFFER_SIZE) {
        xprintf("Invalid extended message size: %d (ID: %d)\n", size, id);
        return false;
    }
    extended_msg_t msg = {
        .id   = id,
        .size = size,
        .data = {0},
    };
    memcpy(msg.data, data, size);
    // xprintf("Extended Transaction sent:\nID: %d, Size: %d, data:\n  ", msg.id, msg.size);
    // for (uint8_t i = 0; i < size; ++i) {
    //     xprintf("%d ", msg.data[i]);
    // }
    // xprintf("\n");
    return transaction_rpc_send(RPC_ID_EXTENDED_SYNC_TRANSPORT, sizeof(extended_msg_t), &msg);
}

#ifdef WPM_ENABLE
void sync_wpm_graph_data(void) {
    bool            needs_sync                                 = false;
    static uint16_t last_sync                                  = 0;
    static uint8_t  local_wpm_graph_samples[WPM_GRAPH_SAMPLES] = {0};

    if (memcmp(wpm_graph_samples, local_wpm_graph_samples, sizeof(local_wpm_graph_samples))) {
        needs_sync = true;
        memcpy(local_wpm_graph_samples, wpm_graph_samples, sizeof(local_wpm_graph_samples));
    }
    if (timer_elapsed(last_sync) > 1000) {
        needs_sync = true;
    }
    if (needs_sync) {
        if (send_extended_message_handler(RPC_ID_EXTENDED_WPM_GRAPH_DATA, wpm_graph_samples, sizeof(wpm_graph_samples))) {
            last_sync = timer_read32();
        }
    }
}
#endif // WPM_ENABLE

void sync_killerwhale_config(void) {
    bool               needs_sync     = false;
    static uint16_t    last_sync      = 0;
    static kw_config_t last_kw_config = {0};

    if (memcmp(&kw_config, &last_kw_config, sizeof(kw_config_t))) {
        needs_sync = true;
        memcpy(&last_kw_config, &kw_config, sizeof(kw_config_t));
    }
    if (timer_elapsed(last_sync) > FORCED_SYNC_THROTTLE_MS) {
        needs_sync = true;
    }
    if (needs_sync) {
        if (send_extended_message_handler(RPC_ID_EXTENDED_KILLERWHALE_CONFIG, &kw_config, sizeof(kw_config_t))) {
            last_sync = timer_read();
        }
    }
}

/**
 * @brief Initialize the transport sync
 *
 */
void keyboard_post_init_transport_sync(void) {
    // Register keyboard state sync split transaction
    transaction_register_rpc(RPC_ID_EXTENDED_SYNC_TRANSPORT, extended_message_handler);
}

/**
 * @brief Performs housekeeping tasks related to transport synchronization.
 *
 * This function is responsible for managing and executing any necessary
 * housekeeping tasks to ensure proper synchronization of transport mechanisms
 * in a split keyboard setup.
 */
void housekeeping_task_transport_sync(void) {
    if (!is_transport_connected()) {
        return;
    }

    if (!has_first_run) {
        if (timer_elapsed(0) > 1500) {
            has_first_run = true;
        } else {
            return;
        }
    }

    // Data sync from master to slave
    if (is_keyboard_master()) {
#ifdef WPM_ENABLE
        sync_wpm_graph_data();
#endif // WPM_ENABLE
        sync_killerwhale_config();
    }
}
