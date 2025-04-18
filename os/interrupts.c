#include <stdint.h>
#include <stdbool.h>
#include "../inc/rp2040.h"
#include "../hw/hwctrl.h"
#include "../hw/sys.h"
#include "../hw/gpio.h"
#include "../os/schedule.h"


#define ARRAY_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))
#define DELAY(cycles) {int count = cycles; while (count--) __asm ("nop");}

extern volatile TCB_t *RunPt;
extern volatile TCB_t *NextRunPt;

extern uint32_t __stack_top;
extern uint8_t _sidata;
extern uint8_t _sdata;
extern uint8_t _edata;
extern uint8_t _sbss;
extern uint8_t _ebss;

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

    // init tick generator
    init_watchdog_tick();

    // clear spinlocks
    for (int i = 0; i < 32; ++i) {
        (*((volatile uint32_t *)(0xD0000100+(i*4)))) = 1;
    }
    
    // copy .data section from FLASH to SRAM
    uint8_t* src = &_sidata;
    uint8_t* dst = &_sdata;
    while (dst < &_edata){
        *dst++ = *src++;
    }
    
    // zero initialize .bss
    uint8_t* bss = &_sbss;
    while (bss < &_ebss){
        *bss++ = 0x0;
    }
    
    // TODO: reset vector table offset?
    //VTOR = (uint32_t)vector_table;
    // TODO: reset stack pointer?

    // initialize heap
    heap_init();

    // bozo debug
    //SPINLOCK0 = 1;

    // jump to main
    main();
}

void hardfault_handler(void) {
    init_gpio(25, GPIO_OUTPUT);

    const bool code[] = {1, 1, 1, 0, 0, 0, 1, 1, 1};

    int i = 0;
    while (1) {
        for (int i = 0; i < 3; i++) {
            gpio_set(25);
            DELAY(10000000);
            gpio_clear(25);
            DELAY(10000000);
        }
        DELAY(5000000);
        if (proc_id()) {
            gpio_set(25);
            DELAY(5000000);
            gpio_clear(25);
            DELAY(5000000);
            gpio_set(25);
            DELAY(5000000);
            gpio_clear(25);
            DELAY(5000000);
        } else {
            gpio_set(25);
            DELAY(5000000);
            gpio_clear(25);
            DELAY(5000000);
        }
        DELAY(20000000);
    }
}

// When pushing context to the stack, the hardware saves eight 32-bit words, comprising xPSR, ReturnAddress(), LR
// (R14), R12, R3, R2, R1, and R0.
__attribute__((naked)) void pendSV_handler(void) {
    __asm (
        "CPSID  I\n"                // disable interrupts
        "LDR    R1, =0xd0000000\n"  // get CPUID
        "LDR    R1, [R1]\n"
        "LSL    R1, R1, #2\n"       // convert CPUID into array offset
        "LDR    R0, =RunPt\n"       // get address of run pointer
        "ADD    R2, R0, R1\n"       // R2 = address of RunPT for current cpu
        "LDR    R0, =NextRunPt\n"   // get address of next run pointer
        "ADD    R3, R0, R1\n"       // R3 = address of NextRunPT for current cpu
        // save context
        "PUSH   {R4-R7}\n" 
        "MOV    R1, R8\n"
        "ADD    SP, SP, #-4\n"
        "STR    R1, [SP]\n"
        "MOV    R1, R9\n"
        "ADD    SP, SP, #-4\n"
        "STR    R1, [SP]\n"
        "MOV    R1, R10\n"
        "ADD    SP, SP, #-4\n"
        "STR    R1, [SP]\n"
        "MOV    R1, R11\n"
        "ADD    SP, SP, #-4\n"
        "STR    R1, [SP]\n"
        //save old stack pointer
        "MOV    R1, SP\n"
        "LDR    R0, [R2]\n"
        "STR    R1, [R0]\n"
        // update RunPt with NextRunPt
        "LDR    R0, [R3]\n"
        "STR    R0, [R2]\n"
        // load new SP
        "LDR    R1, [R0]\n"
        "MOV    SP, R1\n"
        // restore context
        "LDR    R1, [SP]\n"
        "MOV    R1, R11\n"
        "ADD    SP, SP, #4\n"
        "LDR    R1, [SP]\n"
        "MOV    R1, R10\n"
        "ADD    SP, SP, #4\n"
        "LDR    R1, [SP]\n"
        "MOV    R1, R9\n"
        "ADD    SP, SP, #4\n"
        "LDR    R1, [SP]\n"
        "MOV    R1, R8\n"
        "ADD    SP, SP, #4\n"
        "POP    {R4-R7}\n"

        "CPSIE I\n"       // enable interrupts
        "BX LR\n"         // branch to new task
    );
}

// systick interrupt service routine
void systick_handler(void) {
    schedule();
}

void default_isr(void) {
    while (1);
}
