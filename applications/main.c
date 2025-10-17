/*
 * Copyright (c) 2006-2025, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-10-16     RT-Thread    first version
 */

#include <rtthread.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "u_port.h"

int main(void)
{
	LOG_I("Resistance Test Board and Digital Output Board");

	u_port_init();

	extern int vbc_modbus_rtu_slave_thread(void);

	if (vbc_modbus_rtu_slave_thread() != RT_EOK)
	{
		LOG_E("Failed to start Modbus RTU slave thread.");
		return -1;
	}

    while (1)
    {
		for (int i = 0; i < 16; i++)
		{
			if ((PINS_WORD >> i) & 0x01)
			{
				rt_pin_write(DO_PINS[i], PIN_HIGH);
				LOG_I("DO%d set to HIGH", i + 1);
			}
			else
			{
				rt_pin_write(DO_PINS[i], PIN_LOW);
				LOG_I("DO%d set to LOW", i + 1);
			}
		}

		rt_thread_mdelay(50);
    }
}
