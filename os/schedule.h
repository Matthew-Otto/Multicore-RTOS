#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <stdint.h>
#include <stdbool.h>
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
void init_scheduler(uint32_t timeslice, bool multicore);

// schedules next task
void schedule(void);

// add new thread to schedule
uint32_t add_thread(void(*task)(void), uint32_t stack_size, uint32_t priority);

// schedules next thread
void suspend(void);

// sleeps thread for <sleep_time> ms
void sleep(uint32_t sleep_time);

// removes thread from schedule and frees memory
void kill(void);

uint32_t get_RunPt(void);

uint32_t get_NextRunPt(void);

#endif // SCHEDULE_H