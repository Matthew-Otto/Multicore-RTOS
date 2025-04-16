#include <stdio.h>
#include "semaphore.h"
#include "../inc/rp2040.h"
#include "../hw/hwctrl.h"


void multicore_fifo_push_blocking(uint32_t data) {
    while (!(FIFO_ST & 0b10));
    FIFO_WR = data;
    __asm ("sev");
}

uint32_t multicore_fifo_pop_blocking(void) {
    while (!(FIFO_ST & 0b1)) __asm("wfe");
    return FIFO_RD;
}

void multicore_fifo_drain(void) {
    while (FIFO_ST & 0b1) {
        (void)FIFO_RD;
        //volatile uint32_t discard = FIFO_RD;
        //(void)discard;
    }
}

void sched_lock(void) {
    while (!SPINLOCK0);
}

void sched_release(void) {
    SPINLOCK0 = 1;
}

void init_semaphore(Sema4_t *sem, int32_t value){
    sem->value = value;
    sem->bthreads_root = NULL;
}

// decrements semaphore
// blocks until semaphore is acquired
void wait(Sema4_t *sem){
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
void signal(Sema4_t *sem){
    uint32_t stat = start_critical();
    sem->value++;
    if (sem->bthreads_root != NULL) {
        //OS_unblock();
    }
    end_critical(stat);
}

// sets semaphore to 0
// blocks until semaphore is acquired
void b_wait(Sema4_t *sem){
    uint32_t stat = start_critical();
    if (sem->value == 1) {
        sem->value = 0;
    } else {
        while (sem->value != 1) {
            enable_interrupts();
            suspend();
            disable_interrupts();
        }
        //OS_block(RunPt, sem);
        // put thread in blocked list
    }
    end_critical(stat);
}

// set semaphore to 1
// unblocks first (if any) thread blocked by this semaphore
void b_signal(Sema4_t *sem){
    uint32_t stat = start_critical();
    if (sem->bthreads_root != NULL) {
        //OS_unblock();
    } else {
        sem->value = 1;
    }
    end_critical(stat);
}