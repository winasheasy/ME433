#include<xc.h>
#include<sys/attribs.h>

void initSPI()
{
    //Pin B14 has to be SCK1
    //Turn off analog pins
    ANSELA = 0;     //1 for analog
    
    //Make A0 an output for CS
    TRISAbits.TRISA0 = 0;
    LATAbits.LATA0 = 1;
    
    //Make A1 SDO1
    RPA1Rbits.RPA1R = 0b0011;
    
    //Make B5 SDI
    SDI1Rbits.SDI1R = 0b0001;
    
    //setup SPI
    
    SPI1CON = 0; //turn off spi module and reset it
    SPI1BUF;
    
    SPI1BRG = 1;
    SPI1STATbits.SPIROV = 0;
    SPI1CONbits.CKE = 1;
    SPI1CONbits.MSTEN = 1;
    SPI1CONbits.ON = 1;
    
}

unsigned char spi_io(unsigned char o)
{
    SPI1BUF = o;
    while(!SPI1STATbits.SPIRBF)
    {
        ;
    }
    return SPI1BUF;
}
