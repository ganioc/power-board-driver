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
extern uint16_t dma_trans_complete_flag;

uint8_t g_speed = FAST;

//void button_exint_init(void);
//void button_isr(void);

/**
  * @brief  configure button exint
  * @param  none
  * @retval none
  */
void button_exint_init(void)
{
  exint_init_type exint_init_struct;

  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_SCFG_PERIPH_CLOCK, TRUE);

  scfg_exint_line_config(SCFG_PORT_SOURCE_GPIOA, SCFG_PINS_SOURCE0);

  exint_default_para_init(&exint_init_struct);
  exint_init_struct.line_enable = TRUE;
  exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
  exint_init_struct.line_select = EXINT_LINE_0;
  exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
  exint_init(&exint_init_struct);

  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  nvic_irq_enable(EXINT1_0_IRQn, 0, 0);
}

/**
  * @brief  button handler function
  * @param  none
  * @retval none
  */
void button_isr(void)
{
  /* delay 5ms */
  delay_ms(5);

  /* clear interrupt pending bit */
  exint_flag_clear(EXINT_LINE_0);

  /* check input pin state */
  if(SET == gpio_input_data_bit_read(USER_BUTTON_PORT, USER_BUTTON_PIN))
  {
    if(g_speed == SLOW)
      g_speed = FAST;
    else
      g_speed = SLOW;
  }
}

/**
  * @brief  exint0 interrupt handler
  * @param  none
  * @retval none
  */
void EXINT1_0_IRQHandler(void)
{
  button_isr();
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config();
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  uart_print_init(115200);

  at32_board_init();

//  button_exint_init();
  printf("\r\n================================\r\n");
  printf("Power Board Controller\r\n");

  printf("version:%s\r\n",version_buffer);
  printf("================================\r\n");

  delay_ms(1000);

  adc_ordinary_software_trigger_enable(ADC1, TRUE);

  while(1)
  {
//	  printf("90\r\n");
//	  delay_ms(500);
//	  while(usart_flag_get(PRINT_UART, USART_TDBE_FLAG) == RESET);
//	  usart_data_transmit(PRINT_UART, 'a');

//    at32_led_toggle(LED2);
//    delay_ms(g_speed * DELAY);
//      at32_led_toggle(LED3);
//    delay_ms(g_speed * DELAY);
//    at32_led_toggle(LED4);
//    delay_ms(g_speed * DELAY);
//      if(usart_flag_get(USART1, USART_RDBF_FLAG) != RESET){
//    	  usart1_rx_buffer[0] = usart_data_receive(USART1);
//    	  at32_led_toggle(LED4);
//      }
	  /* Only the rx buffer is never overflow */
	  if(usart1_rx_ptr != usart1_rx_counter){
		  printf("%c\r\n",usart1_rx_buffer[usart1_rx_ptr]);
		  parse(usart1_rx_buffer[usart1_rx_ptr]);

		  usart1_rx_ptr++;

		  if(usart1_rx_ptr >= USART1_RX_BUFFER_SIZE){
			  usart1_rx_ptr = 0;
		  }
	  }

	  if(dma_trans_complete_flag == 1){
		  // print out the adc result
		  for(int i=0; i< ADC_REPEAT_TIMES; i++){
			  for(int j=0; j< ADC_CHANNEL_NUM; j++){
				  printf("adc1-%d-%d\r\n",i, adc1_ordinary_valuetab[i][j]);
			  }
		  }
		  dma_trans_complete_flag = 0;
	  }
  }
}

/**
  * @}
  */

/**
  * @}
  */
