/*
 * command.h
 *
 *  Created on: 2022 Aug 5
 *      Author: spikey
 */

#ifndef INCLUDE_COMMAND_H_
#define INCLUDE_COMMAND_H_

#include "at32f421_board.h"

struct SYSTEM_STATE{
	uint8_t vout;
	uint8_t echo_en;

	uint16_t adc_val[ADC_CHANNEL_NUM];

};

void handle_command(uint8_t type,
		uint8_t *tag_buffer,
		uint8_t tag_index,
		uint8_t* value_buffer,
		uint8_t value_index);

#endif /* INCLUDE_COMMAND_H_ */
