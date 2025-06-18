#include <intrinsics.h>

__no_init  volatile unsigned int IO_BANK0_GPIO25_CTRL    @ 0x400280CC;
__no_init  volatile unsigned int IO_BANK0_GPIO27_CTRL    @ 0x400280DC;

__no_init  volatile unsigned int SIO_GPIO_OE_SET         @ 0xd0000038;
__no_init  volatile unsigned int SIO_GPIO_OUT_SET        @ 0xd0000018;
__no_init  volatile unsigned int SIO_GPIO_OUT_CLR        @ 0xd0000020;


__no_init  volatile unsigned char PADS_BANK0_GPIO25      @ 0x40038068;
__no_init  volatile unsigned char PADS_BANK0_GPIO27      @ 0x40038070;


__root unsigned char STACK2[1024];

typedef  void(*isr_adr)(void);


void Reset_Handler2()                      {}
void NMI_Handler2()                      {}
void HardFault_Handler2()                {}
void SVC_Handler2()                      {}
void PendSV_Handler2()                   {}
void SysTick_Handler2()                  {}


__root const isr_adr vector_table2[]={
      (isr_adr)(STACK2+1024),           /*     Initial Stack Pointer */
    Reset_Handler2,                            /*     Reset Handler */
    NMI_Handler2,                              /* -14 NMI Handler */
    HardFault_Handler2,                        /* -13 Hard Fault Handler */
    0,                                        /*     Reserved */
    0,                                        /*     Reserved */
    0,                                        /*     Reserved */
    0,                                        /*     Reserved */
    0,                                        /*     Reserved */
    0,                                        /*     Reserved */
    0,                                        /*     Reserved */
    SVC_Handler2,                              /*  -5 SVCall Handler */
    0,                                        /*     Reserved */
    0,                                        /*     Reserved */
    PendSV_Handler2,                           /*  -2 PendSV Handler */
    SysTick_Handler2,                          /*  -1 SysTick Handler */

};



void CPU1_main( ) {
  
     while ( 1 ) {
    SIO_GPIO_OUT_SET = 0x01U << 27;
    delay();    delay();
    SIO_GPIO_OUT_CLR= 0x01U << 27;
    delay();    delay();
    
  }
  
}

///////////////////
#define SIO_IRQ_FIFO            (25)
#define SIO_FIFO_IRQ_NUM(core) SIO_IRQ_FIFO
//// SIO_BASE  0xd0000000
__no_init volatile unsigned int FIFO_ST  @ 0xd0000050;
__no_init volatile unsigned int FIFO_WD  @ 0xd0000054;
__no_init volatile unsigned int FIFO_RD @ 0xd0000058;

__no_init volatile unsigned int NVIC_ISER0 @ 0xE000E100;
__no_init volatile unsigned int NVIC_ISER1 @ 0xE000E104;
__no_init volatile unsigned int NVIC_ISER2 @ 0xE000E108;


typedef unsigned char   bool;

static inline bool multicore_fifo_rvalid(void) {
    return FIFO_ST & 0x1; /* get VLD bit*/
}

static inline bool multicore_fifo_wready(void) {
    return FIFO_ST &0x2 ;// RDY bit  SIO_FIFO_ST_RDY_BITS;
}

static inline void multicore_fifo_drain(void) {
    while (multicore_fifo_rvalid())
        (void) FIFO_RD;
}

void tight_loop_contents( ) {
  
  __no_operation();
  __no_operation();
  __no_operation();
  __no_operation();
  __no_operation();
}


static inline void multicore_fifo_push_blocking(unsigned int  data) {
    // We wait for the fifo to have some space
    while (!multicore_fifo_wready())
        tight_loop_contents();

    FIFO_WD = data;

    // Fire off an event to the other core
    __SEV();
}

static inline unsigned int  multicore_fifo_pop_blocking(void) {
    // If nothing there yet, we wait for an event first,
    // to try and avoid too much busy waiting
    while (!multicore_fifo_rvalid())
        __WFE();

    return FIFO_RD;
}

//bool irq_is_enabled(uint num) {
////    check_irq_param(num);
//   return 0 != (nvic_hw->iser[num/32] & (1 << num % 32));
//
//}
//
//void irq_set_mask_n_enabled(uint n, uint32_t mask, bool enabled) {
//    irq_set_mask_n_enabled_internal(n, mask, enabled);
//}
//
//void irq_set_enabled(uint num, bool enabled) {
////    check_irq_param(num);
//    // really should update irq_set_mask_enabled?
//    irq_set_mask_n_enabled(num / 32, 1u << (num % 32), enabled);
//}
//static inline void irq_set_mask_n_enabled_internal(uint n, uint32_t mask, bool enabled) {
////    invalid_params_if(HARDWARE_IRQ, n * 32u >= ((NUM_IRQS + 31u) & ~31u));
//    // >32 IRQs (well this works for the bottom 32 which is all that is passed in
//    if (enabled) {
//        nvic_hw->icpr[n] = mask;
//        nvic_hw->iser[n] = mask;
//    } else {
//        nvic_hw->icer[n] = mask;
//    }
//}



/////////////////
const unsigned int  cmd_sequence[] =
{0, 0, 1, (unsigned int ) vector_table2, (unsigned int ) (STACK2+1024), (unsigned int) CPU1_main};




void test_func ( ) {
  int i;
   unsigned int irq_num =25; 
   bool enabled = ((NVIC_ISER0&0x02000000)!= 0)? 1:0;
   NVIC_ISER0 |= 0xFDFFFFFF;//irq_set_enabled(irq_num, false);
   
   
   
    
  unsigned int seq =0;
  unsigned int response;




  
  do {
    unsigned int cmd;
    cmd = cmd_sequence[seq];
    
    if ( !cmd ) {
      multicore_fifo_drain();
      __SEV();
      
    }
    multicore_fifo_push_blocking(cmd);
    response = multicore_fifo_pop_blocking();
    // Move to next state on correct response (echo-d value) otherwise start over
    seq = cmd == response ? seq + 1 : 0;
    
    
  } while ( seq < ( (sizeof(cmd_sequence)/4) ) ) ;
  

  
}



