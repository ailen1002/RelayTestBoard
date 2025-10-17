/**
  ******************************************************************************
  * @file       : modbus
  * @author     : Andrew
  * @date       : 2025-10-17 16:00
  * @brief      : 
  * 
  * @copyright  : Copyright 2025 Zhang Xu. All rights reserved.
  ******************************************************************************
  */

#include "vbc_rtu_mb_core.h"
#include "config_vbc_rtu_mb.h"
#include "vbc_rtu_mb_utils.h"

/* Builds a RTU request header */
static int vbc_rtu_build_request_header(vbc_rtu_mb_t *smb, uint8_t *buff,
										int slave, int fun, int reg, int num) {
	buff[0] = slave;
	buff[1] = fun;
	buff[2] = reg >> 8;
	buff[3] = reg & 0x00ff;
	buff[4] = num >> 8;
	buff[5] = num & 0x00ff;
	return _VBC_MODBUS_RTU_PRESET_REQ_LENGTH;
}

/* Builds a RTU response header */
static int vbc_rtu_build_response_header(vbc_rtu_mb_t *smb, uint8_t *buff,
										 int slave, int fun) {
	buff[0] = slave;
	buff[1] = fun;
	return _VBC_MODBUS_RTU_PRESET_RSP_LENGTH;
}

static int vbc_rtu_check_send_pre(vbc_rtu_mb_t *smb, uint8_t *buff,
								  int length) {
	uint16_t crc = vbc_rtu_mb_crc16(buff, length);
	buff[length++] = crc & 0x00FF;// LOW BYTE
	buff[length++] = crc >> 8;    // HIGH BYTE
	return length;
}

static int vbc_rtu_check_wait_request(vbc_rtu_mb_t *smb, uint8_t *buff,
									  int length) {
	uint16_t crc_recv = ((uint16_t) (buff[length - 2] & 0x00FF) +
						 (uint16_t) (buff[length - 1] << 8));// LOW BYTE HIGH BYTE
	uint16_t crc_cal = vbc_rtu_mb_crc16(buff, length - 2);
	if (crc_cal != crc_recv) {
		vbc_rtu_mb_debug_error(smb, "crc  0x%04X != 0x%04X\n", crc_cal, crc_recv);
		return MODBUS_FAIL_CHECK;
	}
	if (buff[0] != smb->slave_addr && buff[0] != VBC_MODBUS_BROADCAST_ADDRESS) {
		vbc_rtu_mb_debug_error(smb, "slave adrr: 0x%02X != 0x%02X\n", buff[0],
							   smb->slave_addr);
		return MODBUS_FAIL_ADRR;
	}
	return length;
}

static int vbc_rtu_check_wait_response(vbc_rtu_mb_t *smb, uint8_t *buff,
									   int length) {
	uint16_t crc_recv = ((uint16_t) (buff[length - 2] & 0x00FF) +
						 (uint16_t) (buff[length - 1] << 8));// LOW BYTE HIGH BYTE
	uint16_t crc_cal = vbc_rtu_mb_crc16(buff, length - 2);
	if (crc_cal != crc_recv) {
		vbc_rtu_mb_debug_error(smb, "crc  0x%04X != 0x%04X\n", crc_cal, crc_recv);
		return MODBUS_FAIL_CHECK;
	}
	if (buff[0] != smb->slave_addr && buff[0] != VBC_MODBUS_BROADCAST_ADDRESS) {
		vbc_rtu_mb_debug_error(smb, "slave adrr: 0x%02X != 0x%02X\n", buff[0],
							   smb->slave_addr);
		return MODBUS_FAIL_ADRR;
	}
	return length;
}

const vbc_rtu_mb_core_t _vbc_rtu_mb_core = {
		.magic = MODBUS_CORE_MAGIC,
		.type = MODBUS_CORE_RTU,
		.len_header = _VBC_MODBUS_RTU_HEADER_LENGTH,
		.len_checksum = _VBC_MODBUS_RTU_CHECKSUM_LENGTH,
		.len_adu_max = _VBC_MODBUS_RTU_MAX_ADU_LENGTH,
		.build_request_header = vbc_rtu_build_request_header,
		.build_response_header = vbc_rtu_build_response_header,
		.check_send_pre = vbc_rtu_check_send_pre,
		.check_wait_request = vbc_rtu_check_wait_request,
		.check_wait_response = vbc_rtu_check_wait_response};