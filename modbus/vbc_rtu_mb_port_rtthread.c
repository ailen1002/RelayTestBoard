/**
  ******************************************************************************
  * @file       : modbus
  * @author     : Andrew
  * @date       : 2025-10-17 15:57
  * @brief      : 
  * 
  * @copyright  : Copyright 2025 Zhang Xu. All rights reserved.
  ******************************************************************************
  */

#include "vbc_rtu_mb_port_rtthread.h"
/*
 * mb on rtthread
 */
#if VBC_RTU_MODBUS_RTTHREAD
#include "vbc_rtu_mb_base.h"
#include "vbc_rtu_mb_core.h"
#include "vbc_rtu_mb_utils.h"

int _vbc_rtu_mb_debug(vbc_rtu_mb_t *smb, int level, const char *fmt, ...) {
	static char log_buf[256];
	rt_enter_critical();
	if (level <= smb->debug_level) {
		va_list args;
		va_start(args, fmt);
		rt_vsnprintf(log_buf, 256, fmt, args);
		va_end(args);
		rt_kputs(log_buf);
	}
	rt_exit_critical();
	return 0;
}
/*
 *vbc_rtu_mb_init
 */
int vbc_rtu_mb_init(vbc_rtu_mb_t *smb, uint8_t core_type, void *port) {
	vbc_rtu_mb_port_t *smb_port = port;
	if (smb && core_type && smb_port) {
		_vbc_rtu_mb_init(smb);
		if ((core_type == MODBUS_CORE_RTU))// check core type
		{
			smb->core = (vbc_rtu_mb_core_t *) &_vbc_rtu_mb_core;
		} else {
			return 0;
		}
		if ((smb_port->type == MODBUS_PORT_DEVICE) ||
			(smb_port->type == MODBUS_PORT_SOCKET)) {
			smb->port = smb_port;
			return 1;
		}
	}
	return 0;
}

vbc_rtu_mb_t *vbc_rtu_mb_create(uint8_t core_type, void *port) {
	vbc_rtu_mb_t *smb = rt_malloc_align(sizeof(vbc_rtu_mb_t), 4);
	if (smb) {
		if (vbc_rtu_mb_init(smb, core_type, port)) {
			return smb;
		} else {
			rt_free_align(smb);
		}
	}
	return NULL;
}

/*
 *mb port device
 */
#ifdef VBC_RTU_MODBUS_RTTHREAD_USE_DEVICE

#include <rtdevice.h>
#include <rtthread.h>

static rt_err_t _vbc_rtu_mb_rtdevice_rx_indicate(rt_device_t dev, rt_size_t size) {
	vbc_rtu_mb_port_rtdevice_t *smb_port_device = dev->user_data;

	smb_port_device->rx_size = size;

	return rt_sem_release(&(smb_port_device->rx_sem));
}

static int _vbc_rtu_mb_rtdevice_open(vbc_rtu_mb_t *smb) {
	vbc_rtu_mb_port_rtdevice_t *smb_port_device =
			(vbc_rtu_mb_port_rtdevice_t *) smb->port;
	if (smb_port_device->device) {
		smb_port_device->device->user_data = smb_port_device;

		rt_device_set_rx_indicate(smb_port_device->device,
								  _vbc_rtu_mb_rtdevice_rx_indicate);

		rt_device_open(smb_port_device->device, smb_port_device->oflag);

		if (smb_port_device->rts_set) {
			smb_port_device->rts_set(0);
		}
	}
	return 0;
}

static int _vbc_rtu_mb_rtdevice_close(vbc_rtu_mb_t *smb) {
	vbc_rtu_mb_port_rtdevice_t *smb_port_device =
			(vbc_rtu_mb_port_rtdevice_t *) smb->port;
	if (smb_port_device->device) {
		rt_device_close(smb_port_device->device);
	}
	return 0;
}

static int _vbc_rtu_mb_rtdevice_write(vbc_rtu_mb_t *smb, uint8_t *data,
									  uint16_t length) {
	vbc_rtu_mb_port_rtdevice_t *smb_port_device =
			(vbc_rtu_mb_port_rtdevice_t *) smb->port;

	if (smb_port_device->rts_set)
		smb_port_device->rts_set(1);

	rt_enter_critical();// poll write start

	rt_device_write(smb_port_device->device, 0, data, length);

	rt_exit_critical();// poll write end

	if (smb_port_device->rts_set)
		smb_port_device->rts_set(0);

	return length;
}

static int _vbc_rtu_mb_rtdevice_read(vbc_rtu_mb_t *smb, uint8_t *data,
									 uint16_t length) {
	vbc_rtu_mb_port_rtdevice_t *smb_port_device =
			(vbc_rtu_mb_port_rtdevice_t *) smb->port;

	return rt_device_read(smb_port_device->device, 0, data, length);
}

static int _vbc_rtu_mb_rtdevice_flush(vbc_rtu_mb_t *smb) {
	vbc_rtu_mb_port_rtdevice_t *smb_port_device =
			(vbc_rtu_mb_port_rtdevice_t *) smb->port;

	int rc = rt_device_read(smb_port_device->device, 0, smb->read_buff,
							VBC_MODBUS_MAX_ADU_LENGTH);

	rt_sem_control(&(smb_port_device->rx_sem), RT_IPC_CMD_RESET, RT_NULL);
	return rc;
}

static int _vbc_rtu_mb_rtdevice_wait(vbc_rtu_mb_t *smb, int timeout) {
	int rc = -1;
	vbc_rtu_mb_port_rtdevice_t *smb_port_device =
			(vbc_rtu_mb_port_rtdevice_t *) smb->port;

	rc = rt_sem_take(&(smb_port_device->rx_sem), timeout);
	if (rc < RT_EOK) {
		return MODBUS_TIMEOUT;
	}
	if (smb_port_device->rx_size == 0) {
		return MODBUS_ERROR_READ;
	}
	return rc;
}

vbc_rtu_mb_port_rtdevice_t *vbc_rtu_mb_port_rtdevice_get(vbc_rtu_mb_t *smb) {
	if ((smb->port->type == MODBUS_PORT_DEVICE) &&
		(smb->port->magic == MODBUS_PORT_MAGIC)) {
		return (vbc_rtu_mb_port_rtdevice_t *) smb->port;
	}
	return NULL;
}

int vbc_rtu_mb_port_rtdevice_init(vbc_rtu_mb_port_rtdevice_t *port,
								  const char *device_name) {
	(*(uint32_t *) &(port->base.type)) = MODBUS_PORT_DEVICE;
	(*(uint32_t *) &(port->base.magic)) = MODBUS_PORT_MAGIC;
	port->base.open = _vbc_rtu_mb_rtdevice_open;
	port->base.close = _vbc_rtu_mb_rtdevice_close;
	port->base.read = _vbc_rtu_mb_rtdevice_read;
	port->base.write = _vbc_rtu_mb_rtdevice_write;
	port->base.flush = _vbc_rtu_mb_rtdevice_flush;
	port->base.wait = _vbc_rtu_mb_rtdevice_wait;

	port->device_name = device_name;
	port->device = rt_device_find(device_name);
	if (port->device) {
		rt_sem_init(&(port->rx_sem), device_name, 0, RT_IPC_FLAG_FIFO);
	}
	return 0;
}

int vbc_rtu_mb_port_rtdevice_deinit(vbc_rtu_mb_port_rtdevice_t *port) {
	if (port->device) {
		rt_device_close(port->device);
	}
	return 0;
}

vbc_rtu_mb_port_rtdevice_t *
vbc_rtu_mb_port_rtdevice_create(const char *device_name) {
	vbc_rtu_mb_port_rtdevice_t *smb_port_device =
			rt_malloc_align(sizeof(vbc_rtu_mb_port_rtdevice_t), 4);
	if (smb_port_device) {
		rt_memset(smb_port_device, 0, sizeof(vbc_rtu_mb_port_rtdevice_t));
		vbc_rtu_mb_port_rtdevice_init(smb_port_device, device_name);
		return smb_port_device;
	}
	return NULL;
}

int vbc_rtu_mb_port_rtdevice_delete(void *port) {
	vbc_rtu_mb_port_rtdevice_t *port_device = port;
	if (port_device && (port_device->base.type == MODBUS_PORT_DEVICE) &&
		(port_device->base.magic == MODBUS_PORT_MAGIC)) {
		vbc_rtu_mb_port_rtdevice_deinit(port_device);
		rt_free_align(port_device);
	}
	return 0;
}

int vbc_rtu_mb_rtu_set_serial_rts(vbc_rtu_mb_t *smb, int (*rts_set)(int on)) {
	vbc_rtu_mb_port_rtdevice_t *smb_port_device =
			vbc_rtu_mb_port_rtdevice_get(smb);
	if (smb_port_device) {
		smb_port_device->rts_set = rts_set;
	}
	return 0;
}

int vbc_rtu_mb_rtu_set_serial_name(vbc_rtu_mb_t *smb,
								   const char *device_name) {
	vbc_rtu_mb_port_rtdevice_t *smb_port_device =
			vbc_rtu_mb_port_rtdevice_get(smb);
	if (smb_port_device) {
		vbc_rtu_mb_port_rtdevice_init(smb_port_device, device_name);
	}
	return 0;
}

int vbc_rtu_mb_rtu_set_serial_config(vbc_rtu_mb_t *smb,
									 struct serial_configure *serial_config) {
	vbc_rtu_mb_port_rtdevice_t *smb_port_device =
			vbc_rtu_mb_port_rtdevice_get(smb);
	if (smb_port_device) {
		if (smb_port_device->device) {
			rt_device_control(smb_port_device->device, RT_DEVICE_CTRL_CONFIG,
							  serial_config);
		}
	}
	return 0;
}
int vbc_rtu_mb_rtu_set_oflag(vbc_rtu_mb_t *smb, int oflag) {
	vbc_rtu_mb_port_rtdevice_t *smb_port_device =
			vbc_rtu_mb_port_rtdevice_get(smb);
	if (smb_port_device) {
		smb_port_device->oflag = oflag;
	}
	return 0;
}

#endif
#endif