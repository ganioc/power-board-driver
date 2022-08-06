/*
 * adc.c
 *
 *  Created on: 2022 Aug 6
 *      Author: spikey
 */
#include "adc.h"

void handle_adc_value(uint16_t nums, uint16_t times, uint16_t **table) {
	for (int j = 0; j < nums; j++) {
		uint16_t temp = 0;
		for (int i = 0; i < times; i++) {
			printf("adc1-%d-%d\r\n", i, table[i][j]);
			temp += table[i][j];
		}

	}
}

