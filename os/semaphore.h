#ifndef SEMA4_H
#define SEMA4_H

#include <stdint.h>
#include "schedule.h"

typedef struct Sema4 Sema4_t;
struct Sema4{
  int32_t value;   
  TCB_t *bthreads_root;
};

// send data to other core
void multicore_fifo_push_blocking(uint32_t data);
// retrieve value from other core
uint32_t multicore_fifo_pop_blocking(void);
// discard data from fifo
void multicore_fifo_drain(void);

void sched_lock(void);
void sched_release(void);

// initializes semaphore
void init_semaphore(Sema4_t *sem, int32_t value);

// decrements semaphore
// blocks until semaphore is acquired
void wait(Sema4_t *sem);

// increments semaphore
// unblocks first (if any) thread blocked by this semaphore
void signal(Sema4_t *sem);

// sets semaphore to 0
// blocks until semaphore is acquired
void b_wait(Sema4_t *sem);

// set semaphore to 1
// unblocks first (if any) thread blocked by this semaphore
void b_signal(Sema4_t *sem);

#endif // SEMA4_H