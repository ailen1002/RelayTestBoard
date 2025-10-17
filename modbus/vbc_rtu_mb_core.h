/**
  ******************************************************************************
  * @file       : modbus
  * @author     : Andrew
  * @date       : 2025-10-17 15:59
  * @brief      : 
  * 
  * @copyright  : Copyright 2025 Zhang Xu. All rights reserved.
  ******************************************************************************
  */

#ifndef _VBC_RTU_MB_CORE_H
#define _VBC_RTU_MB_CORE_H

#include "vbc_rtu_mb_base.h"
extern const vbc_rtu_mb_core_t _vbc_rtu_mb_core;

#define _VBC_MODBUS_RTU_HEADER_LENGTH 1
#define _VBC_MODBUS_RTU_PRESET_REQ_LENGTH 6
#define _VBC_MODBUS_RTU_PRESET_RSP_LENGTH 2

#define _VBC_MODBUS_RTU_CHECKSUM_LENGTH 2
#define _VBC_MODBUS_RTU_MAX_ADU_LENGTH 260

#endif /* _VBC_MODBUS_RTU_H_ */