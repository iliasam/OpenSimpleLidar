#include "init_periph.h"
#include "stm32f0xx.h"
#include "main.h"


void init_all_periph(void)
{
 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  init_clk();
  init_gpio();
}

//Init Clock System
//Sys clock frequency - 48 MHz
void init_clk(void)
{
  ErrorStatus HSEStartUpStatus;
  
  RCC_DeInit(); /* RCC system reset(for debug purpose) */
  RCC_HSEConfig(RCC_HSE_ON);/* Enable HSE */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();/* Wait till HSE is ready */

  if (HSEStartUpStatus == SUCCESS)
  {
    FLASH_PrefetchBufferCmd(ENABLE);  // Enable Prefetch Buffer
    FLASH_SetLatency(FLASH_Latency_1);// Flash 1 wait state
    
    RCC_HCLKConfig(RCC_SYSCLK_Div1);// HCLK = SYSCLK
    RCC_PCLKConfig(RCC_HCLK_Div1);  // APB  = HCLK
    
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_6);// PLLCLK = 8MHz * 6 = 48 MHz
    RCC_PLLCmd(ENABLE);/* Enable PLL */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){};/* Wait till PLL is ready */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);/* Select PLL as system clock source */
    while (RCC_GetSYSCLKSource() != 0x08){}/* Wait till PLL is used as system clock source */
  }
  
  //testing
  volatile uint32_t nCount;
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq (&RCC_Clocks);
  nCount=RCC_Clocks.HCLK_Frequency;
    
  return;
  
}

void init_gpio(void)
{ 
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE );
    
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = SI_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(SI_PORT, &GPIO_InitStructure);
   
  //Adc
  GPIO_InitStructure.GPIO_Pin   = ADC_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
  GPIO_Init(ADC_PORT, &GPIO_InitStructure);
  
  //Encoder
  GPIO_InitStructure.GPIO_Pin   = ENCODER_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;//GPIO_Mode_AF
  GPIO_Init(ENCODER_PORT, &GPIO_InitStructure);
  
  //Laser
  GPIO_InitStructure.GPIO_Pin   = LASER_PINS;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_Init(LASER_PORT, &GPIO_InitStructure);
  
  //LED
  GPIO_InitStructure.GPIO_Pin   = LED_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_Init(LED_PORT, &GPIO_InitStructure);
  
  //UART is PushPull GPIO
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin   = UART_TX_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_Init(UART_TX_PORT, &GPIO_InitStructure);
}

void switch_led(uint8_t val)
{
  if (val != 0)
    LED_PORT->ODR|= LED_PIN;
  else
    LED_PORT->ODR&= ~LED_PIN;
}


void Delay_ms(uint32_t ms)
{
  volatile uint32_t nCount;
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq (&RCC_Clocks);
  nCount=(RCC_Clocks.HCLK_Frequency/10000)*ms;
  for (; nCount!=0; nCount--);
}


