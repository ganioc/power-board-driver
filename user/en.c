/*
 * en.c
 *
 *  Created on: 2022 Aug 7
 *      Author: spikey
 */
#include "en.h"
#include "command.h"

extern struct SYSTEM_STATE sys_state;

void en_POWER_ON(){
	en_gpio_on(POWER_ON_GPIO, POWER_ON_PIN);

}
void en_POWER_OFF(){
	en_gpio_off(POWER_ON_GPIO, POWER_ON_PIN);
}

void en_POWER1_ON(){
	if(sys_state.power1_on == POWER_OFF){
		en_POWER1_OFF();
		return;
	}

	en_gpio_off(POWER1_EN_GPIO, POWER1_EN_PIN);
	if( sys_state.vout_mode == VOUT_MODE_12V){
		en_gpio_off(VOUT_IO_GPIO, VOUT_IO_PIN);
	}else{
		en_gpio_on(VOUT_IO_GPIO, VOUT_IO_PIN);
	}
}
void en_POWER1_OFF(){
	en_gpio_on(POWER1_EN_GPIO, POWER1_EN_PIN);
}


void en_POWER2_ON(){
	if(sys_state.power2_on == POWER_OFF){
		en_POWER2_OFF();
		return;
	}
	en_gpio_on(POWER2_EN_GPIO, POWER2_EN_PIN);
}
void en_POWER2_OFF(){

	en_gpio_off(POWER2_EN_GPIO, POWER2_EN_PIN);
}
void en_POWER3_ON(){
	if(sys_state.power3_on == POWER_OFF){
		en_POWER3_OFF();
		return;
	}
	en_gpio_on(POWER3_EN_GPIO, POWER3_EN_PIN);
}
void en_POWER3_OFF(){
	en_gpio_off(POWER3_EN_GPIO, POWER3_EN_PIN);
}
void en_POWER4_ON(){
	if(sys_state.power4_on == POWER_OFF){
		en_POWER4_OFF();
		return;
	}
	en_gpio_on(POWER4_EN_GPIO, POWER4_EN_PIN);
}
void en_POWER4_OFF(){
	en_gpio_off(POWER4_EN_GPIO, POWER4_EN_PIN);
}
void en_POWER5_ON(){
	if(sys_state.power5_on == POWER_OFF){
		en_POWER5_OFF();
		return;
	}
	en_gpio_on(POWER5_EN_GPIO, POWER5_EN_PIN);
}
void en_POWER5_OFF(){
	en_gpio_off(POWER5_EN_GPIO, POWER5_EN_PIN);
}
