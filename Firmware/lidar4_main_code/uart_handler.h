#ifndef _UART_HANDLER_H
#define _UART_HANDLER_H
#include "stm32f0xx.h"

void start_send_result(void);
void uart_tx_dma_config(uint8_t* data_ptr, uint16_t length);
void clear_tx_buffer(void);

#endif











