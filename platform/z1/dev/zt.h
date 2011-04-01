/*
 * Copyright (c) 2010, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         Device drivers header file for zt temperature sensor in Zolertia Z1 WSN Platform.
 * \author
 *         Enric M. Calvo, Zolertia <ecalvo@zolertia.com>
 */

#ifndef __ZT_H__
#define __ZT_H__
#include <stdio.h>
#include "i2cmaster.h"

/* -------------------------------------------------------------------------- */
/* Init the temperature sensor: ports, pins, I2C, interrupts (XXX none so far),
*/
void  zt_init(void);

/* Write to a register.
    args:
      reg       register to write to
      val       value to write
*/
void    zt_write_reg(u8_t reg, u16_t val);

/* Read one register.
    args:
      reg       what register to read
    returns the value of the read register
*/
u16_t zt_read_reg(u8_t reg);

/* Read temperature in raw format
    no args needed
*/
u16_t zt_read_spl_raw();

/* Read only integer part of the temperature in 1deg. precision.
    no args needed
*/
int8_t zt_read_spl_simple();

/* -------------------------------------------------------------------------- */
/* Reference definitions */
/* ZT slave address */
#define ZT_ADDR           0x69

/* ZT registers */
#define ZT_SPL            0x00    // read only
#define ZT_STROBE         0xB0
//#define ZT_TLOW           0x02    
//#define ZT_THIGH          0x03   

/* ZT Ports */
/* ZoundTracker hardware ports, pins and registers on the msp430 µC */
//#define ZT_PWR_DIR        P5DIR
//#define ZT_PWR_SEL        P5SEL
//#define ZT_PWR_SEL2       P5SEL2
//#define ZT_PWR_REN        P5REN
//#define ZT_PWR_OUT        P5OUT
//#define ZT_PWR_PIN        (1<<0)          // P5.0
////#define ZT_INT_PIN      (1<<7)          // P1.7


/* -------------------------------------------------------------------------- */
#endif /* ifndef __ZT_H__ */



