// various hardware control

#ifndef SYS_H
#define SYS_H

#include "../inc/rp2040.h"

typedef enum {
    ADC = 0,
    BUSCTRL = 1,
    DMA = 2,
    I2C0 = 3,
    I2C1 = 4,
    IO_BANK0 = 5,
    IO_QSPI = 6,
    JTAG = 7,
    PADS_BANK0 = 8,
    PADS_QSPI = 9,
    PIO0 = 10,
    PIO1 = 11,
    PLL_SYS = 12,
    PLL_USB = 13,
    PWM = 14,
    RTC = 15,
    SPI0 = 16,
    SPI1 = 17,
    SYSCFG = 18,
    SYSINFO = 19,
    TBMAN = 20,
    TIMER = 21,
    UART0 = 22,
    UART1 = 23,
    USBCTRL = 24,
} subsystem_e;

// configure system clock frequency
void init_sysclock(void);

// enable watchdog tick (needed for system timer and watchdog interrupts)
void init_watchdog_tick(void);

// clear reset bit for particular subsystem
void init_subsystem(subsystem_e sys);

// returns the current processor id (0 or 1)
static inline uint8_t proc_id(void) {
    return SIO_CPUID;
}

#endif // SYS_H