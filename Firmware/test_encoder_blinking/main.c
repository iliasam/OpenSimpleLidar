//Open Simple Lidar v4
//Testing code - blinking LED when encoder event occurs
//UART TX pin is used as GPIO pin
//by ILIASAM

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "init_periph.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int main(void)
{
  init_all_periph();
  
  // Blinkng LED
  for (uint8_t i = 0; i < 10; i++)
  {
    switch_led(1);
    Delay_ms(300);
    switch_led(0);
    Delay_ms(300);
  }
  
  // Analyse encoder line
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
