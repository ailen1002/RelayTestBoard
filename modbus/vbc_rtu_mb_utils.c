/**
  ******************************************************************************
  * @file       : modbus
  * @author     : Andrew
  * @date       : 2025-10-17 15:54
  * @brief      : 
  * 
  * @copyright  : Copyright 2025 Zhang Xu. All rights reserved.
  ******************************************************************************
  */

#include "vbc_rtu_mb_utils.h"
#include "vbc_rtu_mb_base.h"
#include "stdint.h"
#include "string.h"

uint16_t vbc_rtu_mb_crc16(uint8_t* buffer, uint16_t buffer_length)
{
	uint16_t CRC = 0XFFFF;
	uint16_t CRC_count = 0;
	uint16_t i = 0;
	for (CRC_count = 0; CRC_count < buffer_length; CRC_count++)
	{
		CRC = CRC ^ *(buffer + CRC_count);
		for (i = 0; i < 8; i++)
		{
			if (CRC & 1)
			{
				CRC >>= 1;
				CRC ^= 0xA001;
			}
			else
			{
				CRC >>= 1;
			}
		}
	}
	return CRC;
}

int vbc_rtu_dio_get_val(uint8_t* array, uint16_t index)
{
	uint8_t offset_bit = (index & 0x07);   //(index%8);  //
	uint8_t offset_arr = (index >> 0x03);  //(index/8);  //
	return (array[offset_arr] & (0x01 << offset_bit)) ? 1 : 0;
}

void vbc_rtu_dio_set_val(uint8_t* array, uint16_t index, uint16_t status)
{
	uint8_t offset_bit = (index & 0x07);   //(index%8);  //
	uint8_t offset_arr = (index >> 0x03);  //(index/8);  //
	if (status)
	{
		array[offset_arr] |= (0x01 << offset_bit);
	}
	else
	{
		array[offset_arr] &= ~(0x01 << offset_bit);
	}
}

uint16_t vbc_rtu_aio_get_val(uint16_t* array, uint16_t index)
{
	uint8_t* buf = (uint8_t*)array;
	uint16_t status = (buf[(index * 2)]) + (buf[(index * 2) + 1] << 8);
	return status;
}

void vbc_rtu_aio_set_val(uint16_t* array, uint16_t index, uint16_t status)
{
	uint8_t* buf = (uint8_t*)array;
	buf[(index * 2)] = (status & 0x00FF);
	buf[(index * 2) + 1] = (status >> 8);
}

int vbc_rtu_mb_check_addr_num(uint8_t function, uint16_t address, uint16_t num)
{
	switch (function)
	{
		case MODBUS_FC_READ_HOLDING_COILS:
		case MODBUS_FC_READ_INPUTS_COILS:
		{
			if ((0 < num) && (num <= VBC_MODBUS_MAX_READ_BITS))  // 读取到的线圈数量大于0且小于2000个
			{
				return 1;
			}
		}
			break;
		case MODBUS_FC_READ_HOLDING_REGISTERS:
		case MODBUS_FC_READ_INPUT_REGISTERS:
		{
			if ((0 < num) && (num <= VBC_MODBUS_MAX_READ_REGISTERS))  // 读取到的寄存器数量大于0且小于125个
			{
				return 1;
			}
		}
			break;
		case MODBUS_FC_WRITE_MULTIPLE_COILS:
		{
			if ((0 < num) && (num <= VBC_MODBUS_MAX_WRITE_BITS))  // 读取到的寄存器数量大于0且小于1968个
			{
				return 1;
			}
		}
			break;
		case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
		{
			if ((0 < num) && (num <= VBC_MODBUS_MAX_WRITE_REGISTERS))  // 读取到的寄存器数量大于0且小于123个
			{
				return 1;
			}
		}
			break;
		case MODBUS_FC_WRITE_SINGLE_COIL:
		case MODBUS_FC_WRITE_SINGLE_REGISTER:
		{
			return 1;
		}
	}
	return 0;
}

void vbc_rtu_mb_byte_copy(uint8_t* des, uint8_t* src, int num)
{
	while (num--)
	{
		*des = *src;
		des++;
		src++;
	}
}

void vbc_rtu_mb_coil_h2m(uint8_t* des, uint8_t* src, int coil_num)
{
	int num = (coil_num / 8) + ((coil_num % 8) ? 1 : 0);
	do
	{
		*des = *src;
		des++;
		src++;
	} while (--num);
}

void vbc_rtu_mb_coil_m2h(uint8_t* des, uint8_t* src, int coil_num)
{
	int num = (coil_num / 8) + ((coil_num % 8) ? 1 : 0);
	do
	{
		*des = *src;
		des++;
		src++;
	} while (--num);
}

/*
 *
Modbus register data is in big end mode
host uint16_t >> mb_reg
*/
void vbc_rtu_mb_reg_h2m(void* dest_mb_reg, void* source_host, int reg_num)
{
	uint8_t* source_host_ptr = source_host;
	uint8_t* dest_mb_ptr = dest_mb_reg;
	uint8_t temp_mb_low = 0;
	uint8_t temp_mb_high = 0;
	do
	{
		temp_mb_high = source_host_ptr[1];  // mb high byte
		temp_mb_low = source_host_ptr[0];   // mb low byte

		dest_mb_ptr[0] = temp_mb_high;
		dest_mb_ptr[1] = temp_mb_low;

		source_host_ptr += 2;
		dest_mb_ptr += 2;
	} while (--reg_num);
}
/*
mb_reg >> host uint16_t
*/
void vbc_rtu_mb_reg_m2h(void* dest_host, void* source_mb_reg, int reg_num)
{
	uint8_t* source_mb_ptr = source_mb_reg;
	uint8_t* dest_host_ptr = dest_host;
	uint8_t temp_mb_low = 0;
	uint8_t temp_mb_high = 0;
	do
	{
		temp_mb_high = source_mb_ptr[0];  // mb high byte
		temp_mb_low = source_mb_ptr[1];   // mb low byte

		dest_host_ptr[0] = temp_mb_low;
		dest_host_ptr[1] = temp_mb_high;

		source_mb_ptr += 2;
		dest_host_ptr += 2;
	} while (--reg_num);
}