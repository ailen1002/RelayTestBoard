/**
  ******************************************************************************
  * @file       : modbus
  * @author     : Andrew
  * @date       : 2025-10-17 15:53
  * @brief      : 
  * 
  * @copyright  : Copyright 2025 Zhang Xu. All rights reserved.
  ******************************************************************************
  */

#include "vbc_rtu_virtualIO.h"
#include "board.h"
#include "vbc_rtu_mb.h"
#include "stdio.h"
#include "string.h"
#include "u_port.h"

#define DBG_TAG "RTU"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#define HOLD_REG_MAX 125
static uint16_t vbc_hold_regs[HOLD_REG_MAX] = {0};

int vbc_rtu_read_hold_regs(uint16_t addr, uint16_t num, uint16_t* buffer)
{
	uint8_t index = 0, base = 0;
	uint16_t io_start = 0, io_end = 0;

	for (index = 0; index < HOLD_REG_MAX; index++)
	{
		// 动态更新参数
		if (index < addr)
		{
			continue;
		}
		if (index > (addr + num))
		{
			break;
		}

		if (index >= 0 && index < 16)
		{
			// Read the bit status
			vbc_hold_regs[index] = (PINS_WORD >> index) & 0x01;
		}
		else
		{
			vbc_hold_regs[index] = 0;
		}
	}

	if (addr > 124)
	{
		addr = addr - 125;
	}
	index = 0;
	if ((0 <= addr) && (addr < HOLD_REG_MAX))
	{
		io_start = addr;
		if ((addr + num) > (HOLD_REG_MAX))
		{
			io_end = HOLD_REG_MAX;
		}
		else
		{
			io_end = addr + num;
		}
		while (io_start < io_end)
		{
			buffer[index] = vbc_hold_regs[io_start];
			io_start++;
			index++;
		}
	}
	return index;
}

int vbc_rtu_write_hold_regs(uint16_t addr, uint16_t num, uint16_t* buffer)
{
	rt_uint16_t base = 0, value = 0;

	if ((0 <= addr) && (addr < HOLD_REG_MAX))
	{
		value = vbc_rtu_aio_get_val(buffer, 0);

		LOG_I("Write Reg Addr:%d Val:%d", addr, value);

		if (addr >= 0 && addr < 16)
		{
			if (value == 0)
			{
				PINS_WORD &= ~(1 << addr);  // Clear the bit
			}
			else
			{
				PINS_WORD |= (1 << addr);  // Set the bit
			}
		}
		return num;
	}
}