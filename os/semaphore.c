#include <stdio.h>
#include "semaphore.h"
#include "schedule.h"
#include "../hw/hwctrl.h"


void init_semaphore(Sema4_t *sem, int32_t value) {
    sem->value = value;
    sem->bthreads_root = NULL;
}

// blocks until semaphore is acquired
void b_wait(Sema4_t *sem) {
    while(atomic_test_and_set(&sem->value)) {
        sched_block(sem);
    } 
}

// unblocks first (if any) thread blocked by this semaphore
// if unblocked thread is higher priority, schedule it immediately
void b_signal(Sema4_t *sem) {
    uint32_t stat = start_critical();
    memory_barrier();
    atomic_clear(&sem->value);
    memory_barrier();
    if (sem->bthreads_root != NULL) {
        if (sched_unblock(sem)) schedule();
    }
    end_critical(stat);
}

// decrements semaphore
// blocks until semaphore is acquired
void c_wait(Sema4_t *sem) {
    uint32_t stat = start_critical();
    lock(COUNTING_SEMA4);

    memory_barrier();
    while (sem->value == 0) {
        memory_barrier();
        unlock(COUNTING_SEMA4);
        enable_interrupts();
        sched_block(sem);
        disable_interrupts();
        lock(COUNTING_SEMA4);
    }
    memory_barrier();
    sem->value--;
    memory_barrier();

    unlock(COUNTING_SEMA4);
    end_critical(stat);
}

// increments semaphore
// unblocks first (if any) thread blocked by this semaphore
void c_signal(Sema4_t *sem) {
    uint32_t stat = start_critical();
    lock(COUNTING_SEMA4);

    sem->value++;
    memory_barrier();
    if (sem->bthreads_root != NULL) {
        if (sched_unblock(sem)) schedule();
    }

    unlock(COUNTING_SEMA4);
    end_critical(stat);
}
