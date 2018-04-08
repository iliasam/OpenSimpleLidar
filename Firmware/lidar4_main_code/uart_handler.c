#include "main.h"
#include "uart_handler.h"
#include "string.h"

extern uint16_t res_buf0[PACKET_LENGTH];
extern uint16_t res_buf1[PACKET_LENGTH];

//Number of buffer, that are filling NOW, another one is transmitting to UART- switched at zero crossing
extern uint8_t  res_buf_num;

extern volatile uint16_t rot_period;//Disc rotation period (in ms)
extern volatile uint8_t overspeed_flag;

#define PACKET_LENGTH_IN_BYTES (uint16_t)(PACKET_LENGTH*2)

//0 bit - overspeed flag
#define STATUS_WORD_OVERSPEED_MASK     (1<<0)

//15 bit - shows that this is lidar 4
#define STATUS_WORD_LIDAR4_MASK        (1<<15)

//LSB is transmetted first
//This function found the bufeer to take data and starts transmission
//360 data + 2 header + 1 status flags + 1 speed
void start_send_result(void)
{
  /*
  uint16_t i;
  for (i=4; i<364; i++)
  {
    res_buf1[i] = i*10;
    res_buf0[i] = i*15;
  }
  */
  
  uint16_t tmp_status_value = 0;
  if (overspeed_flag != 0) 
    tmp_status_value|= STATUS_WORD_OVERSPEED_MASK;
  
  tmp_status_value|= STATUS_WORD_LIDAR4_MASK;
  
  if (res_buf_num == 0)
  {
    res_buf1[0] = 0xBBAA;//header
    res_buf1[1] = 0xDDCC;//header
    res_buf1[2] = tmp_status_value;//status flag
    res_buf1[3] = rot_period;//speed
    uart_tx_dma_config((uint8_t*)res_buf1, PACKET_LENGTH_IN_BYTES);
  }
  else
  {
    res_buf0[0] = 0xBBAA;
    res_buf0[1] = 0xDDCC;
    res_buf0[2] = tmp_status_value;//status flag
    res_buf0[3] = rot_period;//speed
    uart_tx_dma_config((uint8_t*)res_buf0, PACKET_LENGTH_IN_BYTES);
  }
}

//Prepare UART DMA for transmit data
void uart_tx_dma_config(uint8_t* data_ptr, uint16_t length)
{ 
   DMA_Cmd(UART_TX_DMA_CHANNEL, DISABLE);
   UART_TX_DMA_CHANNEL->CMAR = (uint32_t)(data_ptr);
   UART_TX_DMA_CHANNEL->CNDTR = (uint32_t)length;
   DMA_ClearFlag(UART_TX_DMA_FLAG);
   DMA_ClearFlag(UART_TX_DMA_FLAGE);
   DMA_Cmd(UART_TX_DMA_CHANNEL, ENABLE);
}

//Clear current results buffer before writing to it
void clear_tx_buffer(void)
{
  if (res_buf_num == 0)
    memset(res_buf0, 0, sizeof(res_buf0));
  else
    memset(res_buf1, 0, sizeof(res_buf1));
}