/*
 * parser.c
 *
 *  Created on: 2022 Aug 5
 *      Author: spikey
 */

#include "parser.h"
#include "command.h"

uint8_t state = STATE_DUMMY;

uint8_t value_buffer[VALUE_SIZE];
uint8_t value_index;
uint8_t tag_buffer[TAG_SIZE];
uint8_t tag_index;
uint8_t type;

uint8_t AT_in_handling = 0;
uint8_t AT_in_handling_counter = 0;

void clear_value_buffer(){
	for(int i=0; i< VALUE_SIZE; i++){
		value_buffer[i] = 0;
	}
	value_index = 0;
}
void clear_tag_buffer(){
	for(int i=0; i< TAG_SIZE; i++){
		tag_buffer[i] = 0;
	}
	tag_index = 0;
}
void enter_handling(){
	AT_in_handling = 1;
	AT_in_handling_counter = 0;
}
void exit_handling(){
	AT_in_handling = 0;
//	AT_reset_state();
	state = STATE_DUMMY;
}
void AT_reset_state(){
	state = STATE_DUMMY;
}

void parse(uint8_t ch){
	switch(state){
	case STATE_DUMMY:
		if(ch == 'A'){
			state = STATE_HEAD;
			type = AT_TYPE_READ;
			clear_value_buffer();
			clear_tag_buffer();

			enter_handling();
		}
		break;
	case STATE_HEAD:
		if(ch == 'T'){
			state = STATE_OPERATOR;
		}else{
			printf("ERROR\r\n");
			exit_handling();
		}
		break;
	case STATE_OPERATOR:
		if(ch == '+'){
			state = STATE_TAG;
		}else if(ch == '\r'){
			state = STATE_TAIL;
		}
		break;
	case STATE_TAG:
		if(ch == '?'){
			state = STATE_QUERY;
			type = AT_TYPE_READ;
		}else if(ch == '='){
			state = STATE_VALUE;
			type = AT_TYPE_WRITE;
		}else if(ch == '\r' || ch == '\n'){
			printf("ERROR\r\n");
			exit_handling();
		}
		else{
			tag_buffer[tag_index++] = ch;
			if(tag_index >= TAG_SIZE){
				printf("ERROR\r\n");
				exit_handling();
			}
		}
		break;
	case STATE_QUERY:
		if(ch == '\r'){
			state = STATE_TAIL;
		}else{
			printf("ERROR\r\n");
			exit_handling();
		}
		break;
	case STATE_VALUE:
		if(ch == '\r'){
			state = STATE_TAIL;
		}else{
			value_buffer[value_index++] = ch;
			if(value_index >= VALUE_SIZE){
				printf("ERROR\r\n");
				exit_handling();
			}
		}
		break;
	case STATE_TAIL:
		if(ch == '\n'){
			handle_command(type, tag_buffer, tag_index, value_buffer, value_index);
		}else{
			printf("ERROR\r\n");
		}
		exit_handling();
		break;
//	case STATE_ERROR:
//
//		break;
	default:
		break;
	}

}

