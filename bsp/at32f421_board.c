/**
  **************************************************************************
  * @file     at32f421_board.c
  * @version  v2.0.6
  * @date     2022-05-20
  * @brief    set of firmware functions to manage leds and push-button.
  *           initialize delay function.
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

#include "at32f421_board.h"

/** @addtogroup AT32F421_board
  * @{
  */

/** @defgroup BOARD
  * @brief onboard periph driver
  * @{
  */

/* delay macros */
#define STEP_DELAY_MS                    50

/* at-start led resouce array */
gpio_type *led_gpio_port[LED_NUM]        = {LED2_GPIO, LED3_GPIO, LED4_GPIO};
uint16_t led_gpio_pin[LED_NUM]           = {LED2_PIN, LED3_PIN, LED4_PIN};
crm_periph_clock_type led_gpio_crm_clk[LED_NUM] = {LED2_GPIO_CRM_CLK, LED3_GPIO_CRM_CLK, LED4_GPIO_CRM_CLK};

/* usart1 rx buffer */
uint8_t usart1_rx_buffer[USART1_RX_BUFFER_SIZE];
uint16_t usart1_rx_counter = 0;
uint16_t usart1_rx_ptr = 0;
uint8_t version_buffer[]={'2','.','0', 0};

/* adc1 buffer */
__IO uint16_t adc1_ordinary_valuetab[ADC_REPEAT_TIMES][ADC_CHANNEL_NUM] = {0};
__IO uint16_t dma_trans_complete_flag = 0;
uint16_t dma_in_working=0;

__IO uint16_t adc1_ordinary_value = 0;

/* timer1 */
crm_clocks_freq_type crm_clocks_freq_struct = {0};

/* delay variable */
static __IO uint32_t fac_us;
static __IO uint32_t fac_ms;

/* support printf function, usemicrolib is unnecessary */
#if (__ARMCC_VERSION > 6000000)
  __asm (".global __use_no_semihosting\n\t");
  void _sys_exit(int x)
  {
    x = x;
  }
  /* __use_no_semihosting was requested, but _ttywrch was */
  void _ttywrch(int ch)
  {
    ch = ch;
  }
  FILE __stdout;
#else
 #ifdef __CC_ARM
  #pragma import(__use_no_semihosting)
  struct __FILE
  {
    int handle;
  };
  FILE __stdout;
  void _sys_exit(int x)
  {
    x = x;
  }
  /* __use_no_semihosting was requested, but _ttywrch was */
  void _ttywrch(int ch)
  {
    ch = ch;
  }
 #endif
#endif

#if defined (__GNUC__) && !defined (__clang__)
  // #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
  #define PUTCHAR_PROTOTYPE int putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

/**
  * @brief  retargets the c library printf function to the usart.
  * @param  none
  * @retval none
  */
PUTCHAR_PROTOTYPE
{
  while(usart_flag_get(PRINT_UART, USART_TDBE_FLAG) == RESET);
  usart_data_transmit(PRINT_UART, ch);
  return ch;
}
/* For GCC compiler revise _write() function for printf functionality */
int _write(int file, char *ptr, int len)
{
    int i;
//    file = file;
    for (i = 0; i < len; i++)
    {
    	while(usart_flag_get(PRINT_UART, USART_TDBE_FLAG) == RESET);
    	usart_data_transmit(PRINT_UART,ptr[i]);
    }
    return len;
}
/**
  * @brief  initialize uart
  * @param  baudrate: uart baudrate
  * @retval none
  */
void uart_print_init(uint32_t baudrate)
{
  gpio_init_type gpio_init_struct;

  /* enable the uart and gpio clock */
  crm_periph_clock_enable(PRINT_UART_CRM_CLK, TRUE);
  crm_periph_clock_enable(PRINT_UART_TX_GPIO_CRM_CLK, TRUE);

  gpio_default_para_init(&gpio_init_struct);

  /* configure the uart tx pin, and rx pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = PRINT_UART_TX_PIN | PRINT_UART_RX_PIN;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(PRINT_UART_TX_GPIO, &gpio_init_struct);

  /* configure uart1 io mux */
  gpio_pin_mux_config(PRINT_UART_TX_GPIO, PRINT_UART_TX_PIN_SOURCE, PRINT_UART_TX_PIN_MUX_NUM);
  gpio_pin_mux_config(PRINT_UART_RX_GPIO, PRINT_UART_RX_PIN_SOURCE, PRINT_UART_RX_PIN_MUX_NUM);

  /* configure uart param */
  usart_init(PRINT_UART, baudrate, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_parity_selection_config(PRINT_UART, USART_PARITY_NONE);

  usart_transmitter_enable(PRINT_UART, TRUE);
  usart_receiver_enable(PRINT_UART, TRUE);

  usart_interrupt_enable(PRINT_UART, USART_RDBF_INT, TRUE);

  usart_enable(PRINT_UART, TRUE);

  /* config  usart1 nvic interrupt */
  nvic_irq_enable(USART1_IRQn, 0, 0);
}
void adc_gpio_config(void) {
	gpio_init_type gpio_initstructure;
	crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

	gpio_default_para_init(&gpio_initstructure);
	gpio_initstructure.gpio_mode = GPIO_MODE_ANALOG;
	gpio_initstructure.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1 | GPIO_PINS_2
			|GPIO_PINS_3 | GPIO_PINS_4 | GPIO_PINS_5 |GPIO_PINS_6 | GPIO_PINS_7 ;
	gpio_init(GPIOA, &gpio_initstructure);

	crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

	gpio_default_para_init(&gpio_initstructure);
	gpio_initstructure.gpio_mode = GPIO_MODE_ANALOG;
	gpio_initstructure.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1 ;
	gpio_init(GPIOB, &gpio_initstructure);

}
void adc_dma_config(void)
{
  dma_init_type dma_init_struct;
  crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
  nvic_irq_enable(DMA1_Channel1_IRQn, 0, 0);

  dma_reset(DMA1_CHANNEL1);
  dma_default_para_init(&dma_init_struct);

  dma_init_struct.buffer_size = ADC_CHANNEL_NUM* ADC_REPEAT_TIMES;
  dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
  dma_init_struct.memory_base_addr = (uint32_t)adc1_ordinary_valuetab;
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
  dma_init_struct.memory_inc_enable = TRUE;
  dma_init_struct.peripheral_base_addr = (uint32_t)&(ADC1->odt);
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
  dma_init_struct.peripheral_inc_enable = FALSE;
  dma_init_struct.priority = DMA_PRIORITY_HIGH;
  dma_init_struct.loop_mode_enable = FALSE;
  dma_init(DMA1_CHANNEL1, &dma_init_struct);

  dma_interrupt_enable(DMA1_CHANNEL1, DMA_FDT_INT, TRUE);
  dma_channel_enable(DMA1_CHANNEL1, TRUE);
}

void adc_config(void)
{
  adc_base_config_type adc_base_struct;
  crm_periph_clock_enable(CRM_ADC1_PERIPH_CLOCK, TRUE);
  crm_adc_clock_div_set(CRM_ADC_DIV_6);

  adc_base_default_para_init(&adc_base_struct);
  adc_base_struct.sequence_mode = TRUE;
  adc_base_struct.repeat_mode = TRUE;
  adc_base_struct.data_align = ADC_RIGHT_ALIGNMENT;
  adc_base_struct.ordinary_channel_length = ADC_CHANNEL_NUM;
  adc_base_config(ADC1, &adc_base_struct);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_0, 1, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_1, 2, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_2, 3, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_3, 4, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_4, 5, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_5, 6, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_6, 7, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_7, 8, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_8, 9, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_9, 10, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_16, 11, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_17, 12, ADC_SAMPLETIME_239_5);

  adc_ordinary_conversion_trigger_set(ADC1, ADC12_ORDINARY_TRIG_SOFTWARE, TRUE);
  // DMA mode!
  adc_dma_mode_enable(ADC1, TRUE);

  adc_enable(ADC1, TRUE);
  adc_calibration_init(ADC1);
  while(adc_calibration_init_status_get(ADC1));
  adc_calibration_start(ADC1);
  while(adc_calibration_status_get(ADC1));
}
void timer1_config(void) {
	/* get system clock */
	crm_clocks_freq_get(&crm_clocks_freq_struct);
	/* enable tmr1 clock */
	crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);

	/* tmr1 configuration */
	/* time base configuration */
	/* systemclock/12000/10000 = 1hz */
	tmr_base_init(TMR1, TIMER1_COUNT_VAL, (crm_clocks_freq_struct.ahb_freq / 10000) - 1);
	tmr_cnt_dir_set(TMR1, TMR_COUNT_UP);

	/* overflow interrupt enable */
	tmr_interrupt_enable(TMR1, TMR_OVF_INT, TRUE);
	/* tmr1 overflow interrupt nvic init */
	nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
	nvic_irq_enable(TMR1_BRK_OVF_TRG_HALL_IRQn, 0, 0);
	/* enable tmr1 */
	tmr_counter_enable(TMR1, TRUE);

}
/* Only check one ADC source,  */
void dma_config1(){
	  dma_init_type dma_init_struct;

	  crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
	  dma_reset(DMA1_CHANNEL1);
	  dma_default_para_init(&dma_init_struct);
	  dma_init_struct.buffer_size = ADC_CHANNEL_NUM*ADC_REPEAT_TIMES;
	  dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
	  dma_init_struct.memory_base_addr = (uint32_t)adc1_ordinary_valuetab;
	  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
	  dma_init_struct.memory_inc_enable = TRUE;
	  dma_init_struct.peripheral_base_addr = (uint32_t)&(ADC1->odt);
	  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
	  dma_init_struct.peripheral_inc_enable = FALSE;
	  dma_init_struct.priority = DMA_PRIORITY_HIGH;
	  dma_init_struct.loop_mode_enable = TRUE;
	  dma_init(DMA1_CHANNEL1, &dma_init_struct);

	  dma_channel_enable(DMA1_CHANNEL1, TRUE);
}
void adc_config1(void)
{
  adc_base_config_type adc_base_struct;
  crm_periph_clock_enable(CRM_ADC1_PERIPH_CLOCK, TRUE);
  crm_adc_clock_div_set(CRM_ADC_DIV_16);

  adc_base_default_para_init(&adc_base_struct);

  adc_base_struct.sequence_mode = TRUE;
  adc_base_struct.repeat_mode = TRUE;
  adc_base_struct.data_align = ADC_RIGHT_ALIGNMENT;
  adc_base_struct.ordinary_channel_length = ADC_CHANNEL_NUM;
  adc_base_config(ADC1, &adc_base_struct);

  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_0, 1, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_1, 2, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_2, 3, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_3, 4, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_4, 5, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_5, 6, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_6, 7, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_7, 8, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_8, 9, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_9, 10, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_16, 11, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_17, 12, ADC_SAMPLETIME_239_5);

  adc_ordinary_conversion_trigger_set(ADC1, ADC12_ORDINARY_TRIG_SOFTWARE, TRUE);
  adc_dma_mode_enable(ADC1, TRUE);
  adc_tempersensor_vintrv_enable(TRUE);

  adc_enable(ADC1, TRUE);
  adc_calibration_init(ADC1);
  while(adc_calibration_init_status_get(ADC1));
  adc_calibration_start(ADC1);
  while(adc_calibration_status_get(ADC1));
}
/**
  * @brief  board initialize interface init led and button
  * @param  none
  * @retval none
  */
void at32_board_init()
{
  /* initialize delay function */
  delay_init();

  /* configure led in at_start_board */
  at32_led_init(LED2);
  at32_led_init(LED3);
  at32_led_init(LED4);
  at32_led_off(LED2);
  at32_led_off(LED3);
  at32_led_off(LED4);

  /* configure button in at_start board */
//  at32_button_init();



//  timer1_config();

}

/**
  * @brief  configure button gpio
  * @param  button: specifies the button to be configured.
  * @retval none
  */
void at32_button_init(void)
{
  gpio_init_type gpio_init_struct;

  /* enable the button clock */
  crm_periph_clock_enable(USER_BUTTON_CRM_CLK, TRUE);

  /* set default parameter */
  gpio_default_para_init(&gpio_init_struct);

  /* configure button pin as input with pull-up/pull-down */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = USER_BUTTON_PIN;
  gpio_init_struct.gpio_pull = GPIO_PULL_DOWN;
  gpio_init(USER_BUTTON_PORT, &gpio_init_struct);
}

/**
  * @brief  returns the selected button state
  * @param  none
  * @retval the button gpio pin value
  */
uint8_t at32_button_state(void)
{
  return gpio_input_data_bit_read(USER_BUTTON_PORT, USER_BUTTON_PIN);
}

/**
  * @brief  returns which button have press down
  * @param  none
  * @retval the button have press down
  */
button_type at32_button_press()
{
  static uint8_t pressed = 1;
  /* get button state in at_start board */
  if((pressed == 1) && (at32_button_state() != RESET))
  {
    /* debounce */
    pressed = 0;
    delay_ms(10);
    if(at32_button_state() != RESET)
      return USER_BUTTON;
  }
  else if(at32_button_state() == RESET)
  {
    pressed = 1;
  }
  return NO_BUTTON;
}

/**
  * @brief  configure led gpio
  * @param  led: specifies the led to be configured.
  * @retval none
  */
void at32_led_init(led_type led)
{
  gpio_init_type gpio_init_struct;

  /* enable the led clock */
  crm_periph_clock_enable(led_gpio_crm_clk[led], TRUE);

  /* set default parameter */
  gpio_default_para_init(&gpio_init_struct);

  /* configure the led gpio */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_pins = led_gpio_pin[led];
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(led_gpio_port[led], &gpio_init_struct);
}
void at32_io_init(crm_periph_clock_type crm_clock,
		uint16_t pin,gpio_type* port ){
	gpio_init_type gpio_init_struct;
	/* enable the gpio clock */
	crm_periph_clock_enable(crm_clock, TRUE);
	/* set default parameter */
	gpio_default_para_init(&gpio_init_struct);

	/* configure the led gpio */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = pin;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(port, &gpio_init_struct);
}


/**
  * @brief  turns selected led on.
  * @param  led: specifies the led to be set on.
  *   this parameter can be one of following parameters:
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  * @retval none
  */
void at32_led_on(led_type led)
{
  if(led > (LED_NUM - 1))
    return;
  if(led_gpio_pin[led])
    led_gpio_port[led]->scr = led_gpio_pin[led];
}

/**
  * @brief  turns selected led off.
  * @param  led: specifies the led to be set off.
  *   this parameter can be one of following parameters:
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  * @retval none
  */
void at32_led_off(led_type led)
{
  if(led > (LED_NUM - 1))
    return;
  if(led_gpio_pin[led])
    led_gpio_port[led]->clr = led_gpio_pin[led];
}

/**
  * @brief  turns selected led toggle.
  * @param  led: specifies the led to be set off.
  *   this parameter can be one of following parameters:
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  * @retval none
  */
void at32_led_toggle(led_type led)
{
  if(led > (LED_NUM - 1))
    return;
  if(led_gpio_pin[led])
    led_gpio_port[led]->odt ^= led_gpio_pin[led];
}

/**
  * @brief  initialize delay function
  * @param  none
  * @retval none
  */
void delay_init()
{
  /* configure systick */
  systick_clock_source_config(SYSTICK_CLOCK_SOURCE_AHBCLK_NODIV);
  fac_us = system_core_clock / (1000000U);
  fac_ms = fac_us * (1000U);
}

/**
  * @brief  inserts a delay time.
  * @param  nus: specifies the delay time length, in microsecond.
  * @retval none
  */
void delay_us(uint32_t nus)
{
  uint32_t temp = 0;
  SysTick->LOAD = (uint32_t)(nus * fac_us);
  SysTick->VAL = 0x00;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;
  do
  {
    temp = SysTick->CTRL;
  }while((temp & 0x01) && !(temp & (1 << 16)));

  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
  SysTick->VAL = 0x00;
}

/**
  * @brief  inserts a delay time.
  * @param  nms: specifies the delay time length, in milliseconds.
  * @retval none
  */
void delay_ms(uint16_t nms)
{
  uint32_t temp = 0;
  while(nms)
  {
    if(nms > STEP_DELAY_MS)
    {
      SysTick->LOAD = (uint32_t)(STEP_DELAY_MS * fac_ms);
      nms -= STEP_DELAY_MS;
    }
    else
    {
      SysTick->LOAD = (uint32_t)(nms * fac_ms);
      nms = 0;
    }
    SysTick->VAL = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    do
    {
      temp = SysTick->CTRL;
    }while((temp & 0x01) && !(temp & (1 << 16)));

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0x00;
  }
}

/**
  * @brief  inserts a delay time.
  * @param  sec: specifies the delay time, in seconds.
  * @retval none
  */
void delay_sec(uint16_t sec)
{
  uint16_t index;
  for(index = 0; index < sec; index++)
  {
    delay_ms(500);
    delay_ms(500);
  }
}

void en_gpio_on(gpio_type* port, uint16_t pin){
	port->scr = pin;
}
void en_gpio_off(gpio_type* port, uint16_t pin){
	port->clr = pin;
}
void en_gpio_toggle(gpio_type* port, uint16_t pin){
	port->odt ^= pin;
}

void en_gpio_config(void) {
	at32_io_init(POWER2_EN_CRM_CLK, POWER2_EN_PIN, POWER2_EN_GPIO);
	en_gpio_off(POWER2_EN_GPIO, POWER2_EN_PIN);

	at32_io_init(POWER3_EN_CRM_CLK, POWER3_EN_PIN, POWER3_EN_GPIO);
	en_gpio_off(POWER3_EN_GPIO, POWER3_EN_PIN);

	at32_io_init(POWER4_EN_CRM_CLK, POWER4_EN_PIN, POWER4_EN_GPIO);
	en_gpio_off(POWER4_EN_GPIO, POWER4_EN_PIN);

	at32_io_init(POWER5_EN_CRM_CLK, POWER5_EN_PIN, POWER5_EN_GPIO);
	en_gpio_off(POWER5_EN_GPIO, POWER5_EN_PIN);

	at32_io_init(VOUT_IO_CRM_CLK, VOUT_IO_PIN, VOUT_IO_GPIO);
	en_gpio_off(VOUT_IO_GPIO, VOUT_IO_PIN);

	at32_io_init(POWER1_EN_CRM_CLK, POWER1_EN_PIN, POWER1_EN_GPIO);
	en_gpio_off(POWER1_EN_GPIO, POWER1_EN_PIN);

	at32_io_init(POWER_ON_CRM_CLK, POWER_ON_PIN, POWER_ON_GPIO);
	en_gpio_off(POWER_ON_GPIO, POWER_ON_PIN);

	// CLK control
	at32_io_init(CLK_CRM_CLK, CLK_PIN, CLK_GPIO);
	en_gpio_off(CLK_GPIO, CLK_PIN);

}

void wdt_config(void){
	if (crm_flag_get(CRM_WDT_RESET_FLAG) != RESET) {
		/* reset from wdt */
		crm_flag_clear(CRM_WDT_RESET_FLAG);
	}
	/* disable register write protection */
	wdt_register_write_enable(TRUE);

	/* set the wdt divider value */
	wdt_divider_set(WDT_CLK_DIV_8);
	/* set reload value

	 timeout = reload_value * (divider / lick_freq )    (s)

	 lick_freq    = 40000 Hz
	 divider      = 4
	 reload_value = 3000

	 timeout = 3000 * (4 / 40000 ) = 0.3s = 300ms
	 timeout = 3000 * ( 8/ 4000 ) = 0.6s = 600ms
	 */
	wdt_reload_value_set(3000 - 1);

	/* enable wdt */
	wdt_enable();
}
void gen_gpio_config() {
	gpio_init_type gpio_init_struct;

	/* enable the led clock */
	crm_periph_clock_enable(CLK_CRM_CLK, TRUE);

	/* set default parameter */
	gpio_default_para_init(&gpio_init_struct);

	/* configure the led gpio */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = CLK_PIN;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(CLK_GPIO, &gpio_init_struct);
}

/**
  * @}
  */

/**
  * @}
  */

