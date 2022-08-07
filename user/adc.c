/*
 * adc.c
 *
 *  Created on: 2022 Aug 6
 *      Author: spikey
 */
#include "adc.h"
#include "command.h"
#include "en.h"
#include "led.h"

extern struct SYSTEM_STATE sys_state;

float get_TEMP_VAL(uint16_t val){
	return (ADC_TEMP_BASE - (float)val * ADC_VREF / 4096) / ADC_TEMP_SLOPE + 25.0;
}
float get_VREF_VAL(uint16_t val){
	return (float)(1.2 * 4095) / val;
}
float get_VIN_VAL(uint16_t val){
	return ((float)val * ADC_VREF / 4096)* 23.0;
}
float get_VOUT_12V_VAL(uint16_t val){
	return ((float)val * ADC_VREF / 4096)* 23.0;
}

void check_RT_ADC(uint16_t val){
//	printf("RT_ADC: %d\r\n", val);
}
void check_L5_ADC(uint16_t val){
	float v = get_VIN_VAL(val);
//	printf("L5_ADC: %d %.2f\r\n", val,v);
//	if(v > MAX_VOLTAGE){
//
//	}
}
void check_L4_ADC(uint16_t val){
	float v = get_VIN_VAL(val);
//	printf("L4_ADC: %d %.2f\r\n", val,v);
}
void check_L3_ADC(uint16_t val){
	float v = get_VIN_VAL(val);
//	printf("L3_ADC: %d %.2f\r\n", val,v);
}
void check_L2_ADC(uint16_t val){
	float v = get_VIN_VAL(val);
//	printf("L2_ADC: %d %.2f\r\n", val,v);
}
void check_L1_ADC(uint16_t val){
	float v = get_VIN_VAL(val);
//	printf("L1_ADC: %d %.2f\r\n", val,v);
}
void check_VOUT_ADC(uint16_t val){
	float v = get_VOUT_12V_VAL(val);
//	printf("VOUT_ADC: %d %.2f\r\n", val, v);
}
void check_VOUT_12V_ADC(uint16_t val){
	float v = get_VOUT_12V_VAL(val);
//	printf("VOUT_12V_ADC: %d %.2f\r\n", val, v);

}
void check_VIN_ADC(uint16_t val){
	float v = get_VIN_VAL(val);
//	printf("VIN_ADC: %d %.2f\r\n", val, v);

	if( v <= DC_MAX && v >= DC_MIN){
		en_POWER_ON();
		en_POWER1_ON();
		en_POWER2_ON();
		en_POWER3_ON();
		en_POWER4_ON();
		en_POWER5_ON();

		led_red_shine_slow();

	}else{
		en_POWER_OFF();
		en_POWER1_OFF();
		en_POWER2_OFF();
		en_POWER3_OFF();
		en_POWER4_OFF();
		en_POWER5_OFF();

		led_red_shine_fast();
	}
}
/* Not used */
void check_IN_ADC(uint16_t val){
//	printf("IN_ADC: %d\r\n", val);
}
void check_TEMP_ADC(uint16_t val){
//	printf("TEMP_ADC: %d %.2f\r\n", val, get_TEMP_VAL(val));

}
void check_VREF_ADC(uint16_t val){
//	printf("VREF_ADC: %d %.2f\r\n", val, get_VREF_VAL(val));
}

void check_adc_value(uint16_t nums,uint16_t adc[]){
	check_RT_ADC(adc[RT_ADC_ID]);
	check_L5_ADC(adc[L5_ADC_ID]);
	check_L4_ADC(adc[L4_ADC_ID]);
	check_L3_ADC(adc[L3_ADC_ID]);
	check_L2_ADC(adc[L1_ADC_ID]);
	check_L1_ADC(adc[L1_ADC_ID]);
	check_VOUT_ADC(adc[VOUT_ADC_ID]);
	check_VOUT_12V_ADC(adc[VOUT_12V_ADC_ID]);
	check_VIN_ADC(adc[VIN_ADC_ID]);
	check_IN_ADC(adc[IN_ADC_ID]);
	check_TEMP_ADC(adc[TEMP_ADC_ID]);
	check_VREF_ADC(adc[VREF_ADC_ID]);
}

void handle_adc_value(uint16_t nums, uint16_t times, uint16_t table[ADC_REPEAT_TIMES][ADC_CHANNEL_NUM]) {
	for (int j = 0; j < nums; j++) {
		uint16_t temp = 0;
		for (int i = 0; i < times; i++) {
//			printf("adc1-%d-%d\r\n", i, table[i][j]);
			temp += table[i][j];
		}
		sys_state.adc_val[j] = temp/times;
	}
	check_adc_value(nums, sys_state.adc_val);
}

