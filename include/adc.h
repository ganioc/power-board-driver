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
#define MAX_CURRENT         3.0
#define MAX_VOLTAGE         (3.0*0.05)

#define RT_ADC_ID           0
#define L5_ADC_ID           1
#define L4_ADC_ID           2
#define L3_ADC_ID           3
#define L2_ADC_ID           4
#define L1_ADC_ID           5
#define VOUT_ADC_ID         6
#define VOUT_12V_ADC_ID     7
#define VIN_ADC_ID          8
#define IN_ADC_ID           9
#define TEMP_ADC_ID         10
#define VREF_ADC_ID         11

#define ADC_VREF                         (3.3)
#define ADC_TEMP_BASE                    (1.26)
#define ADC_TEMP_SLOPE                   (-0.00423)


/* functions */
void handle_adc_value(uint16_t nums, uint16_t times, uint16_t table[ADC_REPEAT_TIMES][ADC_CHANNEL_NUM]);
float get_VIN_VAL(uint16_t val);
float get_VOUT_12V_VAL(uint16_t val);
float get_TEMP_VAL(uint16_t val);
float get_VREF_VAL(uint16_t val);


#endif /* INCLUDE_ADC_H_ */
