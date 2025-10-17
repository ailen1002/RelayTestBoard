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

#ifndef _U_PORT_H_
#define _U_PORT_H_

#include <board.h>
#include <rtdevice.h>
#include <rtthread.h>
#include <rtdbg.h>

#define DO1  GET_PIN(A, 8)
#define DO2  GET_PIN(C, 9)
#define DO3  GET_PIN(C, 8)
#define DO4  GET_PIN(C, 7)
#define DO5  GET_PIN(C, 6)
#define DO6  GET_PIN(B, 15)
#define DO7  GET_PIN(B, 14)
#define DO8  GET_PIN(B, 13)
#define DO9  GET_PIN(B, 12)
#define DO10 GET_PIN(B, 2)
#define DO11 GET_PIN(B, 1)
#define DO12 GET_PIN(B, 0)
#define DO13 GET_PIN(C, 5)
#define DO14 GET_PIN(C, 4)
#define DO15 GET_PIN(A, 7)
#define DO16 GET_PIN(A, 6)

extern volatile rt_uint16_t PINS_WORD;
extern rt_base_t DO_PINS[];

void u_port_init(void);
void u_port_set_pin(rt_base_t pin, rt_ssize_t value);

#endif /* _U_PORT_H_ */