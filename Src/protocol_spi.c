#include "protocol_spi.h"
#include "spi.h"
#include <string.h>

osStreamId_t chip_output_stream;
osStreamId_t chip_input_stream;
osStreamId_t chip_return_stream;// return values got from spi, split from input stream

static uint8_t is_busy = 0; // 0: idle, otherwise: busy


int cmd_with_return(const uint8_t* cmd, uint8_t* ret_val, int timeout) {
    memset(ret_val, 0, CMD_SIZE);
    osStreamPut(chip_output_stream, cmd, CMD_SIZE, timeout);
    int len = osStreamGet(chip_return_stream, ret_val, CMD_SIZE, timeout);
    if(len == 0) return 0;
    return 1;
}
void cmd_no_return(const uint8_t* cmd, int timeout) {
    osStreamPut(chip_output_stream, cmd, CMD_SIZE, timeout);
}

void init_chip_stream() {
    chip_output_stream = osStreamNew(256);
    chip_input_stream = osStreamNew(256);
    chip_return_stream = osStreamNew(256);
}

void chip_send_knock() {
    uint8_t cmd[CMD_SIZE] = { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint8_t ret_val[CMD_SIZE];
    if(cmd_with_return(cmd, ret_val, 5000) != 0) {// get return value

    }else{

    }
}
void chip_return_knock() {
    uint8_t cmd[CMD_SIZE] = { 0x00, 'h', 'i', '\0', 0x00, 0x00, 0x00, 0x00 };
    cmd_no_return(cmd, 5000);
}

void chip_get_mode() {
    uint8_t cmd[CMD_SIZE] = { 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint8_t ret_val[CMD_SIZE];
    if(cmd_with_return(cmd, ret_val, 5000) != 0) {// get return value

    }else{

    }
}
uint8_t get_mode() {
    return 0x02; // 1: OTA, 2: APP
}
void chip_return_mode() {
    uint8_t mode = get_mode();
    uint8_t cmd[CMD_SIZE] = { 0x00, mode, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    cmd_no_return(cmd, 5000);
}

void chip_send_reboot() {
    uint8_t cmd[CMD_SIZE] = { 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    osStreamPut(chip_output_stream, cmd, CMD_SIZE, 5000);
}
void chip_send_reboot_ota() {
    uint8_t cmd[CMD_SIZE] = { 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    osStreamPut(chip_output_stream, cmd, CMD_SIZE, 5000);
}

void chip_get_link_state() {
    uint8_t cmd[CMD_SIZE] = { 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint8_t ret_val[CMD_SIZE];
    if(cmd_with_return(cmd, ret_val, 5000) != 0) {// get return value

    }else{

    }
}
void chip_return_link_state() {
    uint8_t cmd[CMD_SIZE] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    cmd_no_return(cmd, 5000);
}

void chip_send_midi(uint8_t* midi) {
    uint8_t cmd[CMD_SIZE] = { 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    memcpy(&cmd[1], midi, 4);
    osStreamPut(chip_output_stream, cmd, CMD_SIZE, 5000);
}

void chip_send_freq(float freq) {
    uint8_t cmd[CMD_SIZE] = { 0xAC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    memcpy(&cmd[1], (uint8_t*)(&freq), 4);
    cmd_no_return(cmd, 5000);
}

void chip_switch_led(uint8_t state) {
    uint8_t cmd[CMD_SIZE] = { 0xA0, state, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    cmd_no_return(cmd, 5000);
}
void chip_send_battery_life(uint8_t bl) {
    uint8_t cmd[CMD_SIZE] = { 0xA1, bl, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    cmd_no_return(cmd, 5000);
}

void parse_chip_received_cmd(const uint8_t* cmd) {
    switch(cmd[0]) {
        case OP_RETURN: //get return value;
            osStreamPut(chip_return_stream, cmd, CMD_SIZE, 5000);
            break;
        case OP_KNOCK: // receive knock cmd from remote
            chip_return_knock();
            break;
        case OP_MODE: // receive mode check cmd
            chip_return_mode();
            break;
        case OP_REBOOT: // receive reboot cmd
            break;
        case OP_REBOOT_DFU: // receive reboot ota cmd
            break;
        /*
        case OP_GET_LINKE_STATE: // receive get_link_state cmd
            chip_return_link_state();
            break;
        case OP_SEND_MIDI: // receive midi data
            break;
        case OP_SEND_FREQ: // receive frequence data
            break;
        case OP_SWITCH_LED: // receive LED switch
            break;
        case OP_SEND_BATTERY_LIFE: // receive battery life
            break;
        */
    }
}