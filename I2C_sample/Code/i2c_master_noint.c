#include "i2c_master_noint.h"

void i2c_master_setup(void)
{
    //set baud rate by BRG and ON bit to 1
    I2C1BRG = 1000;
    I2C1CONbits.ON = 1;
}

void i2c_master_start(void)
{
    I2C1CONbits.SEN = 1; //send the start bit
    while(I2C1CONbits.SEN)
    {
        ; //wait for the start bit to be sent
    }
}

void i2c_master_restart(void)
{
    I2C1CONbits.RSEN = 1; //send the restart bit
    while(I2C1CONbits.RSEN)
    {
        ; //wait for the Restart bit to be sent
    }
}

void i2c_master_send(unsigned char byte)
{
    I2C1TRN = byte;
    while(I2C1STATbits.TRSTAT)
    {
        ;
    }
    if(I2C1STATbits.ACKSTAT)
    {
        while(1)
        {
            ;
        }
    }
}

unsigned char i2c_master_recv(void) { // receive a byte from the slave
    I2C1CONbits.RCEN = 1; // start receiving data
    while (!I2C1STATbits.RBF) 
    {
        ;
    } // wait to receive the data
    return I2C1RCV; // read and return the data
}

void i2c_master_ack(int val) { // sends ACK = 0 (slave should send another byte)
    // or NACK = 1 (no more bytes requested from slave)
    I2C1CONbits.ACKDT = val; // store ACK/NACK in ACKDT
    I2C1CONbits.ACKEN = 1; // send ACKDT
    while (I2C1CONbits.ACKEN) 
    {
        ;
    } // wait for ACK/NACK to be sent
}

void i2c_master_stop(void) { // send a STOP:
    I2C1CONbits.PEN = 1; // comm is complete and master relinquishes bus
    while (I2C1CONbits.PEN) 
    {
        ;
    } // wait for STOP to complete
}

void i2c_master_read_multiple(unsigned char address, unsigned char reg, unsigned char * data, int length)
{
    int i = 0;
    i2c_master_start();
    i2c_master_send(address);
    i2c_master_send(reg);
    i2c_master_restart();
    i2c_master_send(address|0x01);
    for(i=0; i<length-1; i++)
    {
        data[i] = i2c_master_recv();
        i2c_master_ack(0);
    }
    data[length-1] = i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop();
}   



