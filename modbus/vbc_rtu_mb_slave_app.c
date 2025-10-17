/**
  ******************************************************************************
  * @file       : modbus
  * @author     : Andrew
  * @date       : 2025-10-17 15:21
  * @brief      : 
  * 
  * @copyright  : Copyright 2025 Zhang Xu. All rights reserved.
  ******************************************************************************
  */


#include "board.h"
#include "vbc_rtu_mb.h"
#include "vbc_rtu_virtualIO.h"
#include "stdio.h"
#include "string.h"

static int vbc_modbus_rtu_slave_callback(vbc_rtu_mb_t* smb, int function_code, int addr, int num, void* read_write_data)
{
	int rc = 0;
	switch (function_code)
	{
		case MODBUS_FC_READ_HOLDING_REGISTERS:
		{
			if ((0 <= addr) && (addr < 125))
			{
				rc = vbc_rtu_read_hold_regs(addr, num, read_write_data);
			}
		}
			break;
		case MODBUS_FC_WRITE_SINGLE_REGISTER:
		{
			if ((0 <= addr) && (addr < 125))
			{
				rc = vbc_rtu_write_hold_regs(addr, num, read_write_data);
			}
		}
			break;
	}
	if (rc < 0)
	{
		rt_kprintf("callback fail %d\n", rc);
	}
	return rc;
}

static int uart_rts(int on)
{
	return 0;
}

static vbc_rtu_mb_t modbus_rtu_slave = {0};

static void vbc_vbc_modbus_rtu_slave_thread_entry(void* param)
{
	int rc = 0;
	vbc_rtu_mb_t* smb_slave = param;
	vbc_rtu_mb_init(smb_slave, MODBUS_CORE_RTU, vbc_rtu_mb_port_rtdevice_create(VBC_RTU_UART_DEVICE_NAME));
	struct serial_configure serial_config;
	serial_config.baud_rate = BAUD_RATE_9600;
	serial_config.data_bits = DATA_BITS_8;
	serial_config.stop_bits = STOP_BITS_1;
	serial_config.bufsz = 512;
	serial_config.parity = PARITY_NONE;
	vbc_rtu_mb_rtu_set_serial_config(smb_slave, &serial_config);
	vbc_rtu_mb_rtu_set_serial_rts(smb_slave, uart_rts);
	vbc_rtu_mb_rtu_set_oflag(smb_slave, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
	vbc_rtu_mb_set_slave(smb_slave, VBC_RTU_SLAVE_ADDR);
	vbc_rtu_mb_connect(smb_slave);

	while (1)
	{
		rc = vbc_rtu_mb_slave_wait_handle(smb_slave, vbc_modbus_rtu_slave_callback, MODBUS_WAIT_FOREVER);
		if (rc > 0)
		{
			continue;
		}
		else
		{
			vbc_rtu_mb_error_recovery(smb_slave);
		}
	}
}

int vbc_modbus_rtu_slave_thread(void)
{
	rt_thread_t thread_id;

	thread_id = rt_thread_create("RTU", vbc_vbc_modbus_rtu_slave_thread_entry, &modbus_rtu_slave, VBC_RTU_THREAD_STACK_SIZE, VBC_RTU_THREAD_PRIORITY, VBC_RTU_THREAD_TICKS);
	if (thread_id != RT_NULL)
	{
		rt_thread_startup(thread_id);
	}
	else
	{
		goto __exit;
	}

	return RT_EOK;
	__exit:
	if (thread_id)
		rt_thread_delete(thread_id);
	return RT_ERROR;
}

