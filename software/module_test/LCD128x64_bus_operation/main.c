
#include "STC12C5A60S2.H"
//#include <reg52.h>
#include "12864.h"
#include <stdlib.h>

uchar code num[]={
/*--  0  --*/
0x00,0x7C,0x82,0x82,0x82,0x7C,0x00,0x00,
/*--  1  --*/
0x00,0x00,0x04,0xFE,0x00,0x00,0x00,0x00,
/*--  2  --*/
0x00,0xC4,0xA2,0x92,0x8A,0x84,0x00,0x00,
/*--  3  --*/
0x00,0x44,0x82,0x92,0x92,0x6C,0x00,0x00
};


void delay(unsigned int i)
{
	while (i--);
}


main()
{
  int i;

delay(60000);

  init12864();

  P1 = 0xFF;
  while(1)
  {
    clear12864();
    delay(10000);
    //P1 ^= 1;
  }
#if 0
  for (i=0; i<128; i+=8)
  {
    font8(i, 0, num);
    font8(i, 1, num+8);
    font8(i, 2, num+16);
    font8(i, 3, num+24);
    font8(i, 4, num);
    font8(i, 5, num+8);
    font8(i, 6, num+16);
    font8(i, 7, num+24);
  }
  
  while(1)
  {
    P1 = ~P1;
    delay(10000);
  }		
#endif	
}
