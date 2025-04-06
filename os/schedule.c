#include <stdint.h>

// initialize cpu1
// initialize systick timer
// enable interrupts
void init_scheduler(void) {

}

// triggers a pendsv
void context_switch(void) {
__asm(
    "LDR     R0, =NVIC_INT_CTRL   \n" // Load address of the ICSR register
    "MOV     R1, #NVIC_PENDSVSET  \n" // Set the PENDSVSET bit in R1
    "STR     R1, [R0]             \n" // Write R1 to ICSR register
);
}

// systick interrupt service routine
void systick_handler(void) {
    schedule();
}

// schedules next task
void schedule(void) {

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