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

struct SYSTEM_STATE sys_state = {
		vout:    0,
		echo_en: 0,
		vout_mode: VOUT_MODE_12V,
		status:  0,
		adc_val: {0},
};

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
