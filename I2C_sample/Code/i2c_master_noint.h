/* 
 * File:   i2c_master_noint.h
 * Author: hp
 *
 * Created on April 24, 2020, 6:01 PM
 */

#ifndef I2C_MASTER_NOINT_H__
#define	I2C_MASTER_NOINT_H__

#include<xc.h>

void i2c_master_setup(void);
void i2c_master_start(void);
void i2c_master_restart(void);
void i2c_master_send(unsigned char byte);
unsigned char i2c_master_recv(void);
void i2c_master_ack(int val);
void i2c_master_stop(void);

void i2c_master_read_multiple(unsigned char address, unsigned char reg, unsigned char * data, int length);

#endif