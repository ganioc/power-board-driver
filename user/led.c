/*
 * led.c
 *
 *  Created on: 2022 Aug 6
 *      Author: spikey
 */
#include "at32f421_board.h"
#include "led.h"

uint16_t led_red_counter=0;
uint16_t led_blue_counter=0;
uint16_t led_green_counter=0;

uint16_t led_red_timer=0;
uint16_t led_red_timer_en = 0;

uint16_t led_blue_timer=0;
uint16_t led_blue_timer_en = 0;

uint16_t led_green_timer=0;
uint16_t led_green_timer_en = 0;

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

void led_red_shine_slow(){
	led_red_timer_en = 1;
	led_red_timer = LED_SHINE_SLOW;
}

void led_red_shine_fast(){
	led_red_timer_en = 1;
	led_red_timer = LED_SHINE_FAST;
}
void led_red_shine_off(){
	led_red_timer_en = 0;
}
void led_red_shine(){
	if(led_red_timer_en){
		led_red_counter++;
		if(led_red_counter >= led_red_timer){
			led_red_counter = 0;
			led_red_toggle();
		}
	}
}
void led_blue_shine(){
	if(led_blue_timer_en){
		led_blue_on();
	}
}
void led_green_shine(){
	if(led_green_timer_en){
		led_green_on();
	}
}
void led_blue_shine_slow(){
	led_blue_timer_en = 1;
	led_blue_timer = LED_SHINE_SLOW;
}
void led_blue_shine_fast(){
	led_blue_timer_en = 1;
	led_blue_timer = LED_SHINE_FAST;
}
void led_blue_shine_off(){
	led_blue_timer_en = 0;
}
void led_green_shine_slow(){
	led_green_timer_en = 1;
	led_green_timer = LED_SHINE_SLOW;
}
void led_green_shine_fast(){
	led_green_timer_en = 1;
	led_green_timer = LED_SHINE_FAST;
}
void led_green_shine_off(){
	led_green_timer_en = 0;
}

