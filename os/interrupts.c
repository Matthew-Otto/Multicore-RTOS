#include <stdint.h>
#include <stdbool.h>
#include "../inc/rp2040.h"
#include "../hw/hwctrl.h"
#include "../hw/sys.h"
#include "../hw/timer.h"
#include "../hw/uart.h"
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
void timer0_interrupt(void);
void uart0_handler(void);

// vector table
__attribute__((used, section(".vectors"))) void (*vector_table[])(void) =
{
    (void *)CPU0_STACK_TOP, //  0 stack pointer
    // System Exceptions
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
    // Interrupts
    timer0_interrupt,       //  0 TIMER_IRQ_0
    default_isr,            //  1 TIMER_IRQ_1
    default_isr,            //  2 TIMER_IRQ_2
    default_isr,            //  3 TIMER_IRQ_3
    default_isr,            //  4 PWM_IRQ_WRAP
    default_isr,            //  5 USBCTRL_IRQ
    default_isr,            //  6 XIP_IRQ
    default_isr,            //  7 PIO0_IRQ_0
    default_isr,            //  8 PIO0_IRQ_1
    default_isr,            //  9 PIO1_IRQ_0
    default_isr,            // 10 PIO1_IRQ_1
    default_isr,            // 11 DMA_IRQ_0
    default_isr,            // 12 DMA_IRQ_1
    default_isr,            // 13 IO_IRQ_BANK0
    default_isr,            // 14 IO_IRQ_QSPI
    default_isr,            // 15 SIO_IRQ_PROC0
    default_isr,            // 16 SIO_IRQ_PROC1
    default_isr,            // 17 CLOCKS_IRQ
    default_isr,            // 18 SPI0_IRQ
    default_isr,            // 19 SPI1_IRQ
    uart0_handler,          // 20 UART0_IRQ
    default_isr,            // 21 UART1_IRQ
    default_isr,            // 22 ADC_IRQ_FIFO
    default_isr,            // 23 I2C0_IRQ
    default_isr,            // 24 I2C1_IRQ
    default_isr,            // 25 RTC_IRQ
};


__attribute__((naked)) void reset_handler(void) {
    disable_interrupts();

    // configure clocks
    init_sysclock();

    // reset cpu1
    PSM_FRCE_OFF_SET = 0x1 << 16;
    PSM_FRCE_OFF_CLR = 0x1 << 16;

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

    // initialize heap
    heap_init();

    // init tick generator
    init_watchdog_tick();

    // configure system timer
    init_timer();

    // initialize uart
    init_uart();

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
        "MOV    R4, R8\n"
        "MOV    R5, R9\n"
        "MOV    R6, R10\n"
        "MOV    R7, R11\n"
        "PUSH   {R4-R7}\n"
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
        "POP    {R4-R7}\n"
        "MOV    R8, R4\n"
        "MOV    R9, R5\n"
        "MOV    R10, R6\n"
        "MOV    R11, R7\n"
        "POP    {R4-R7}\n"

        "CPSIE I\n"       // enable interrupts
        "BX LR\n"         // branch to new task
    );
}

#include "../hw/gpio.h"
// systick interrupt service routine
void systick_handler(void) {
    gpio_set(2);
    schedule();
    gpio_clear(2);
}

void timer0_interrupt(void) {
    // acknowledge interrupt
    TIMER_INTR = 0x1;
    NVIC_ICPR = 0x1 << TIMER_IRQ_0;
    unsleep();
}

void uart0_handler(void) {
    // acknowledge interrupt
    NVIC_ICPR = 0x1 << UART0_IRQ;
    uint32_t intstat = UART0_UARTRIS;
    if (intstat & (0x1 << 5)) {
        UART0_UARTICR = (0x1 << 5); // acknowledge interrupt
        uart_tx_interrupt();
    } else if (intstat & ((0x1 << 4) | (0x1 << 6))) {
        UART0_UARTICR = (0x1 << 4); // acknowledge interrupt
        uart_rx_interrupt();
    }
}

void default_isr(void) {
    while (1);
}
