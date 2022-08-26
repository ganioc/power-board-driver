/*
 * eeprom.h
 * Simulation of eeprom,
 *
 *  Created on: 2022 Aug 7
 *      Author: spikey
 */

#ifndef INCLUDE_EEPROM_H_
#define INCLUDE_EEPROM_H_

#include "at32f421_board.h"
//
//#define MAGIC_1 'R'
//#define MAGIC_2 'U'
//#define MAGIC_3 'F'
//#define MAGIC_4 'F'

#define INFO_ONE       1

// output mode, 0 - 12V, 1 - 19V,
#define VOUT_POS       1
#define POWER2_POS     1<<1
#define POWER3_POS     1<<2
#define POWER4_POS     1<<3
#define POWER5_POS     1<<4
#define POWER1_POS     1<<5

#define INFO_DEFAULT 0b111110

//#define TEST_BUFFER_SIZE                 0x4
//#define TEST_FLASH_ADDRESS_START         (0x08000000 + 1024 * 62)

#define EEPROM_FLASH_ADDRESS_START         (0x08000000 + 1024 * 63)

#define SECTOR_SIZE                      1024

int read_from_EEPROM(uint16_t * info_hword);
int write_to_EEPROM(uint16_t info_hword);

//void flash_read(uint32_t read_addr, uint16_t *p_buffer, uint16_t num_read);
//void flash_write(uint32_t write_addr, uint16_t *p_buffer, uint16_t num_write);

void check_EEPROM();


#endif /* INCLUDE_EEPROM_H_ */
