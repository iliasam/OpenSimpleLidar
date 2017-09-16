#include "main.h"
#include "image_processing.h"

#define AN_WIDTH_HIGH 8 //половина ширины анализа пика
#define AN_WIDTH_LOW 3 //половина ширины анализа пика

//#define REFL_FILTER //защита от ложных отраженных сигналов

uint8_t AN_WIDTH = AN_WIDTH_HIGH;

extern volatile uint16_t* data_adc_laser_p;//pointer to raw data from ADC with enabled laser
extern volatile uint16_t* data_adc_off_p;  //pointer to raw data from ADC with disabled laser

centroid_result_type find_centroid(void)
{
  uint8_t i;
  uint8_t  max_pos = 0;
  uint16_t max_val = 0;
  uint8_t  start_pos;
  uint8_t  stop_pos;
  uint32_t summ = 0;
  uint32_t summ2 = 0;
  uint32_t tmp_centroid = 0;
  
  uint16_t proc_data[CAPTURED_POINTS_CNT];//массив обработки
  uint16_t* proc_adc_laser_p;//указатель на данные с лазером
  uint16_t* proc_adc_off_p;//указатель на данные без лазера
  
  int16_t delta1;
  int16_t delta2;
  uint8_t fmax_val = 0;
  
  uint16_t val1 = 0;
  uint16_t val2 = 0;
  
  uint16_t off_flag = 0;//2 изображения с выключенным лазером
  
  centroid_result_type result = {0,0,0};
  
  proc_adc_laser_p = (uint16_t*)data_adc_laser_p;
  proc_adc_off_p   = (uint16_t*)data_adc_off_p;
    
  for (i=0; i<128; i++)
  {
    //вычитание одного изображения из другого
    if (proc_adc_laser_p[i] > proc_adc_off_p[i]) {proc_data[i] = proc_adc_laser_p[i] - proc_adc_off_p[i];} else {proc_data[i] = 0;}
    if (proc_data[i] > max_val) {max_val = proc_data[i]; max_pos = i;}//поиск максимума
    //if ((data_adc_off2[i] > proc_adc_off_p[i]) && ((data_adc_off2[i] - proc_adc_off_p[i]) > 100)) {off_flag = 1;}
  }
  
  if (max_pos < 20) 
    AN_WIDTH = AN_WIDTH_LOW;
  else 
    AN_WIDTH = AN_WIDTH_HIGH;
    
#ifndef REFL_FILTER
    if (max_pos < AN_WIDTH) {max_pos = AN_WIDTH;}
    if (max_pos > (127-AN_WIDTH)) {max_pos = (127-AN_WIDTH);}
    start_pos = max_pos - AN_WIDTH;
    stop_pos =  max_pos + AN_WIDTH;
#endif
    
#ifdef REFL_FILTER

    if ((max_pos < AN_WIDTH) || (max_pos > (127-AN_WIDTH)))
    {
      //невозможно запустить фильтр
      if (max_pos < AN_WIDTH) {max_pos = AN_WIDTH;}
      if (max_pos > (127-AN_WIDTH)) {max_pos = (127-AN_WIDTH);}
      start_pos = max_pos - AN_WIDTH;
      stop_pos =  max_pos + AN_WIDTH;
    }
    else
    { 
     
      start_pos = max_pos - AN_WIDTH;
      stop_pos =  max_pos + AN_WIDTH;
      
      for (i=(start_pos+1);i<(stop_pos-1);i++)
      {
        
        delta1 = (int16_t)proc_data[i] - (int16_t)proc_data[i-1];
        delta2 = (int16_t)proc_data[i+1] - (int16_t)proc_data[i];
        
        if ((delta1 > 0) && (delta2 < 0) && (i != max_pos))
        {
          //найден максимум
          if (fmax_val < proc_data[i]) {fmax_val = proc_data[i];}
        }
        
      }
      val1 = max_val/15;
      val2 = max_val*2/3;
      
      if ((fmax_val > val1) && (fmax_val < val2)) {off_flag = 1;}
    }
    
#endif
    
    //субпиксельный расчет максимума
    for (i = start_pos;i<stop_pos;i++)
    {
      summ = summ + i * proc_data[i];
      summ2 = summ2 + proc_data[i];				
    }
    
    if (summ2 > 0) 
      tmp_centroid = ((summ*100) / summ2);
    
    /*
    if (max_val < 50)  {tmp_centroid = 1;}//защита от слишком слабых сигналов
    if (max_pos < 6)   {tmp_centroid = 2;}//защита от слишком слабых сигналов
    if (max_pos > 126) {tmp_centroid = 3;}//защита от неправильного вычисления на близких расстояниях
    */

    if (off_flag == 1) {tmp_centroid = tmp_centroid + 20000;}
    
    result.max_val = max_val;
    result.centroid = tmp_centroid;
  
    return result;
}