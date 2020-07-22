#include "i2c_master_noint.h"
#include "ssd1306.h"
#include <xc.h>     // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include "font.h"
#include <stdio.h>

// DEVCFG0
#pragma config DEBUG = OFF // disable debugging
#pragma config JTAGEN = OFF // disable jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // disable flash write protect
#pragma config BWP = OFF // disable boot write protect
#pragma config CP = OFF // disable code protect

// DEVCFG1
#pragma config FNOSC = PRIPLL // use primary oscillator with pll
#pragma config FSOSCEN = OFF // disable secondary oscillator
#pragma config IESO = OFF // disable switching clocks
#pragma config POSCMOD = HS // high speed crystal mode
#pragma config OSCIOFNC = OFF // disable clock output
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // disable clock switch and FSCM
#pragma config WDTPS = PS1048576 // use largest wdt
#pragma config WINDIS = OFF // use non-window mode wdt
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz

// DEVCFG3
#pragma config USERID = 0X0000 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations

void printscrn(char msg[], int line)
{
    int i = 0;
    int index, j, k, col;
    while(msg[i] != 0)
        {
            index = msg[i]-0x20;
            for (j=0; j<5; j++)
            {
                for(k=0; k<8; k++)
                {
                    col = ((ASCII[index][j])>>k)&0x01;
                    ssd1306_drawPixel(i*5+j,(line*8)+k,col);
                }
            }
            i++;
        }
}

int main()
{
   __builtin_disable_interrupts(); // disable interrupts while initializing things

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;
    
    //do your TRIS and LAT commands here
    TRISAbits.TRISA4 = 0;
    LATAbits.LATA4 = 1;
    
    i2c_master_setup();
    ssd1306_setup();
    //ssd1306_clear();
    
    char msg[26];
    int count = 0, count_check = 0;
    float fps = 0.0;
    _CP0_SET_COUNT(0);
    while(1)
    {
        sprintf(msg, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        printscrn(msg,0);
        sprintf(msg, "01234567890123456789012345");
        printscrn(msg,1);
        sprintf(msg, "Hello world! i = %d", count);
        printscrn(msg,2);
        sprintf(msg, "    FPS: %.2f",fps);
        printscrn(msg,3);
        //ssd1306_drawPixel(1,0,1);
        //ssd1306_drawPixel(2,0,0);
        //ssd1306_drawPixel(3,0,1);
        ssd1306_update();
        count++;
        if(_CP0_GET_COUNT()>=24000000)
        {
            _CP0_SET_COUNT(0);
            fps = (fps+(count - count_check))/2;
            count_check = count;
        }
        
        
        
        //i++;
        //if(i == 126)
        //    i = 0;
        
        
        LATAbits.LATA4 = !(LATAbits.LATA4);
        
    }
}
