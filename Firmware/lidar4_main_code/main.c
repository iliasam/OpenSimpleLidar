//Open Simple Lidar v4
//Testing code - blinking LED when encoder evert occurs
//by ILIASAM

//Main configuration values are placed at "main.h"

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "init_periph.h"
#include "uart_handler.h"
#include <stdio.h>

int putchar(int c);

void uart_send_captured_data(void);

int main(void)
{
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq (&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / SYSTICK_FREQUENCY);
  
  init_all_periph();
  
  while (1)
  {
     process_handler();
  }
}


int putchar(int c)
{
  USART_SendData(USART1, c);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
  return c;
}
