#ifndef LICAP_R_EVT_PROTOCOL_H
#define LICAP_R_EVT_PROTOCOL_H

typedef enum {
    OP_RETURN,
    OP_KNOCK,
    OP_MODE,
    OP_REBOOT,
    OP_REBOOT_DFU
} opcode_t;

typedef enum {
    DFU=0x01,
    APP
} dev_mode_t;

typedef enum {
    OK,
    TIMEOUT
} protocol_status_t;

#endif //LICAP_R_EVT_PROTOCOL_H
