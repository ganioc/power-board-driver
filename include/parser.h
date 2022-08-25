/*
 * parser.h
 *
 *  Created on: 2022 Aug 5
 *      Author: spikey
 */

#ifndef INCLUDE_PARSER_H_
#define INCLUDE_PARSER_H_

#include "at32f421_board.h"

#define STATE_DUMMY   0
#define STATE_HEAD    1
#define STATE_OPERATOR 11
#define STATE_QUERY    12
#define STATE_TAG     2
#define STATE_VALUE   3
#define STATE_TAIL    4
#define STATE_END     5
#define STATE_ERROR   6

#define VALUE_SIZE    12
#define TAG_SIZE      12

#define AT_TYPE_READ  0
#define AT_TYPE_WRITE 1

#define AT_IN_HANDLING_COUNTER_MAX  5

void parse(uint8_t ch);
void AT_reset_state();


#endif /* INCLUDE_PARSER_H_ */
