#include <stdint.h>
#include "../inc/rp2040.h"
#include "../hw/clock.h"

extern uint32_t __stack_top;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sidata;
extern uint32_t _sbss;
extern uint32_t _ebss;

// forward declaration of ISR
extern void main(void);
static void default_isr(void);
void reset_handler(void);
extern void pendSV_handler(void);
extern void systick_handler(void);


// vector table
__attribute__( ( used, section( ".vectors" ) ) ) void ( *vector_table[] )( void ) =
{
    (void *)&__stack_top,     //  0 stack pointer
    reset_handler,    //  1 reset
    default_isr,      //  2 NMI
    default_isr,      //  3 hardFault
    0,                //  4 reserved
    0,                //  5 reserved
    0,                //  6 reserved
    0,                //  7 reserved
    0,                //  8 reserved
    0,                //  9 reserved
    0,                // 10 reserved
    default_isr,      // 11 SVCall
    0,                // 12 reserved
    0,                // 13 reserved
    pendSV_handler,   // 14 pendSV
    systick_handler,  // 15 sysTick
};

void default_isr(void) {
    while (1);
}

__attribute__((naked)) void reset_handler(void) {

    // reset platform?
    init_sysclock();

    // configure clocks

    // Copy .data section from flash to RAM
    //uint32_t* src = &_sidata; // data_load
    //uint32_t* dst = &_sdata; // data_start
    //while (dst < &_edata) { // data_end
    //    *dst++ = *src++;
    //}
    
    // Zero initialize the .bss section
    //dst = &_sbss; // start bss
    //while (dst < &_ebss) { // end bss
    //    *dst++ = 0;
    //}

    // TODO: set stack pointer

    // Set vector table offset
    //VTOR = (uint32_t)vector_table;
    
    // jump to main
    main();

    while (1);
}
