/**
  ******************************************************************************
  * @file       : applications
  * @author     : Andrew
  * @date       : 2025-10-17 13:35
  * @brief      : 
  * 
  * @copyright  : Copyright 2025 Zhang Xu. All rights reserved.
  ******************************************************************************
  */

#include <rtthread.h>
#include <rtdevice.h>

#define DBG_TAG "u_port"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#include "u_port.h"

rt_base_t DO_PINS[] = {DO1, DO2, DO3, DO4, DO5, DO6, DO7, DO8,
					   DO9, DO16, DO15, DO14, DO13, DO10, DO11, DO12};

volatile rt_uint16_t PINS_WORD = 0x0000;

void u_port_init(void)
{
	for (int i = 0; i < sizeof(DO_PINS) / sizeof(DO_PINS[0]); i++)
	{
		rt_pin_mode(DO_PINS[i], PIN_MODE_OUTPUT);
		rt_pin_write(DO_PINS[i], PIN_LOW);
	}

	LOG_I("Port initialized, all outputs set LOW.");
}

void u_port_set_pin(rt_base_t pin, rt_ssize_t value)
{
	if (pin < 0 || pin >= sizeof(DO_PINS) / sizeof(DO_PINS[0]))
	{
		LOG_D("Pin %d set to %d", pin, value);
		return;
	}

	rt_pin_write(DO_PINS[pin], value);
}