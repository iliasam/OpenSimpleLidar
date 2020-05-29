#include "init_periph.h"
#include "uart_handler.h"

//Prepare UART DMA for transmit data (16-bit data - interpreted by DMA like 8-bit data)
void uart_tx_dma_config(uint16_t* data_ptr, uint16_t length)
{ 
   DMA_Cmd(UART_TX_DMA_CHANNEL, DISABLE);
   UART_TX_DMA_CHANNEL->CMAR = (uint32_t)data_ptr;
   UART_TX_DMA_CHANNEL->CNDTR = (uint32_t)length * 2;//16 bit
   DMA_ClearFlag(DMA1_FLAG_TC4);
   DMA_Cmd(UART_TX_DMA_CHANNEL, ENABLE);
}