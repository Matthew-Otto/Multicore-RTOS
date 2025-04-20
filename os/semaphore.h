#ifndef SEMA4_H
#define SEMA4_H

#include <stdint.h>
#include "../inc/rp2040.h"
#include "../os/schedule.h"

typedef enum {
  SCHEDULER = 0,
  HEAP = 1,
  UART = 2,
  BIN_SEMA4 = 29,
  COUNTING_SEMA4 = 30,
} lock_t;

typedef struct TCB TCB_t;
typedef struct Sema4 Sema4_t;
struct Sema4{
  int32_t value;   
  TCB_t *bthreads_root;
};

static inline void lock(lock_t lock) {
  volatile uint32_t *spinlock_addr = (volatile uint32_t *)(0xd0000100+(lock<<2));
  while (*spinlock_addr == 0);
}

static inline void unlock(lock_t lock) {
  volatile uint32_t *spinlock_addr = (volatile uint32_t *)(0xd0000100+(lock<<2));
  *spinlock_addr = 1;
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