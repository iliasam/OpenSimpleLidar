#ifndef _LASER_H
#define _LASER_H

typedef enum
{
  PHASE_WAIT = 0,
  PHASE_1,
  PHASE_2,
  PHASE_3,
  PHASE_DONE,
}meas_status_type;

void meas_handler(void);
void stop_capture(void);
void refresh_sync_timer(uint16_t time);
void switch_adc_buf(void);
void process_handler(void);

void test_func(void);


#endif











