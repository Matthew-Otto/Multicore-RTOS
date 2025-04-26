#ifndef SEMA4_H
#define SEMA4_H

#include <stdint.h>
#include "../inc/rp2040.h"
#include "../hw/hwctrl.h"
#include "../os/schedule.h"

typedef enum {
  SCHEDULER = 0,
  BGSCHEDULER = 1,
  HEAP = 2,
  UART = 3,
  BIN_SEMA4 = 29,
  COUNTING_SEMA4 = 30,
} lock_e;

typedef struct TCB TCB_t;
typedef struct Sema4 Sema4_t;
struct Sema4{
    int32_t value;   
    TCB_t *bthreads_root;
};

static inline void lock(lock_e lock) {
    memory_barrier();
    volatile uint32_t *spinlock_addr = (volatile uint32_t *)(0xd0000100+(lock<<2));
    while (*spinlock_addr == 0) memory_barrier();
    memory_barrier();
}

static inline void unlock(lock_e lock) {
    volatile uint32_t *spinlock_addr = (volatile uint32_t *)(0xd0000100+(lock<<2));
    memory_barrier();
    *spinlock_addr = 1;
    memory_barrier();
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
void c_wait(Sema4_t *sem);

// increments semaphore
// unblocks first (if any) thread blocked by this semaphore
void c_signal(Sema4_t *sem);

#endif // SEMA4_H