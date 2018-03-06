//Open Simple Lidar v4
//Testing code - blinking LED when encoder evert occurs
//by ILIASAM

/* Includes ------------------------------------------------------------------*/
#include "main.h"


#include "init_periph.h"
#include "uart_handler.h"
#include <stdio.h>

int putchar(int c);

void uart_send_captured_data(void);

int main(void)
{
  init_all_periph();
  
  while (1)
  {
     process_handler();
    //start_send_result();
    //delay_ms(200);
  }
}


int putchar(int c)
{
  USART_SendData(USART1, c);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
  return c;
}
