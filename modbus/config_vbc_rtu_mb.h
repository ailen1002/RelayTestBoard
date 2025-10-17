/**
  ******************************************************************************
  * @file       : modbus
  * @author     : Andrew
  * @date       : 2025-10-17 15:37
  * @brief      : 
  * 
  * @copyright  : Copyright 2025 Zhang Xu. All rights reserved.
  ******************************************************************************
  */

#ifndef _CONFIG_VBC_RTU_MB_H
#define _CONFIG_VBC_RTU_MB_H
/*
 *  RTTHREAD PORT
 */
#define VBC_RTU_MODBUS_RTTHREAD            1u
#define VBC_RTU_MODBUS_RTTHREAD_USE_DEVICE 1u
#if VBC_RTU_MODBUS_RTTHREAD
#include "vbc_rtu_mb_port_rtthread.h"
#endif

#define VBC_RTU_MODBUS_CRC_BYTE_SWAP 0u

/*
 *  USER CONFIG
 */
#define VBC_RTU_SLAVE_ADDR        1u
#define VBC_RTU_UART_DEVICE_NAME  "uart5"
#define VBC_RTU_THREAD_STACK_SIZE 1024u
#define VBC_RTU_THREAD_PRIORITY   12u
#define VBC_RTU_THREAD_TICKS      15u
#endif
/* __CONFIG_VBC_MODBUS_H__ */
