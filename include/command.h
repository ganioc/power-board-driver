/*
 * command.h
 *
 *  Created on: 2022 Aug 5
 *      Author: spikey
 */

#ifndef INCLUDE_COMMAND_H_
#define INCLUDE_COMMAND_H_

#include "at32f421_board.h"

#define SYS_STATUS_SHUTDOWN    0
#define SYS_STATUS_DC_IN       1
#define VOUT_MODE_12V          0
#define VOUT_MODE_19V          1
#define POWER_ON               1
#define POWER_OFF              0

struct SYSTEM_STATE{
	uint8_t vout;
	uint8_t echo_en;
	uint8_t vout_mode;
	uint8_t power1_on;
	uint8_t power2_on;
	uint8_t power3_on;
	uint8_t power4_on;
	uint8_t power5_on;
	uint16_t status;
	uint16_t adc_val[ADC_CHANNEL_NUM];


};

void handle_command(uint8_t type,
		uint8_t *tag_buffer,
		uint8_t tag_index,
		uint8_t* value_buffer,
		uint8_t value_index);

void update_state(uint16_t hword);

#endif /* INCLUDE_COMMAND_H_ */
