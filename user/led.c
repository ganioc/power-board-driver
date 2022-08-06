/*
 * led.c
 *
 *  Created on: 2022 Aug 6
 *      Author: spikey
 */
#include "at32f421_board.h"


void led_blue_on(void){
	at32_led_on(LED4);
}
void led_blue_off(void){
	at32_led_off(LED4);
}
void led_blue_toggle(void){
	at32_led_toggle(LED4);
}
void led_green_on(void){
	at32_led_on(LED3);
}
void led_green_off(void){
	at32_led_off(LED3);
}
void led_green_toggle(void){
	at32_led_toggle(LED3);
}
void led_red_on(void){
	at32_led_on(LED2);
}
void led_red_off(void){
	at32_led_off(LED2);
}
void led_red_toggle(void){
	at32_led_toggle(LED2);
}
