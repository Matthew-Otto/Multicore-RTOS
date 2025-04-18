#ifndef SEMA4_H
#define SEMA4_H

#include <stdint.h>
#include "../inc/rp2040.h"
#include "../os/schedule.h"

typedef struct TCB TCB_t;
typedef struct Sema4 Sema4_t;
struct Sema4{
  int32_t value;   
  TCB_t *bthreads_root;
};

static inline void sched_lock(void) {
  while (!SPINLOCK0);
}

static inline void sched_release(void) {
  SPINLOCK0 = 1;
}

// initializes semaphore
void init_semaphore(Sema4_t *sem, int32_t value);

// sets semaphore to 1
// blocks until semaphore is acquired
void b_wait(Sema4_t *sem);

// set semaphore to 0
// unblocks first (if any) thread blocked by this semaphore
void b_signal(Sema4_t *sem);

// decrements semaphore
// blocks until semaphore is acquired
void wait(Sema4_t *sem);

// increments semaphore
// unblocks first (if any) thread blocked by this semaphore
void signal(Sema4_t *sem);

#endif // SEMA4_H