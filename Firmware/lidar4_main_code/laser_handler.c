#include "main.h"
#include "init_periph.h"
#include "uart_handler.h"
#include "laser_handler.h"
#include "image_processing.h"

//Results buffers - their values are transmitted to UART
uint16_t res_buf0[PACKET_LENGTH];
uint16_t res_buf1[PACKET_LENGTH];
uint8_t  res_buf_num = 0;//Number of buffer, that are filling NOW, another one is transmitting to UART

//Buffer0 - raw data from ADC
volatile uint16_t data_adc_laser0[CAPTURED_POINTS_CNT];
volatile uint16_t data_adc_off0[CAPTURED_POINTS_CNT];

//Buffer1 - raw data from ADC
volatile uint16_t data_adc_laser1[CAPTURED_POINTS_CNT];
volatile uint16_t data_adc_off1[CAPTURED_POINTS_CNT];

volatile uint16_t* data_adc_laser_p = data_adc_laser0;//pointer to raw data from ADC with enabled laser
volatile uint16_t* data_adc_off_p   = data_adc_off0;  //pointer to raw data from ADC with disabled laser

volatile uint8_t proc_done_flag = 0;//This flag is set to 1 after processing all data

volatile meas_status_type meas_status;//Status of captring data

extern volatile uint16_t cap_number;//Number of sampled points


//управляет захватом
//вызывается из обработчика прерывания TIM16 или по прерыванию DMA TIM1 - по завершению захвата строки
//This function is controlling data capture - it is switching capture modes
void meas_handler(void)
{
  
  switch(meas_status)
  {
    case PHASE_WAIT:
    {
      //switch_led(1);
      enable_laser();
      capture_start(data_adc_off_p);//Laser is enabled, but captured data are dummy
      meas_status = PHASE_1;
      break;
    }
    case PHASE_1: //завершилась 1 фаза
    {
      disable_laser();
      capture_start(data_adc_laser_p);//Laser is disabled, but captured data are with laser enabled
      meas_status = PHASE_2;
      break;
    }

    case PHASE_2: //завершилась 2 фаза
    {
      capture_start(data_adc_off_p);//Laser is disabled, and captured data are with laser disabled
      meas_status = PHASE_3;
      break;
    }
    case PHASE_3: //завершилась 3 фаза
    {
      meas_status = PHASE_DONE;
      proc_done_flag = 0;//декодирование разрешено
      //switch_led(0);
      break;
    }
    case PHASE_DONE:
    {
      break;
    }
  }//end of case
  
  //meas_status = PHASE_DONE;
  //proc_done_flag = 0;//декодирование разрешено
}


//переключает буферы захавта ацп
//вызывается из обработчика прерывания - перед началом новгого захвата
//Switch ADC capture buffer
//Called from DMA interrupt handler
void switch_adc_buf(void)
{
  volatile static uint8_t tmp_flag = 0;
  tmp_flag^= 1;
  
  //if ((cap_number & 1) == 1) //нечетные изображения в 0 буфер
  if ((tmp_flag & 1) == 1) //нечетные изображения в 0 буфер
  {
    data_adc_laser_p = &data_adc_laser0[0];
    data_adc_off_p   = &data_adc_off0[0];
  }
  else //четные изображения в 1 буфер
  {
    data_adc_laser_p = &data_adc_laser1[0];
    data_adc_off_p   = &data_adc_off1[0];
  }
}


//производит обработку полученных данных
//Make data processing
void process_handler(void)
{
  //This variables are used to protect from changing volatile variables
  //для защиты от возможного изменения переменной
  uint16_t tmp_cap_number = cap_number;
  uint8_t  tmp_res_buf_num = res_buf_num;
  uint16_t tmp_centroid = 0;

  //If images of the last degree are captured and processing is not done
  //Изображения последнего градуса захвачены и обработка еще не выполнялась
  if ((meas_status == PHASE_DONE) && (proc_done_flag == 0))
  {
    if (tmp_cap_number > 359)
    {
      //Error found
      proc_done_flag = 1;
      return;
    }
  
    //switch_led(1);
    centroid_result_type centroid_result = find_centroid();
    
    
    //Data is placed to  certain result buffer
    //Данные помещаются в нужный буфер
    tmp_centroid = centroid_result.centroid;
    
    if (centroid_result.max_val < 50)
      tmp_centroid = 1;//Amplitude is too low 
    
    if (tmp_res_buf_num == 0)
      res_buf0[tmp_cap_number + PACKET_OFFSET] = tmp_centroid; 
    else
      res_buf1[tmp_cap_number + PACKET_OFFSET] = tmp_centroid;
    
    
    proc_done_flag = 1;//Protection from new data processing start - защита от повторного запуска
    //switch_led(0);
  }
}

//Stop capturng image, reset some flags and switch buffers
void stop_capture(void)
{
  stop_hardware_capture();
  meas_status = PHASE_WAIT;
  res_buf_num^= 1;//Switch result buffer - изменить буфер результата
  clear_tx_buffer();//Clean tx buffer before transmitting data
}

