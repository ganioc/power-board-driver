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

void update_state(uint16_t hword){
	if(hword & VOUT_POS == 0){
		sys_state.vout_mode = VOUT_MODE_12V;
	}else{
		sys_state.vout_mode = VOUT_MODE_19V;
	}
	if(hword & POWER1_POS == 0){
		sys_state.power1_on = POWER_OFF;
	}else{
		sys_state.power1_on = POWER_ON;
	}
	if(hword & POWER2_POS == 0){
		sys_state.power2_on = POWER_OFF;
	}else{
		sys_state.power2_on = POWER_ON;
	}
	if(hword & POWER3_POS == 0){
		sys_state.power3_on = POWER_OFF;
	}else{
		sys_state.power3_on = POWER_ON;
	}
	if(hword & POWER4_POS == 0){
		sys_state.power4_on = POWER_OFF;
	}else{
		sys_state.power4_on = POWER_ON;
	}
	if(hword & POWER5_POS == 0){
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
	if (strcmp(tag_buffer, "VCCOUT") == 0) {
		printf("%d\r\n", sys_state.vout);
	} else if (strcmp(tag_buffer, "ECHO") == 0) {
		printf("%d\r\n", sys_state.echo_en);
	} else {
		printf("ERROR\r\n");
	}

}
void handle_write_command(uint8_t *tag_buffer, uint8_t tag_index,
		uint8_t *value_buffer, uint8_t value_index) {
	if (tag_index == 0 || value_index == 0) {
		printf("ERROR\r\n");
		return;
	}
	if (strcmp(tag_buffer, "VCCOUT") == 0) {
		if (value_buffer[0] == '0') {
			sys_state.vout = 0;
		} else {
			sys_state.vout = 1;
		}
		printf("OK\r\n");
	} else if (strcmp(tag_buffer, "ECHO") == 0) {
		if (value_buffer[0] == '0') {
			sys_state.echo_en = 0;
		} else {
			sys_state.echo_en = 1;
		}
		printf("OK\r\n");
	} else {
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
