/*
 * Copyright (c) 2011, Zolertia(TM) is a Trademark by Advancare,SL
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
 *         Device drivers for ZoundTracker in Zolertia Z1.
 * \author
 *         Enric M. Calvo, Zolertia <ecalvo@zolertia.com>
 */


#include <stdio.h>
#include <signal.h>
#include "contiki.h"
#include "i2cmaster.h"
#include "zt.h"



/* Bitmasks and bit flag variable for keeping track of zt status. */
enum ZT_STATUSTYPES
{
  /* must be a bit and not more, not using 0x00. */
  INITED = 0x01,
  RUNNING = 0x02,
  STOPPED = 0x04,
  LOW_POWER = 0x08,
  AAA = 0x10,			// available to extend this...
  BBB = 0x20,			// available to extend this...
  CCC = 0x40,			// available to extend this...
  DDD = 0x80 			// available to extend this...
};
static enum ZT_STATUSTYPES _ZT_STATUS = 0x00;


/*---------------------------------------------------------------------------*/
/* Init the temperature sensor: ports, pins, registers, interrupts (none enabled), I2C,
    default threshold values etc. */

void
zt_init (void)
{
  if (!(_ZT_STATUS & INITED))
    {
      PRINTFDEBUG ("ZT init\n");
      _ZT_STATUS |= INITED;
      /* ZoundTracker is a Ziglet, hence always powered 
      ZT_PWR_DIR |= ZT_PWR_PIN;
      ZT_PWR_SEL &= ~ZT_PWR_SEL;
      ZT_PWR_SEL2 &= ~ZT_PWR_SEL;
      ZT_PWR_REN &= ~ZT_PWR_SEL;
      ZT_PWR_OUT |= ZT_PWR_PIN; */

      /* Set up ports and pins for I2C communication */
      i2c_enable ();

   }
}

/*---------------------------------------------------------------------------*/
/* Write to a 16-bit register.
    args:
      reg       register to write to
      val       value to write
*/

void
zt_write_reg (u8_t reg, u16_t val)
{
  u8_t tx_buf[] = { reg, 0x00, 0x00 };

  tx_buf[1] = (u8_t) (val >> 8);
  tx_buf[2] = (u8_t) (val & 0x00FF);

  i2c_transmitinit (ZT_ADDR);
  while (i2c_busy ());
  PRINTFDEBUG ("I2C Ready to TX\n");

  i2c_transmit_n (3, tx_buf);
  while (i2c_busy ());
  PRINTFDEBUG ("WRITE_REG 0x%04X @ reg 0x%02X\n", val, reg);
}

/*---------------------------------------------------------------------------*/
/* Read register.
    args:
      reg       what register to read
    returns the value of the read register type u16_t
*/
/*
u32_t
zt_read_reg (u8_t reg)
{
  u8_t buf[] = { 0x00, 0x00, 0x00, 0x00 };
  u32_t retVal = 0;
  u8_t rtx = reg;
  PRINTFDEBUG ("READ_REG 0x%02X\n", reg);

  // transmit the register to read 
  i2c_transmitinit (ZT_ADDR);
  while (i2c_busy ());
  i2c_transmit_n (1, &rtx);
  while (i2c_busy ());

  // receive the data 
  i2c_receiveinit (ZT_ADDR);
  while (i2c_busy ());
  i2c_receive_n (4, &buf[0]);
  while (i2c_busy ());

  retVal = (u16_t) (buf[0] << 8 | (buf[1]));

  return retVal;
}
*/
/*---------------------------------------------------------------------------*/
/* Read SPL in a raw format in 32 bit. Further processing will be needed
   to make an interpretation of these 12 or 13-bit data, depending on configuration
*/
/*
u32_t
zt_read_spl_raw (void)
{
  u16_t rd = 0;

  rd = zt_read_reg32 (ZT_SPL);

  return rd;
}
*/

/*---------------------------------------------------------------------------*/
/* Read register.
    args:
      reg       what register to read
    returns the value of the read register type u16_t
*/

u16_t
zt_read_reg (u8_t reg)
{
  u8_t buf[] = { 0x00, 0x00 };
  u16_t retVal = 0;
  u8_t rtx = reg;
  PRINTFDEBUG ("READ_REG 0x%02X\n", reg);

  // transmit the register to read 
  i2c_transmitinit (ZT_ADDR);
  while (i2c_busy ());
  i2c_transmit_n (1, &rtx);
  while (i2c_busy ());

  // receive the data 
  i2c_receiveinit (ZT_ADDR);
  while (i2c_busy ());
  i2c_receive_n (2, &buf[0]);
  while (i2c_busy ());

  retVal = (u16_t) (buf[0] << 8 | (buf[1]));

  return retVal;
}

/*---------------------------------------------------------------------------*/
/* Read temperature in a raw format. Further processing will be needed
   to make an interpretation of these 12 or 13-bit data, depending on configuration
*/

u16_t
zt_read_spl_raw (void)
{
  u16_t rd = 0;

  rd = zt_read_reg (ZT_SPL);

  return rd;
}

/*---------------------------------------------------------------------------*/
/* Simple Read temperature. Return is an integer with temperature in 1deg. precision 
   Return value is a signed 8 bit integer.
*/

int8_t
zt_read_spl_simple (void)
{
  int16_t raw = 0;
  int8_t rd = 0;
  int16_t sign = 1;
  int16_t abstemp, temp_int;

  raw = (int16_t) zt_read_reg (ZT_SPL);
  if (raw < 0)
    {
      abstemp = (raw ^ 0xFFFF) + 1;
      sign = -1;
    }

  /* Integer part of the temperature value */
  temp_int = (abstemp >> 8) * sign;

  /* See test-zt.c on how to print values of temperature with decimals 
     fractional part in 1/10000 of degree 
     temp_frac = ((abstemp >>4) % 16) * 625; 
     Data could be multiplied by 63 to have less bit-growth and 1/1000 precision 
     Data could be multiplied by 64 (<< 6) to trade-off precision for speed 
  */

  rd = (int8_t) (temp_int);
  return rd;
}



