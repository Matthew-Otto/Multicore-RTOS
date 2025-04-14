#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <stdint.h>
#include "heap.h"

typedef enum {RUNNING, ACTIVE, SLEEPING, BLOCKED, DEAD} ThreadState;

typedef struct TCB TCB_t;
struct TCB {
  uint32_t *sp;
  TCB_t *next_tcb;
  TCB_t *prev_tcb;
  uint8_t id;
  ThreadState state;
  uint32_t resume_tick;
  uint8_t priority;
  uint32_t *stack;
};

// initialize cpu1
// initialize systick timer
// enable interrupts
void init_scheduler(uint32_t timeslice);

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

#endif // SCHEDULE_H