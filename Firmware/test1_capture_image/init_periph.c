#include "init_periph.h"
#include "stm32f0xx.h"
#include "main.h"


DMA_InitTypeDef           DMA_InitStructure;
extern tx_status_type tx_status;

extern uint16_t data_adc_off[CAPTURED_POINTS_CNT];


void init_all_periph(void)
{
 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  init_clk();
  init_gpio();
  init_uart1();
  //ext_int_init();
  timer1_init();
  //timer2_init();
  init_adc();
  adc_dma_init();
  uart_dma_init();
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
   
  //Configure TIM1 PWM (CLK pin)
  init_clk_pin(CLK_GPIO);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_2);//TIM1
  
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
}

//Initialize CLK pin for GPIO or PWM mode
void init_clk_pin(ClkPinType pin_type)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin   = CLK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  
  if (pin_type == CLK_PWM)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  else
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  
  GPIO_Init(CLK_PORT, &GPIO_InitStructure);
}


//Initialize TIM1 - used to generate CLK pulses and used as ADC trigger
void timer1_init(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  
  //This timer is used as a trigger for ADC
  //TIM1 configuration
  TIM_DeInit(TIM1);
  TIM_OCStructInit(&TIM_OCInitStructure);
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

  TIM_TimeBaseStructure.TIM_Prescaler = 0; 
  TIM_TimeBaseStructure.TIM_Period = 83 - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
  
  
  // channel2 - PWM
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
  TIM_OCInitStructure.TIM_Pulse = 10; 
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;         
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);
  
  // channel4 - ADC trigger
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;// TIM_OutputState_Enable needed for F030 to be ADC trigger         
  TIM_OCInitStructure.TIM_Pulse = 70; 
  TIM_OC4Init(TIM1, &TIM_OCInitStructure);
  
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
  TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
  TIM_ARRPreloadConfig(TIM1, ENABLE);
  
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
  
  //This timer will be started later
}

void timer1_set_new_period(uint16_t period)
{
  TIM_SetAutoreload(TIM1, period);
  TIM_SetCompare2(TIM1, 10);
  TIM_SetCompare4(TIM1, period-10);
}


void init_adc(void)
{
  ADC_InitTypeDef  ADC_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
  RCC_ADCCLKConfig(RCC_ADCCLK_PCLK_Div2);
  
  ADC_StructInit(&ADC_InitStructure);
  
  // ADC1 configuration  
  ADC_InitStructure.ADC_Resolution              = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ContinuousConvMode      = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv        = ADC_ExternalTrigConv_T1_CC4;
  ADC_InitStructure.ADC_ExternalTrigConvEdge    = ADC_ExternalTrigConvEdge_Rising;
  ADC_InitStructure.ADC_DataAlign               = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ScanDirection           = ADC_ScanDirection_Upward;
  ADC_Init(ADC1, &ADC_InitStructure);
  
  ADC_ChannelConfig(ADC1, ADC_Channel_0 , ADC_SampleTime_1_5Cycles);
  
  ADC_GetCalibrationFactor(ADC1);
  
  ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_OneShot);
  ADC_DMACmd(ADC1, ENABLE);//adc generate events for DMA
  
  /// Enable ADC1
  ADC_Cmd(ADC1, ENABLE);
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY)); 
  
  ADC_StartOfConversion(ADC1);
}


//DAC is used to copy data from ADC to RAM
void adc_dma_init(void)
{
  DMA_InitTypeDef           DMA_InitStructure;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
  DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)&data_adc_off[0];
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize         = CAPTURED_POINTS_CNT;
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority           = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
  DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure); 
  
  DMA_Cmd(ADC_DMA_CHANNEL, ENABLE);
  
  //DMA_RemapConfig(DMA1, ADC_DMA_CHANNEL);
  DMA_Init(UART_TX_DMA_CHANNEL, &DMA_InitStructure); 
  
  DMA_ITConfig(ADC_DMA_CHANNEL, DMA_IT_TC, ENABLE);//transfer complete
  
  NVIC_SetPriority(DMA1_Channel1_IRQn, 1);
  NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}


//Configure DMA for new capture
void dma_config(uint16_t* pointer)
{ 
   DMA_Cmd(DMA1_Channel1, DISABLE);
   //DMA1_Channel1->CMAR = (uint32_t)&data_adc[0];
   DMA1_Channel1->CMAR = (uint32_t)pointer;
   DMA1_Channel1->CNDTR = CAPTURED_POINTS_CNT;
}

//Start image capture
void capture_start(uint16_t* pointer)
{ 
  init_clk_pin(CLK_GPIO);
  GPIO_SetBits(SI_PORT, SI_PIN);
  small_delay();
  GPIO_SetBits(CLK_PORT, CLK_PIN);
  small_delay();
  GPIO_ResetBits(SI_PORT, SI_PIN);
  small_delay();

  GPIO_ResetBits(CLK_PORT, CLK_PIN);
  init_clk_pin(CLK_PWM);
  
  ADC_StartOfConversion(ADC1);
  TIM_SetCounter(TIM1,0);
  dma_config(pointer);
  DMA_Cmd(DMA1_Channel1, ENABLE);
  TIM1->CR1 |= TIM_CR1_CEN;//Start TIM1 -> ADC -> DMA -> RAM
}



void small_delay(void)
{
  //asm("nop");
  //asm("nop");asm("nop");
  //asm("nop");
}


void enable_laser(void)
{
  LASER_PORT->ODR |= LASER_PINS;
}

void disable_laser(void)
{
  LASER_PORT->ODR&= ~LASER_PINS;
}


void switch_led(uint8_t val)
{
  if (val != 0)
    LED_PORT->ODR|= LED_PIN;
  else
    LED_PORT->ODR&= ~LED_PIN;
}


void init_uart1(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  
  // Enable GPIO clock
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE );
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  RCC_USARTCLKConfig(RCC_USART1CLK_SYSCLK);
  
  GPIO_PinAFConfig(GPIOA, UART_TX_PINSOURCE, GPIO_AF_1); //GPIO_AF_1 - uart1
  
  // Configure USART1 Tx as alternate function push-pull
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin   = UART_TX_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_Init(UART_TX_PORT, &GPIO_InitStructure);
  
  USART_InitStructure.USART_BaudRate    = UART_BAUDRATE;
  USART_InitStructure.USART_WordLength  = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits    = USART_StopBits_1;
  USART_InitStructure.USART_Parity      = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode        = USART_Mode_Tx;
  
  USART_Init(USART1, &USART_InitStructure);
  
  //USART_DMAReceptionErrorConfig(USART1, USART_DMAOnError_Enable);
  USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
  
  USART_Cmd(USART1, ENABLE);
}

//Initialize DMA for UART
void uart_dma_init(void)
{
  DMA_InitTypeDef           DMA_InitStructure;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  
  DMA_DeInit(UART_TX_DMA_CHANNEL);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->TDR;
  DMA_InitStructure.DMA_MemoryBaseAddr     = SRAM_BASE;//only for init
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize         = 10;
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;
  DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
  
  DMA_RemapConfig(DMA1, DMA1_CH2_USART1_TX);
  DMA_Init(UART_TX_DMA_CHANNEL, &DMA_InitStructure); 
  //DMA_Cmd(UART_TX_DMA_CHANNEL, ENABLE);
}

void Delay_ms(uint32_t ms)
{
  volatile uint32_t nCount;
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq (&RCC_Clocks);
  nCount=(RCC_Clocks.HCLK_Frequency/10000)*ms;
  for (; nCount!=0; nCount--);
}


