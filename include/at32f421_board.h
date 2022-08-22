/**
  **************************************************************************
  * @file     at32f421_board.h
  * @version  v2.0.6
  * @date     2022-05-20
  * @brief    header file for at-start board. set of firmware functions to
  *           manage leds and push-button. initialize delay function.
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

#ifndef __AT32F421_BOARD_H
#define __AT32F421_BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdio.h"
#include "at32f421.h"

/** @addtogroup AT32F421_board
  * @{
  */

/** @addtogroup BOARD
  * @{
  */

/** @defgroup BOARD_pins_definition
  * @{
  */

/**
  * this header include define support list:
  * 1. at-start-f421 v1.x boards
  * if define AT_START_F421_V1, the header file support at-start-f421 v1.x board
  */

#if !defined (AT_START_F421_V1)
#error "please select first the board at-start device used in your application (in at32f421_board.h file)"
#endif

/******************** define led ********************/
typedef enum
{
  LED2                                   = 0,
  LED3                                   = 1,
  LED4                                   = 2
} led_type;

#define LED_NUM                          3

#if defined (AT_START_F421_V1)
#define LED2_PIN                         GPIO_PINS_6
#define LED2_GPIO                        GPIOB
#define LED2_GPIO_CRM_CLK                CRM_GPIOB_PERIPH_CLOCK

#define LED3_PIN                         GPIO_PINS_7
#define LED3_GPIO                        GPIOB
#define LED3_GPIO_CRM_CLK                CRM_GPIOB_PERIPH_CLOCK

#define LED4_PIN                         GPIO_PINS_8
#define LED4_GPIO                        GPIOB
#define LED4_GPIO_CRM_CLK                CRM_GPIOB_PERIPH_CLOCK
#endif


#define EN_IO_NUM                        7

#define POWER2_EN_PIN                    GPIO_PINS_5
#define POWER2_EN_GPIO                   GPIOB
#define POWER2_EN_CRM_CLK                CRM_GPIOB_PERIPH_CLOCK

#define POWER3_EN_PIN                    GPIO_PINS_4
#define POWER3_EN_GPIO                   GPIOB
#define POWER3_EN_CRM_CLK                CRM_GPIOB_PERIPH_CLOCK

#define POWER4_EN_PIN                    GPIO_PINS_3
#define POWER4_EN_GPIO                   GPIOB
#define POWER4_EN_CRM_CLK                CRM_GPIOB_PERIPH_CLOCK

#define POWER5_EN_PIN                    GPIO_PINS_15
#define POWER5_EN_GPIO                   GPIOA
#define POWER5_EN_CRM_CLK                CRM_GPIOA_PERIPH_CLOCK

#define VOUT_IO_PIN                      GPIO_PINS_12
#define VOUT_IO_GPIO                     GPIOA
#define VOUT_IO_CRM_CLK                  CRM_GPIOA_PERIPH_CLOCK

#define POWER1_EN_PIN                    GPIO_PINS_11
#define POWER1_EN_GPIO                   GPIOA
#define POWER1_EN_CRM_CLK                CRM_GPIOA_PERIPH_CLOCK

#define POWER_ON_PIN                     GPIO_PINS_8
#define POWER_ON_GPIO                    GPIOA
#define POWER_ON_CRM_CLK                 CRM_GPIOA_PERIPH_CLOCK

#define CLK_PIN                          GPIO_PINS_2
#define CLK_GPIO                         GPIOB
#define CLK_CRM_CLK                      CRM_GPIOB_PERIPH_CLOCK



/**************** define print uart ******************/
#define PRINT_UART                       USART1
#define PRINT_UART_CRM_CLK               CRM_USART1_PERIPH_CLOCK
#define PRINT_UART_TX_PIN                GPIO_PINS_9
#define PRINT_UART_TX_GPIO               GPIOA
#define PRINT_UART_RX_PIN                GPIO_PINS_10
#define PRINT_UART_RX_GPIO               GPIOA
#define PRINT_UART_TX_GPIO_CRM_CLK       CRM_GPIOA_PERIPH_CLOCK
#define PRINT_UART_TX_PIN_SOURCE         GPIO_PINS_SOURCE9
#define PRINT_UART_TX_PIN_MUX_NUM        GPIO_MUX_1
#define PRINT_UART_RX_PIN_SOURCE         GPIO_PINS_SOURCE10
#define PRINT_UART_RX_PIN_MUX_NUM        GPIO_MUX_1



/******************* define button *******************/
typedef enum
{
  USER_BUTTON                            = 0,
  NO_BUTTON                              = 1
} button_type;

#define USER_BUTTON_PIN                  GPIO_PINS_0
#define USER_BUTTON_PORT                 GPIOA
#define USER_BUTTON_CRM_CLK              CRM_GPIOA_PERIPH_CLOCK

/* About usart1 rx buffer */
#define USART1_RX_BUFFER_SIZE            256


/* About ADC pins */
#define ADC_CHANNEL_NUM                  12
// It is 2 before,
#define ADC_REPEAT_TIMES                 50

/* Timer1, 1s=9999 */
#define TIMER1_COUNT_VAL                 999

#define UART1_BAUDRATE                  115200

/**
  * @}
  */

/** @defgroup BOARD_exported_functions
  * @{
  */

/******************** functions ********************/
void at32_board_init(void);

/* led operation function */
void at32_led_init(led_type led);
void at32_led_on(led_type led);
void at32_led_off(led_type led);
void at32_led_toggle(led_type led);

/* button operation function */
void at32_button_init(void);
button_type at32_button_press(void);
uint8_t at32_button_state(void);

/* delay function */
void delay_init(void);
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);
void delay_sec(uint16_t sec);

/* printf uart init function */
void uart_print_init(uint32_t baudrate);

void adc_gpio_config();
void adc_dma_config();
void adc_config();

void timer1_config(void);

void dma_config1();
void adc_config1(void);

void en_gpio_config(void);
void en_gpio_on(gpio_type* port, uint16_t pin);
void en_gpio_off(gpio_type* port, uint16_t pin);
void en_gpio_toggle(gpio_type* port, uint16_t pin);

void wdt_config(void);
void gen_gpio_config();

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif

