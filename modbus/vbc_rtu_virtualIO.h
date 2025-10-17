/**
  ******************************************************************************
  * @file       : modbus
  * @author     : Andrew
  * @date       : 2025-10-17 15:48
  * @brief      : 
  * 
  * @copyright  : Copyright 2025 Zhang Xu. All rights reserved.
  ******************************************************************************
  */

#ifndef _VBC_RTU_VIRTUALIO_H
#define _VBC_RTU_VIRTUALIO_H

#include "vbc_rtu_mb.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

int vbc_rtu_read_hold_regs(uint16_t addr, uint16_t num, uint16_t *buffer);
int vbc_rtu_write_hold_regs(uint16_t addr, uint16_t num, uint16_t *buffer);
#endif