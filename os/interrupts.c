#include <stdint.h>
#include <stdbool.h>
#include "../inc/rp2040.h"
#include "../hw/hwctrl.h"
#include "../hw/sys.h"
#include "../hw/gpio.h"
#include "../os/schedule.h"
//#include "heap.h"
//#include "semaphore.h"

#define ARRAY_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))
#define DELAY(cycles) {int count = cycles; while (count--) __asm ("nop");}

extern uint32_t __stack_top;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

// forward declaration of ISR
extern void main(void);
static void default_isr(void);
void reset_handler(void);
void hardfault_handler(void);
void pendSV_handler(void);
void systick_handler(void);


// vector table
__attribute__((used, section(".vectors"))) void (*vector_table[])(void) =
{
    (void *)CPU0_STACK_TOP, //  0 stack pointer
    reset_handler,          //  1 reset
    default_isr,            //  2 NMI
    hardfault_handler,      //  3 hardFault
    0,                      //  4 reserved
    0,                      //  5 reserved
    0,                      //  6 reserved
    0,                      //  7 reserved
    0,                      //  8 reserved
    0,                      //  9 reserved
    0,                      // 10 reserved
    default_isr,            // 11 SVCall
    0,                      // 12 reserved
    0,                      // 13 reserved
    pendSV_handler,         // 14 pendSV
    systick_handler,        // 15 sysTick
};


__attribute__((naked)) void reset_handler(void) {
    disable_interrupts();
    
    // configure clocks
    init_sysclock();
    
    // copy .data section from FLASH to SRAM
    uint32_t* src = &_sidata;
    uint32_t* dst = &_sdata;
    while (dst < &_edata){
        *dst++ = *src++;
    }
    
    // zero initialize .bss
    uint32_t* bss = &_sbss;
    while (bss < &_ebss){
        *bss++ = 0x0;
    }
    
    // TODO: reset vector table offset?
    //VTOR = (uint32_t)vector_table;
    // TODO: reset stack pointer?

    // initialize heap
    heap_init();

    // jump to main
    main();
}

void hardfault_handler(void) {
    init_gpio(25, GPIO_OUTPUT);

    const bool code[] = {1, 1, 1, 0, 0, 0, 1, 1, 1};

    int i = 0;
    while (1) {
        GPIO_OUT_SET = 1 << 25;
        if (code[i]) {
            DELAY(10000000);
        } else {
            DELAY(5000000);
        }
        GPIO_OUT_CLR = 1 << 25;
        DELAY(5000000);
        
        if (i == (ARRAY_LEN(code)-1)) {
            i = 0;
            DELAY(10000000);
        } else {
            i++;
        }
    }
}

void pendSV_handler(void) {
    __asm volatile (
        "CPSID I\n"
        "PUSH {R4-R7}\n"
        "LDR R1, [%0]\n" // get RunPt
        "MRS R3, MSP\n"
        "STR R3, [R1]\n" // store SP

        "LDR R1, [%1]\n" // load next TCB pointer
        "STR R1, [%0]\n" // update RunPt
        "LDR R3, [R1]\n" // load SP
        "MSR MSP, R3\n"
        "POP {R4-R7}\n"
        "CPSIE I\n"
        "BX LR\n"
        :: "r" (get_RunPt()), "r" (get_NextRunPt())
    );
}

// systick interrupt service routine
void systick_handler(void) {
    //GPIO_OUT_XOR = 1 << 25;  // XOR the LED pin
    GPIO_OUT_XOR = 1 << 2;
    //schedule();
}

void default_isr(void) {
    while (1);
}
