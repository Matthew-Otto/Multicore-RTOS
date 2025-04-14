#include "hw/gpio.h"
#include "os/schedule.h"

#include "os/ipc.h" // test

#define CLR (0x3000)
#define RESETS_BASE 0x4000C000UL
#define RESETS_RESET (RESETS_BASE + 0x00)
#define RESETS_RESET_DONE (RESETS_BASE + 0x08)
#define PUT32(address, value) (*((volatile unsigned int *)address)) = value
#define GET32(address) *(volatile unsigned int *)address

void main(void) {   
    // initialize gpio
    init_gpio(25);

    FIFO32_t *fifo = fifo32_init(128);

    // initialize scheduler (starts OS, never returns)
    init_scheduler(126); // 126ms timeslice (maximum at 133MHz clock)

    for (;;) {}
}