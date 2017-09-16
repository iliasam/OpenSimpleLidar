#ifndef _IMAGE_PROCESSING_H
#define _IMAGE_PROCESSING_H

#include "stm32f0xx.h"

//Single distance measurement result
typedef struct
{
  uint16_t centroid;
  uint8_t low_power;
  uint16_t max_val;
} centroid_result_type;

centroid_result_type find_centroid(void);

#endif
