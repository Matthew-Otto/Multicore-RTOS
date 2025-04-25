// low level cpu control
// exposes arm instructions as c functions

#ifndef HW_CTRL_H
#define HW_CTRL_H

#include <stdint.h>
#include "../inc/rp2040.h"

// disable interrupts
static inline void disable_interrupts(void){
    __asm volatile ("CPSID I");
}

// enable interrupts
static inline void enable_interrupts(void){
    __asm volatile ("CPSIE I");
}

// saves current interrupt status before disabling
static inline uint32_t start_critical(void){
    uint32_t primask;
    __asm volatile (
        "MRS %0, PRIMASK\n"
        "CPSID I" 
        : "=r" (primask) :: "memory");
    return primask;
}

// restores previous interrupt status (disabled/enabled)
static inline void end_critical(uint32_t primask){
    __asm volatile ("MSR PRIMASK, %0" :: "r" (primask) : "memory");
}

// go to low power mode while waiting for the next interrupt
static inline void wait_for_interrupt(void){
    __asm volatile ("WFI");
}

static inline void memory_barrier(void) {
    __asm volatile ("dmb 0xF" ::: "memory");
}

static inline void data_sync_barrier(void) {
    __asm volatile ("dsb 0xF" ::: "memory");
}

static inline void instr_sync_barrier(void) {
    __asm volatile ("isb" ::: "memory");
}

// multicore mutex
// returns 0 if lock was acquired
static inline uint32_t atomic_test_and_set(uint32_t *lock) {
    uint32_t primask = start_critical();
    while (!SPINLOCK31);
    memory_barrier();
    uint32_t rv = *lock;
    *lock = 1;
    memory_barrier();
    SPINLOCK31 = 1;
    end_critical(primask);
    return rv;
}

static inline void atomic_clear(uint32_t *lock) {
    uint32_t primask = start_critical();
    while (!SPINLOCK31);
    memory_barrier();
    *lock = 0;
    memory_barrier();
    SPINLOCK31 = 1;
    end_critical(primask);
}

#endif // HW_CTRL_H