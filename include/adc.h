/*
 * adc.h
 *
 *  Created on: 2022 Aug 6
 *      Author: spikey
 */

#include "at32f421_board.h"

#ifndef INCLUDE_ADC_H_
#define INCLUDE_ADC_H_

#define VCC_MCU             3.3
#define DC_MAX              60.0
#define DC_MIN              8.0
#define DEC_MAX             4096
#define VIN_ADC_DIVISION    0.04


/* functions */
void handle_adc_value(uint16_t nums, uint16_t times, uint16_t(*)[ADC_CHANNEL_NUM] table);


#endif /* INCLUDE_ADC_H_ */
