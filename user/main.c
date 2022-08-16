/**
 **************************************************************************
 * @file     main.c
 * @version  v2.0.6
 * @date     2022-05-20
 * @brief    main program
 **************************************************************************
 *                       Copyright notice & Disclaimer
 *
 * The software Board Support Package (BSP) that is made available to
 * download from Artery official website is the copyrighted work of Artery.
 * Artery authorizes customers to use, copy, and distribute the BSP
 * software and its related documentation for the purpose of design and
 * development in conjunction with Artery microcontrollers. Use of the
 * software is governed by this copyright notice and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
 * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
 * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
 * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
 * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
 *
 **************************************************************************
 */

#include <stdio.h>

#include "at32f421_board.h"
#include "at32f421_clock.h"
#include "parser.h"
#include "adc.h"
#include "eeprom.h"

/** @addtogroup AT32F421_periph_template
 * @{
 */

/** @addtogroup 421_LED_toggle LED_toggle
 * @{
 */

#define DELAY                            100
#define FAST                             1
#define SLOW                             4

extern uint8_t usart1_rx_buffer[];
extern uint16_t usart1_rx_counter;
extern uint16_t usart1_rx_ptr;
extern uint8_t version_buffer[];
extern uint16_t adc1_ordinary_valuetab[ADC_REPEAT_TIMES][ADC_CHANNEL_NUM];


uint8_t g_speed = FAST;


//uint16_t buffer_write[TEST_BUFFER_SIZE];
//uint16_t buffer_read[TEST_BUFFER_SIZE];


/**
 * @brief  main function.
 * @param  none
 * @retval none
 */
int main(void) {
	system_clock_config();
	nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

	uart_print_init(UART1_BAUDRATE);

	at32_board_init();

	printf("\r\n================================\r\n");
	printf("Power Board Controller\r\n");

	printf("version:%s\r\n", version_buffer);
	printf("================================\r\n");

	timer1_config();
	adc_gpio_config();

	dma_config1();
	adc_config1();

	en_gpio_config();

//	write_to_EEPROM(0x0);

	check_EEPROM();

	// printf("internal_temperature_sensor \r\n");
	adc_ordinary_software_trigger_enable(ADC1, TRUE);
	wdt_config();

	while (1) {
		/* reload wdt counter */
		wdt_counter_reload();

		/* Only the rx buffer is never overflow */
		if (usart1_rx_ptr != usart1_rx_counter) {
			// printf("%c\r\n", usart1_rx_buffer[usart1_rx_ptr]);
			parse(usart1_rx_buffer[usart1_rx_ptr]);

			usart1_rx_ptr++;

			if (usart1_rx_ptr >= USART1_RX_BUFFER_SIZE) {
				usart1_rx_ptr = 0;
			}
		}

		if(dma_flag_get(DMA1_FDT1_FLAG) != RESET){
			dma_flag_clear(DMA1_FDT1_FLAG);
			handle_adc_value(ADC_CHANNEL_NUM, ADC_REPEAT_TIMES, adc1_ordinary_valuetab);
		}

	}
}

/**
 * @}
 */

/**
 * @}
 */
