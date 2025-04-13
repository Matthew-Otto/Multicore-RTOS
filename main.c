#include "hw/gpio.h"
#include "os/schedule.h"

#define CLR (0x3000)
#define RESETS_BASE 0x4000C000UL
#define RESETS_RESET (RESETS_BASE + 0x00)
#define RESETS_RESET_DONE (RESETS_BASE + 0x08)
#define PUT32(address, value) (*((volatile unsigned int *)address)) = value
#define GET32(address) *(volatile unsigned int *)address

void main(void) {
    // initialize system clock

    
    // reset subsys?
    PUT32(( RESETS_RESET | CLR ), ( 1 << 5 ) );
    while ( GET32(RESETS_RESET_DONE ) & ( 1 << 5 ) == 0 );
    
    // initialize gpio
    init_gpio(25);

    // initialize scheduler (starts OS, never returns)
    init_scheduler(100); // 500ms timeslice

    for (;;) {}
}