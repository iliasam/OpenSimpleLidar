//Open Simple Lidar v4
//Testing code - capture image from linear image sensor and send it to PC
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
void uart1_send_byte(uint8_t c);

uint16_t data_adc_laser[CAPTURED_POINTS_CNT];
uint16_t data_adc_off[CAPTURED_POINTS_CNT];

volatile uint16_t* data_adc_laser_p = &data_adc_laser[0];//pointer to data with laser enabled
volatile uint16_t* data_adc_off_p   = &data_adc_off[0];//pointer to data with laser disabled

extern volatile uint8_t capture_done_flag;


void uart_send_captured_data(void);

int main(void)
{
  init_all_periph();
  
  enable_laser();
  
  //TIM1->CR1 |= TIM_CR1_CEN;//Start TIM1 -> ADC -> DMA -> RAM
  
  
  
  while (1)
  {
    //Delay_ms(500);
    //switch_led(1);
    /*
    Delay_ms(500);
    switch_led(0);
    */
    switch_led(1);
    disable_laser();
    capture_start(data_adc_off);
    while (capture_done_flag == 0){} //bad data
    
    
    
    enable_laser();
    capture_done_flag = 0;
    capture_start(data_adc_off);
    while (capture_done_flag == 0){}
    
    disable_laser();
    capture_done_flag = 0;
    capture_start(data_adc_laser);
    while (capture_done_flag == 0){}
    capture_done_flag = 0;
    switch_led(0);
    
    
    uart_send_captured_data();
  }
}

//testing
void uart1_send_byte(uint8_t c)
{
    USART_SendData(USART1, c);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
}

void uart_send_captured_data(void)
{
  uint8_t i;
  
  uart1_send_byte(65);
  uart1_send_byte(98);
  uart1_send_byte(67);
  uart1_send_byte(100);
  uart1_send_byte(69);
  uart1_send_byte(102);
  
  
  for (i=0; i<CAPTURED_POINTS_CNT; i++)
  {
    if (data_adc_laser[i] > data_adc_off[i]) {data_adc_laser[i] = data_adc_laser[i] - data_adc_off[i];} else {data_adc_laser[i] = 0;}
    data_adc_laser[i] = data_adc_laser[i];
    //data_adc_off[i] = 10+i;
  }
  
  
  uart_tx_dma_config(data_adc_laser, CAPTURED_POINTS_CNT);
  Delay_ms(40);//время на передачу
}
