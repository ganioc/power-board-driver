/*
 * eeprom.c
 *
 *  Created on: 2022 Aug 7
 *      Author: spikey
 */

#include "eeprom.h"


uint16_t flash_buf[SECTOR_SIZE / 2]={0};
uint16_t data_buf[5]={0};


void flash_read(uint32_t read_addr, uint16_t *p_buffer, uint16_t num_read);
void flash_write_nocheck(uint32_t write_addr, uint16_t *p_buffer, uint16_t num_write);
void flash_write(uint32_t write_addr,uint16_t *p_Buffer, uint16_t num_write);


/**
  * @brief  read data using halfword mode
  * @param  read_addr: the address of reading
  * @param  p_buffer: the buffer of reading data
  * @param  num_read: the number of reading data
  * @retval none
  */
void flash_read(uint32_t read_addr, uint16_t *p_buffer, uint16_t num_read)
{
  uint16_t i;
  for(i = 0; i < num_read; i++)
  {
    p_buffer[i] = *(uint16_t*)(read_addr);
    read_addr += 2;
  }
}

/**
  * @brief  write data using halfword mode without checking
  * @param  write_addr: the address of writing
  * @param  p_buffer: the buffer of writing data
  * @param  num_write: the number of writing data
  * @retval none
  */
void flash_write_nocheck(uint32_t write_addr, uint16_t *p_buffer, uint16_t num_write)
{
  uint16_t i;
  for(i = 0; i < num_write; i++)
  {
    flash_halfword_program(write_addr, p_buffer[i]);
    write_addr += 2;
  }
}

/**
  * @brief  write data using halfword mode with checking
  * @param  write_addr: the address of writing
  * @param  p_buffer: the buffer of writing data
  * @param  num_write: the number of writing data
  * @retval none
  */
void flash_write(uint32_t write_addr, uint16_t *p_buffer, uint16_t num_write)
{
  uint32_t offset_addr;
  uint32_t sector_position;
  uint16_t sector_offset;
  uint16_t sector_remain;
  uint16_t i;

  flash_unlock();
  offset_addr = write_addr - FLASH_BASE;
  sector_position = offset_addr / SECTOR_SIZE;
  sector_offset = (offset_addr % SECTOR_SIZE) / 2;
  sector_remain = SECTOR_SIZE / 2 - sector_offset;
  if(num_write <= sector_remain)
    sector_remain = num_write;
  while(1)
  {
    flash_read(sector_position * SECTOR_SIZE + FLASH_BASE, flash_buf, SECTOR_SIZE / 2);
    for(i = 0; i < sector_remain; i++)
    {
      if(flash_buf[sector_offset + i] != 0xFFFF)
        break;
    }
    if(i < sector_remain)
    {
      flash_sector_erase(sector_position * SECTOR_SIZE + FLASH_BASE);
      for(i = 0; i < sector_remain; i++)
      {
        flash_buf[i + sector_offset] = p_buffer[i];
      }
      flash_write_nocheck(sector_position * SECTOR_SIZE + FLASH_BASE, flash_buf, SECTOR_SIZE / 2);
    }
    else
    {
      flash_write_nocheck(write_addr, p_buffer, sector_remain);
    }
    if(num_write == sector_remain)
      break;
    else
    {
      sector_position++;
      sector_offset = 0;
      p_buffer += sector_remain;
      write_addr += (sector_remain * 2);
      num_write -= sector_remain;
      if(num_write > (SECTOR_SIZE / 2))
        sector_remain = SECTOR_SIZE / 2;
      else
        sector_remain = num_write;
    }
  }
  flash_lock();
}

int write_to_EEPROM(uint16_t info_hword){
	data_buf[0] = 0x5255;
	data_buf[1] = 0x4646;
	data_buf[2] = info_hword;

	flash_write(EEPROM_FLASH_ADDRESS_START, data_buf, 3);

	update_state(info_hword);

	return 0;
}

int read_from_EEPROM(uint16_t * info_hword){
	flash_read(EEPROM_FLASH_ADDRESS_START, data_buf, 3);

	printf("-> %x %x\r\n",data_buf[0],data_buf[1]);

	if(data_buf[0] != 0x5255 || data_buf[1] != 0x4646){
		return -1;
	}
	*info_hword = data_buf[2];
	return 0;
}

void check_EEPROM() {
	uint16_t temp = 0, rtn;
	rtn = read_from_EEPROM(&temp);
	if(rtn != 0){
		/* write default value */
		write_to_EEPROM(INFO_DEFAULT);

	}else{
		update_state(temp);
	}
}
