// various functions to control system timers

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

typedef enum {
    SLEEP = 0,
    PERIODIC = 1,
    TIMER2,
    TIMER3,
} timer_e;

void init_timer(void);

// sets alarm<alarm_id> to trigger in <duration> ms in the future
void arm_timer(timer_e alarm_id, uint32_t alarm_value);

// get raw time in usec
uint64_t get_raw_time(void);

// get raw time in msec
uint32_t get_raw_time_ms(void);

// get system time in ms
uint32_t get_mstime(void);

// clear system time
void clear_mstime(void);

#endif // TIMER_H