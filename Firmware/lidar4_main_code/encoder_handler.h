#ifndef _ENCODER_HANDLER_H
#define _ENCODER_HANDLER_H

#include "stm32f0xx.h"
#include "main.h"

#define ENC_TIM_NAME            TIM17
#define DEGREE_TIM_NAME         TIM16

#define TIMERS_FREQUENCY        50000 //Frequency of encoder timer and degree timer in Hz
#define KILO_CONST              1000

#define ENC_TIM_PRESCALER       (uint16_t)((SystemCoreClock / TIMERS_FREQUENCY) - 1)//APB1 = HCLK/2*2
#define DEGREE_TIM_PRESCALER    (uint16_t)((SystemCoreClock / TIMERS_FREQUENCY) - 1)//APB2 = HCLK


#define MAX_SPEED               (3.5f)//Maximum allowed rotation speed - in Hz
#define MIN_ENC_TIME            (uint16_t)((float)TIMERS_FREQUENCY / (MAX_SPEED * (ENC_NUM + 1)))//Minimum allowed time of encoder events
#define ENC_ARC_DEG             (uint16_t)(360 / (ENC_NUM + 1)) //Number of degrees in single encoder mark arc

void TIM16_IRQHandler(void);
void TIM17_IRQHandler(void);

void init_encoder_timer(void);
void init_degree_timer(void);
void refresh_degree_timer(uint16_t time);

uint8_t check_zero_point(uint16_t time);

void set_degree_timer_period(uint16_t period);

void emulated_encoder_timer_event(void);

#endif











