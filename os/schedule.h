#include <stdint.h>

// initialize cpu1
// initialize systick timer
// enable interrupts
void init_scheduler(void);

// triggers a pendsv
void context_switch(void);

// systick interrupt service routine
void systick_handler(void);

// schedules next task
void schedule(void);

// add new thread to schedule
void add_thread(void);

// schedules next thread
void suspend(void);

// sleeps thread for <sleep_time> ms
void sleep(uint32_t sleep_time);

// removes thread from schedule and frees memory
void kill(void);