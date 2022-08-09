/*
 * command.c
 *
 *  Created on: 2022 Aug 5
 *      Author: spikey
 */

#include <stdio.h>
#include <string.h>

#include "command.h"
#include "parser.h"
#include "eeprom.h"
#include "adc.h"

struct SYSTEM_STATE sys_state = {
		vout:    0,
		echo_en: 0,
		vout_mode: VOUT_MODE_12V,
		power1_on: POWER_ON,
		power2_on: POWER_ON,
		power3_on: POWER_ON,
		power4_on: POWER_ON,
		power5_on: POWER_ON,
		status:  0,
		adc_val: {0},
};
void backup_state(){
	uint16_t temp = 0;
	if(sys_state.vout_mode == VOUT_MODE_19V){
		temp += VOUT_POS;
	}
	if(sys_state.power1_on == 1){
		temp += POWER1_POS;
	}
	if(sys_state.power2_on == 1){
		temp += POWER2_POS;
	}
	if(sys_state.power3_on == 1){
		temp += POWER3_POS;
	}
	if(sys_state.power4_on == 1){
		temp += POWER4_POS;
	}
	if(sys_state.power5_on == 1){
		temp += POWER5_POS;
	}
	write_to_EEPROM(temp);
}
void update_state(uint16_t hword){
	if((hword & VOUT_POS) == 0){
		sys_state.vout_mode = VOUT_MODE_12V;
	}else{
		sys_state.vout_mode = VOUT_MODE_19V;
	}
	if((hword & POWER1_POS) == 0){
		sys_state.power1_on = POWER_OFF;
	}else{
		sys_state.power1_on = POWER_ON;
	}
	if((hword & POWER2_POS) == 0){
		sys_state.power2_on = POWER_OFF;
	}else{
		sys_state.power2_on = POWER_ON;
	}
	if((hword & POWER3_POS) == 0){
		sys_state.power3_on = POWER_OFF;
	}else{
		sys_state.power3_on = POWER_ON;
	}
	if((hword & POWER4_POS) == 0){
		sys_state.power4_on = POWER_OFF;
	}else{
		sys_state.power4_on = POWER_ON;
	}
	if((hword & POWER5_POS) == 0){
		sys_state.power5_on = POWER_OFF;
	}else{
		sys_state.power5_on = POWER_ON;
	}
}

void handle_read_command(uint8_t *tag_buffer, uint8_t tag_index) {
	if (tag_index == 0) {
		printf("OK\r\n");
		return;
	}
	if (strcmp(tag_buffer, "ECHO") == 0) {
		printf("%d\r\n", sys_state.echo_en);
	} else if (strcmp(tag_buffer, "POWER1") == 0) {
		printf("%d\r\n", sys_state.vout_mode);
	} else if (strcmp(tag_buffer, "GETVIN") == 0) {
		printf("%.2f\r\n", get_VIN_VAL(sys_state.adc_val[VIN_ADC_ID]));
	}else if (strcmp(tag_buffer, "GETV12") == 0) {
		printf("%.2f\r\n", get_VOUT_12V_VAL(sys_state.adc_val[VOUT_12V_ADC_ID]));
	} else if (strcmp(tag_buffer, "GETVOUT") == 0) {
		printf("%.2f\r\n", get_VOUT_12V_VAL(sys_state.adc_val[VOUT_ADC_ID]));
	} else if (strcmp(tag_buffer, "GETTEMP") == 0) {
		printf("%.2f\r\n", get_TEMP_VAL(sys_state.adc_val[TEMP_ADC_ID]));
	} else if (strcmp(tag_buffer, "GETVREF") == 0) {
		printf("%.2f\r\n", get_VREF_VAL(sys_state.adc_val[VREF_ADC_ID]));
	} else {
		printf("ERROR\r\n");
	}

}
void check_state_by_command(volatile uint8_t * s, uint8_t v){
	if(*s != v){
		*s = v;
		backup_state();
	}
}
void handle_write_command(uint8_t *tag_buffer, uint8_t tag_index,
		uint8_t *value_buffer, uint8_t value_index) {
	if (tag_index == 0 || value_index == 0) {
		printf("ERROR\r\n");
		return;
	}
	if (strcmp(tag_buffer, "ECHO") == 0) {
		if (value_buffer[0] == '0') {
			sys_state.echo_en = 0;
		} else {
			sys_state.echo_en = 1;
		}
		printf("OK\r\n");
	} else if(strcmp(tag_buffer, "OFFPOWER") == 0){
		switch(value_buffer[0]){
		case '1':
			check_state_by_command(&(sys_state.power1_on), POWER_OFF);
			break;
		case '2':
			check_state_by_command(&(sys_state.power2_on), POWER_OFF);
			break;
		case '3':
			check_state_by_command(&(sys_state.power3_on), POWER_OFF);
			break;
		case '4':
			check_state_by_command(&(sys_state.power4_on), POWER_OFF);
			break;
		case '5':
			check_state_by_command(&(sys_state.power5_on), POWER_OFF);
			break;
		default:
			printf("ERROR\r\n");
			return;
		}
		printf("OK\r\n");
	} else if(strcmp(tag_buffer, "ONPOWER") == 0){
		switch(value_buffer[0]){
		case '1':
			check_state_by_command(&(sys_state.power1_on), POWER_ON);
			break;
		case '2':
			check_state_by_command(&(sys_state.power2_on), POWER_ON);
			break;
		case '3':
			check_state_by_command(&(sys_state.power3_on), POWER_ON);
			break;
		case '4':
			check_state_by_command(&(sys_state.power4_on), POWER_ON);
			break;
		case '5':
			check_state_by_command(&(sys_state.power5_on), POWER_ON);
			break;
		default:
			printf("ERROR\r\n");
			return;
		}
		printf("OK\r\n");
	}else if(strcmp(tag_buffer, "GETPOWER") == 0){
		switch(value_buffer[0]){
		case '1':
			printf("%d\r\n", sys_state.power1_on);
			break;
		case '2':
			printf("%d\r\n", sys_state.power2_on);
			break;
		case '3':
			printf("%d\r\n", sys_state.power3_on);
			break;
		case '4':
			printf("%d\r\n", sys_state.power4_on);
			break;
		case '5':
			printf("%d\r\n", sys_state.power5_on);
			break;
		default:
			printf("ERROR\r\n");
			return;
		}
	}else if(strcmp(tag_buffer, "POWER1") == 0){
		switch(value_buffer[0]){
		case '0':
			check_state_by_command(&(sys_state.vout_mode),0);
			break;
		case '1':
			check_state_by_command(&(sys_state.vout_mode),1);
			break;
		default:
			printf("ERROR\r\n");
			return;
		}

		printf("OK\r\n");
	}
	else {
		printf("ERROR\r\n");
	}

}
void handle_command(uint8_t type, uint8_t *tag_buffer, uint8_t tag_index,
		uint8_t *value_buffer, uint8_t value_index) {
	if (type == AT_TYPE_READ) {
		handle_read_command(tag_buffer, tag_index);
	} else if (type == AT_TYPE_WRITE) {
		handle_write_command(tag_buffer, tag_index, value_buffer, value_index);
	} else {
		printf("ERROR\r\n");
	}
}
