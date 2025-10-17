/**
  ******************************************************************************
  * @file       : modbus
  * @author     : Andrew
  * @date       : 2025-10-17 15:45
  * @brief      : 
  * 
  * @copyright  : Copyright 2025 Zhang Xu. All rights reserved.
  ******************************************************************************
  */
#ifndef _VBC_MODBUS_UTILS_H_
#define _VBC_MODBUS_UTILS_H_

#include "stdint.h"
#include "stdio.h"
#include "string.h"

uint16_t vbc_rtu_mb_crc16(uint8_t *buffer, uint16_t buffer_length);
int vbc_rtu_mb_check_addr_num(uint8_t function, uint16_t address,
							  uint16_t num);
void vbc_rtu_mb_byte_copy(uint8_t *des, uint8_t *src, int num);
void vbc_rtu_mb_coil_h2m(uint8_t *des, uint8_t *src, int coil_num);
void vbc_rtu_mb_coil_m2h(uint8_t *des, uint8_t *src, int coil_num);
void vbc_rtu_mb_reg_h2m(void *dest_mb_reg, void *source_host, int reg_num);
void vbc_rtu_mb_reg_m2h(void *dest_host, void *source_mb_reg, int reg_num);

int vbc_rtu_dio_get_val(uint8_t *array, uint16_t index);
void vbc_rtu_dio_set_val(uint8_t *array, uint16_t index, uint16_t status);
uint16_t vbc_rtu_aio_get_val(uint16_t *array, uint16_t index);
void vbc_rtu_aio_set_val(uint16_t *array, uint16_t index, uint16_t status);

#endif
