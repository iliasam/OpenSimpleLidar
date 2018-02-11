//Open Simple Lidar v4
//Testing code - blinking LED when encoder evert occurs
//UART TX pin is used as GPIO pin
//by ILIASAM

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "init_periph.h"
#include "uart_handler.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int main(void)
{
  init_all_periph();
  
  switch_led(1);
  
  while (1)
  {
    if  (GPIO_ReadInputDataBit(ENCODER_PORT, ENCODER_PIN) == Bit_RESET)
    {
      switch_led(1);
      UART_TX_PORT->ODR|= UART_TX_PIN;
    }
    else
    {
      switch_led(0);
      UART_TX_PORT->ODR&= ~UART_TX_PIN;
    }
  }
}
