#ifndef LICAP_R_EVT_PROTOCOL_SPI_H
#define LICAP_R_EVT_PROTOCOL_SPI_H

#include "protocol.h"
#include "cmsis_os2.h"

#define CMD_SIZE 8

#ifdef __cplusplus
extern "C" {
#endif
void init_chip_stream();
void init_chip_rtos_tasks();
void parse_chip_received_cmd(const uint8_t *cmd);

extern osStreamId_t chip_output_stream;
extern osStreamId_t chip_input_stream;
extern osStreamId_t chip_return_stream;// return values got from spi, split from input stream


#ifdef __cplusplus
}
#endif

#endif //LICAP_R_EVT_PROTOCOL_SPI_H
