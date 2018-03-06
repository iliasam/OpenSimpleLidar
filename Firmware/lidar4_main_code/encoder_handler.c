#include "main.h"
#include "encoder_handler.h"
#include "stdio.h"

//Used for speed sensor
volatile uint16_t capture_old = 0;
volatile uint16_t capture_now = 0;
volatile uint16_t time_tmp = 0;//Time of rotating for 1 deg
volatile uint8_t  rot_num  = 0;//Number of encoder events
volatile uint16_t rot_period  = 0;//Disc rotation period (in ms)

volatile uint8_t overspeed_flag   = 0;//1 - speed is too high
volatile uint8_t sync_failed_flag = 0;//1 - sync error - erron en counting encoder marks

volatile uint16_t cap_number = 0;//Number of sampled points
extern volatile meas_status_type meas_status;//Status of capture 3 images

//TIM16 Compare interrupt - every 1 deg of rotation
//used for starting image capture
void TIM16_IRQHandler(void)
{
  if (TIM_GetITStatus(DEGREE_TIM_NAME, TIM_IT_CC1) != RESET)
  {
    TIM_ClearITPendingBit(ENC_TIM_NAME, TIM_IT_CC1);
    DEGREE_TIM_NAME->SR &= ~TIM_SR_CC1IF;//Clear interrupt flag
    if (cap_number < 359)
    {
	  //Start capturing new degree - from zero phase
      meas_status = PHASE_WAIT;//Начать захват нового градуса (с нулевой фазы)
      cap_number++;
#ifdef TWO_DEG_MODE
  cap_number++;
#endif
      switch_adc_buf();
      meas_handler(); //Start capture
    }
  }
}

//TIM17 Input Capture interrupt - from encoder
void TIM17_IRQHandler(void)
{
  volatile uint16_t enc_period;//Time between encoder events
  volatile static uint16_t zero_old = 0;
  volatile static uint16_t zero_now = 0;
  
  if (TIM_GetITStatus(ENC_TIM_NAME, TIM_IT_CC1) != RESET)
  {
    TIM_ClearITPendingBit(ENC_TIM_NAME, TIM_IT_CC1);
    
    capture_old = capture_now;
    capture_now = TIM_GetCapture1(ENC_TIM_NAME);
    
    //Overflow controlling
    if (capture_now >= capture_old) 
      enc_period = capture_now - capture_old;
    else 
      enc_period = 0xFFFF - capture_old + capture_now;

#ifdef TWO_DEG_MODE
    if (enc_period < (MIN_ENC_TIME / 2))
    {
      switch_led(1);
      enc_period = (MIN_ENC_TIME / 2);
      overspeed_flag = 1;
    }    
#else
    if (enc_period < MIN_ENC_TIME)
    {
      switch_led(1);
      enc_period = MIN_ENC_TIME;
      overspeed_flag = 1;
    } 
#endif
    

    else 
    {   
      switch_led(0);
      overspeed_flag = 0;
    }
    
    
    time_tmp = enc_period / ENC_ARC_DEG;//time of rotation for 1 deg
    
    rot_num++;
    cap_number = rot_num * ENC_ARC_DEG;
    
    if (check_zero_point(time_tmp) == 1)//Zero crossing
    {
      //"enc_period" here is doubled because of encoder construction
      enc_period = enc_period / 2;
      time_tmp = enc_period / ENC_ARC_DEG;
      
      zero_old = zero_now;
      zero_now = TIM_GetCapture1(ENC_TIM_NAME);
      if (zero_now >= zero_old) 
        rot_period = (zero_now - zero_old) / (TIMERS_FREQUENCY / KILO_CONST);
      else 
        rot_period = (0xFFFF - zero_old + zero_now) / (TIMERS_FREQUENCY / KILO_CONST);
      
      if (rot_num != ENC_NUM)//(ENC_NUM-1) because of one absent mark
      {
        sync_failed_flag = 1;
        switch_led(1);
      }
      else
	  {
        //switch_led(0);
	  }
      
      stop_capture();
      if (sync_failed_flag == 0)
      {
        start_send_result();//Encoder is working fine
      }
      //printf("%d\r\n", rot_num);
      sync_failed_flag = 0;
      rot_num = 0;
      cap_number = 0;//???
      //printf("zero %d\r\n", time_tmp);
      refresh_degree_timer(time_tmp-1);
    }
    else
    {
      //printf("%d\r\n", time_tmp);
      set_degree_timer_period(time_tmp-1);
    }
  }
}

//This timer is capturing events from the encoder
//Timer mode - input capture
void init_encoder_timer(void)
{
  TIM_TimeBaseInitTypeDef       TIM_TimeBaseStructure;
  TIM_ICInitTypeDef             TIM_ICInitStructure;
  NVIC_InitTypeDef              NVIC_InitStructure;
  GPIO_InitTypeDef              GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);
  
  //Encoder
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin   = ENCODER_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  //GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(ENCODER_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(ENCODER_PORT, ENCODER_PIN_SRC, GPIO_AF_5);
  
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

  TIM_TimeBaseStructure.TIM_Prescaler   = ENC_TIM_PRESCALER;
  TIM_TimeBaseStructure.TIM_Period      = 0xFFFF;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(ENC_TIM_NAME, &TIM_TimeBaseStructure);
  
  // channel1 - input capture - falling edge
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_Channel       = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity    = TIM_ICPolarity_Falling;
  TIM_ICInitStructure.TIM_ICSelection   = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler   = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter      = 15;//uart produce some noise
  TIM_ICInit(ENC_TIM_NAME, &TIM_ICInitStructure);

  TIM_ARRPreloadConfig(TIM17, ENABLE);

  TIM_ClearITPendingBit(ENC_TIM_NAME, TIM_IT_CC1);
  TIM_ITConfig(ENC_TIM_NAME, TIM_IT_CC1, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel            = TIM17_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority    = 15;
  NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  TIM_Cmd(ENC_TIM_NAME, ENABLE);
}

//This timer is used for generating interrupts every 1 degree of rotating
void init_degree_timer(void)
{
  TIM_TimeBaseInitTypeDef       TIM_TimeBaseStructure;
  TIM_OCInitTypeDef             TIM_OCInitStructure;
  NVIC_InitTypeDef              NVIC_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);

  TIM_DeInit(DEGREE_TIM_NAME);
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

  TIM_TimeBaseStructure.TIM_Prescaler = TIM16_PRESCALER;
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(DEGREE_TIM_NAME, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;//Timing!
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
  TIM_OCInitStructure.TIM_Pulse = 100;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OC1Init(DEGREE_TIM_NAME, &TIM_OCInitStructure);

  TIM_ARRPreloadConfig(DEGREE_TIM_NAME, ENABLE);
  TIM_OC1PreloadConfig(DEGREE_TIM_NAME, TIM_OCPreload_Enable);

  TIM_ClearITPendingBit(DEGREE_TIM_NAME, TIM_IT_CC1);

  TIM_ITConfig(DEGREE_TIM_NAME, TIM_IT_CC1, ENABLE);
  NVIC_InitStructure.NVIC_IRQChannel            = TIM16_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority    = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  //NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
  //Timer will be started later - in "refresh_degree_timer"
}

//Если время в 1.5 раза превышает среднее - возвращает 1
//Return 1 if "time" > 1.5 times bigger than average time (4 points).
uint8_t check_zero_point(uint16_t time)
{
  static uint16_t times[4];
  static uint8_t pos = 0;//pos = 0-3
  uint16_t avr_time = 0;//average period
  uint16_t result;

  times[pos] = time;
  pos++;
  pos&= 3;
  avr_time = (times[0] + times[1] + times[2] + times[3]) / 4;
  time= time*4;
  result = time / avr_time;
  if (result >= 5)
    return 1;
  else 
    return 0;//6/4 = 1.5
}


//Stops degree timer and load a new value to it - for 1 deg rotation
//Called at zero crossing
void refresh_degree_timer(uint16_t time)
{

  DEGREE_TIM_NAME->CR1&= ~TIM_CR1_CEN;//disable timer
  TIM_SetCounter(DEGREE_TIM_NAME, 0);
  set_degree_timer_period(time);
  DEGREE_TIM_NAME->EGR|= TIM_EGR_UG;//update all registers and prescaler
  DEGREE_TIM_NAME->CR1|= TIM_CR1_CEN;//enable timer
}

//"period" - period in timer ticks
void set_degree_timer_period(uint16_t period)
{
#ifdef TWO_DEG_MODE
  period = period * 2;
#endif
  TIM_SetAutoreload(DEGREE_TIM_NAME, period);
  TIM_SetCompare1(DEGREE_TIM_NAME, (period/2));
}


