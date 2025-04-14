// low level cpu control

#ifndef HW_INT_H
#define HW_INT_H

#include <stdint.h>

// disable interrupts
static void disable_interrupts(void){
    __asm volatile ("CPSID I\n");
}

// enable interrupts
static void enable_interrupts(void){
    __asm volatile ("CPSIE I\n");
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

#endif // CPU_H