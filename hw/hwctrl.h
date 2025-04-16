// low level cpu control
// exposes arm instructions as c functions

#ifndef HW_CTRL_H
#define HW_CTRL_H

#include <stdint.h>

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

#endif // HW_CTRL_H