#include <stdio.h>
#include "semaphore.h"
#include "schedule.h"
#include "../hw/hwctrl.h"
#include "../hw/gpio.h" // BOZO

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
    atomic_clear(&sem->value);
    if (sem->bthreads_root != NULL) {
        if (sched_unblock(sem)) schedule();
    }
    end_critical(stat);
}

// decrements semaphore
// blocks until semaphore is acquired
void wait(Sema4_t *sem) {
    uint32_t stat = start_critical();
    if (sem->value > 0) {
        sem->value--;
    } else {
        while (sem->value <= 0) {
            enable_interrupts();
            suspend();
            disable_interrupts();
        }
        //OS_block(RunPt, sem);
        // put thread in blocked list
    }
    end_critical(stat);
}

// increments semaphore
// unblocks first (if any) thread blocked by this semaphore
void signal(Sema4_t *sem) {
    uint32_t stat = start_critical();
    sem->value++;
    if (sem->bthreads_root != NULL) {
        //OS_unblock();
    }
    end_critical(stat);
}
