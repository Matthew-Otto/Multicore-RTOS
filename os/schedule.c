#include <stdint.h>
#include "../inc/rp2040.h"

// initialize cpu1
// initialize systick timer
// enable interrupts
void init_scheduler(uint32_t timeslice) {
    // intialize cpu1
    // TODO

    // initialize systick timer
    #define CLK_RATE 12000000 // 12MHz XOSC
    SYST_RVR = (CLK_RATE / 1000) * timeslice; // set schedule timeslice (in ms)
    SYST_CSR |= 1 << 2; // use processor clock
    SYST_CSR |= 1 << 1; // enable systick interrupts
    SYST_CSR |= 1;      // enable systick timer

    // enable interrupts
    // TODO
}

// triggers a pendsv interrupt
void context_switch(void) {
    ICSR = 1 << PENDSVSET_OFFSET;
}

// systick interrupt service routine
void systick_handler(void) {
    schedule();
}

// schedules next task
void schedule(void) {

    // ...
    context_switch();
}

// add new thread to schedule
void add_thread(void) {

}

// schedules next thread
void suspend(void) {

}

// sleeps thread for <sleep_time> ms
void sleep(uint32_t sleep_time) {

}

// removes thread from schedule and frees memory
void kill(void) {

}