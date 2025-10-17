/**
  ******************************************************************************
  * @file       : modbus
  * @author     : Andrew
  * @date       : 2025-10-17 15:46
  * @brief      : 
  * 
  * @copyright  : Copyright 2025 Zhang Xu. All rights reserved.
  ******************************************************************************
  */
#ifndef _VBC_RTU_MB_PORT_RTTHREAD_H
#define _VBC_RTU_MB_PORT_RTTHREAD_H

#include "config_vbc_rtu_mb.h"
#include "vbc_rtu_mb_base.h"
#include "stdint.h"
#include "string.h"

/*
 * mb on rtthread
 */
#if VBC_RTU_MODBUS_RTTHREAD
/*
 *vbc_rtu_mb_init
 */
int vbc_rtu_mb_init(vbc_rtu_mb_t *smb, uint8_t core_type, void *port);
vbc_rtu_mb_t *vbc_rtu_mb_create(uint8_t core_type, void *port);

/*
 *mb port rtthread device
 */
#if VBC_RTU_MODBUS_RTTHREAD_USE_DEVICE

#include <rtdevice.h>
#include <rtthread.h>

typedef struct vbc_rtu_mb_port_rtdevice vbc_rtu_mb_port_rtdevice_t;

struct vbc_rtu_mb_port_rtdevice {
	struct _vbc_rtu_mb_port base;
	struct serial_configure serial_config;
	const char *device_name;
	struct rt_device *device;
	struct rt_semaphore rx_sem;
	rt_size_t rx_size;
	int oflag;
	int (*rts_set)(int on);
};

vbc_rtu_mb_port_rtdevice_t *vbc_rtu_mb_port_rtdevice_get(vbc_rtu_mb_t *smb);

int vbc_rtu_mb_port_rtdevice_init(vbc_rtu_mb_port_rtdevice_t *port,
								  const char *device_name);
int vbc_rtu_mb_port_rtdevice_deinit(vbc_rtu_mb_port_rtdevice_t *port);
vbc_rtu_mb_port_rtdevice_t *
vbc_rtu_mb_port_rtdevice_create(const char *device_name);
int vbc_rtu_mb_port_rtdevice_delete(void *port);
int vbc_rtu_mb_rtu_set_serial_rts(vbc_rtu_mb_t *smb, int (*rts_set)(int on));
int vbc_rtu_mb_rtu_set_serial_name(vbc_rtu_mb_t *smb,
								   const char *device_name);
int vbc_rtu_mb_rtu_set_serial_config(vbc_rtu_mb_t *smb,
									 struct serial_configure *serial_config);
int vbc_rtu_mb_rtu_set_oflag(vbc_rtu_mb_t *smb, int oflag);

#endif
#endif
#endif