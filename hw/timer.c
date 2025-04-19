#include <stdint.h>
#include "../inc/rp2040.h"
#include "../hw/sys.h"

static uint32_t systime_start = 0;

void init_timer(void) {
    init_subsystem(TIMER);
    
    // disable halting timer during debug
    TIMER_DBGPAUSE = 0x0;

    // enable timer interrupts
    TIMER_INTE = 0xF;
    NVIC_ICPR = 0x1 << TIMER_IRQ_0;
    NVIC_ISER = 0x1 << TIMER_IRQ_0;
}

void arm_timer(uint8_t alarm_id, uint32_t alarm_value) {
    switch (alarm_id) {
        case 0:
            TIMER_ALARM0 = alarm_value;
            break;
        case 1:
            TIMER_ALARM1 = alarm_value;
            break;
        case 2:
            TIMER_ALARM2 = alarm_value;
            break;
        case 3:
            TIMER_ALARM3 = alarm_value;
            break;
    }
}

uint64_t get_raw_time(void) {
    uint32_t low = TIMER_TIMELR;
    uint64_t high = TIMER_TIMEHR;
    return (high << 32) | low;
}

uint32_t get_raw_time_ms(void) {
    uint32_t low = TIMER_TIMELR;
    uint64_t high = TIMER_TIMEHR;
    return ((high << 32) | low) / 1000;
}

uint32_t get_mstime(void) {
    uint32_t current = get_raw_time_ms();
    return current - systime_start;
}

void clear_mstime(void) {
    systime_start = get_raw_time_ms();
}