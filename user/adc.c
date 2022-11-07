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


int average_index[ADC_CHANNEL_NUM]={0};
uint16_t average_val[ADC_CHANNEL_NUM][AVERAGE_TIMES];

static uint16_t calculate_adc_val(int channel, uint16_t new_val){

	average_val[channel][average_index[channel]] = new_val;

	average_index[channel]++;
	if(average_index[channel]>=AVERAGE_TIMES ){
		average_index[channel] = 0;
	}

	uint32_t sum = 0;
	for(int i=0; i< AVERAGE_TIMES; i++){
		sum += average_val[channel][i];
	}
	return sum/AVERAGE_TIMES;
}

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
/**
 * Minus v_in_adc voltage
 */
float get_CURRENT_VAL(uint16_t val){
	float v = (float)val * ADC_VREF / 4096;

	float v_inadc =  (float) sys_state.adc_val[IN_ADC_ID] * ADC_VREF / 4096;

	float i = (v - v_inadc)* 10.0;
	return i;
}
float get_CURRENT_VAL_80(uint16_t val){
	float v = (float)val * ADC_VREF / 4096;

	float v_inadc =  (float) sys_state.adc_val[IN_ADC_ID] * ADC_VREF / 4096;

	float i = (v - v_inadc)* 12.5;
	return i;
}
float get_TOTAL_CURRENT_VAL(uint16_t val){
	float v = (float) val * ADC_VREF / 4096;
	float i = v / 0.03;
	return i;
}
/**
 * 0:16   - 17
 * 28:31  - 4
 * 32:33  - 2
 * 79:87  - 9
 */
uint32_t get_UID(){
	uint32_t m1 = *(uint32_t  *)UID31_BASE;
	uint32_t m2 = *(uint32_t  *)UID63_BASE;
	uint32_t m3 = *(uint32_t  *)UID95_BASE;

	uint32_t m = (m1 & 0x1FFFF)|((m1 & 0xF0000000)>>9)|((m2 & 0x3) << 21)| ((m3 & 0xFF8000) << 8);

	return m;
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
	if( v >= 10.0){
		if(sys_state.vout_mode == VOUT_MODE_12V){
			led_green_on();
			led_blue_off();
		}else{
			led_blue_on();
			led_green_off();
		}
	}else{
		led_green_off();
		led_blue_off();
	}
}
void check_VOUT_12V_ADC(uint16_t val){
	float v = get_VOUT_12V_VAL(val);
//	printf("VOUT_12V_ADC: %d %.2f\r\n", val, v);

}
void check_VIN_ADC(uint16_t val){
	float v = get_VIN_VAL(val);
//	printf("VIN_ADC: %d %.2f\r\n", val, v);

	if( v <= DC_MAX && v >= DC_MIN){

		if( v < DC_OUT1_THRESHOLD){
			// voltage is not high enough to output Vout1, 19V
			sys_state.enable_vout1_19v = 0;
		}else{
			sys_state.enable_vout1_19v = 1;
		}

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

void check_adc_value(uint16_t nums, volatile uint16_t adc[]){
	check_RT_ADC(adc[RT_ADC_ID]);
	check_L5_ADC(adc[L5_ADC_ID]);
	check_L4_ADC(adc[L4_ADC_ID]);
	check_L3_ADC(adc[L3_ADC_ID]);
	check_L2_ADC(adc[L2_ADC_ID]);
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
		uint32_t temp = 0;
		for (int i = 0; i < times; i++) {
//			printf("adc1-%d-%d\r\n", i, table[i][j]);
			temp += table[i][j];
		}

		// sys_state.adc_val[j] = temp/times;
		sys_state.adc_val[j] = calculate_adc_val(j, temp/times);
	}
	check_adc_value(nums, sys_state.adc_val);
}

