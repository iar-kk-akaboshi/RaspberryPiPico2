#include <stdio.h>



__no_init  volatile unsigned int IO_BANK0_GPIO25_CTRL    @ 0x400280CC;


__no_init  volatile unsigned int SIO_GPIO_OE_SET         @ 0xd0000038;
__no_init  volatile unsigned int SIO_GPIO_OUT_SET        @ 0xd0000018;
__no_init  volatile unsigned int SIO_GPIO_OUT_CLR        @ 0xd0000020;


__no_init  volatile unsigned char PADS_BANK0_GPIO25      @ 0x40038068;

void delay() {
    volatile int i;
    for (i=0; i<1000000;i++);
}

int main()
{

  IO_BANK0_GPIO25_CTRL = 0x05;
  PADS_BANK0_GPIO25    = 0x34;
  
  SIO_GPIO_OE_SET = 0x01U << 25;
  
  
  
  while ( 1 ) {
    SIO_GPIO_OUT_SET = 0x01U << 25;
    delay();
    SIO_GPIO_OUT_CLR= 0x01U << 25;
    delay();
    
  }
  
  
  return 0;
}
