#include <stdio.h>



__no_init  volatile unsigned int IO_BANK0_GPIO25_CTRL    @ 0x400280CC;
__no_init  volatile unsigned int IO_BANK0_GPIO27_CTRL    @ 0x400280DC;

__no_init  volatile unsigned int SIO_GPIO_OE_SET         @ 0xd0000038;
__no_init  volatile unsigned int SIO_GPIO_OUT_SET        @ 0xd0000018;
__no_init  volatile unsigned int SIO_GPIO_OUT_CLR        @ 0xd0000020;


__no_init  volatile unsigned char PADS_BANK0_GPIO25      @ 0x40038068;
__no_init  volatile unsigned char PADS_BANK0_GPIO27      @ 0x40038070;


void delay() {
    volatile int i;
    for (i=0; i<1000000;i++);
}

#pragma location=".picobin_block"
__root const  unsigned int IMAGE_DEF[] ={0xffffded3, 0x10210142, 0x000001ff, 0x000000000, 0xab123579};


int main()
{
  int i;
  IO_BANK0_GPIO25_CTRL = 0x05;
  PADS_BANK0_GPIO25    = 0x34;
  SIO_GPIO_OE_SET = 0x01U << 25;
  
  IO_BANK0_GPIO27_CTRL = 0x05;
  PADS_BANK0_GPIO27    = 0x34;
  SIO_GPIO_OE_SET = 0x01U << 27;
  
  
  for (i=0;i<5;i++ ) {
    SIO_GPIO_OUT_SET = 0x01U << 25;
    delay();
    SIO_GPIO_OUT_CLR= 0x01U << 25;
    delay();    
  }
  
  test_func();
  
  
  while ( 1 ) {
    SIO_GPIO_OUT_SET = 0x01U << 25;
    delay();
    SIO_GPIO_OUT_CLR= 0x01U << 25;
    delay();
    
  }
  
  
  return 0;
}
