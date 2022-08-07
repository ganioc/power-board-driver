/*
 * led.h
 *
 *  Created on: 2022 Aug 6
 *      Author: spikey
 */

#ifndef INCLUDE_LED_H_
#define INCLUDE_LED_H_

#define LED_SHINE_SLOW  10
#define LED_SHINE_FAST  1


void led_blue_on(void);
void led_blue_off(void);
void led_green_on(void);
void led_green_off(void);
void led_red_on(void);
void led_red_off(void);

void led_red_shine_slow();
void led_red_shine_fast();
void led_red_shine_off();
void led_red_shine();

void led_blue_shine_slow();
void led_blue_shine_fast();
void led_green_shine_slow();
void led_green_shine_fast();


#endif /* INCLUDE_LED_H_ */
