// simple bootloader for raspberry pi pico
// loaded by first-stage bootloader.
// configures flash for XIP and jumps to reset_handler

#include <stdint.h>
#include "../inc/rp2040.h"

#define VECTOR_TABLE    0x10000100

__attribute__((naked, noreturn, section(".boot2")))
void _start(void) {
    // enable XIP
    SSIENR = 0x00000000;
    BAUDR  = 0x00000004;
    CTRLR0 = 0x001F0300;
    SPI_CTRLR0 = 0x03000218;
    SSIENR = 0x00000001;

    // set VTOR
    VTOR = VECTOR_TABLE;

    // set stack pointer
    uint32_t SP = *(uint32_t *)VECTOR_TABLE;
    __asm("MSR msp, %0" :: "r" (SP));

    // Jump to reset_handler
    uint32_t reset_handler = *(uint32_t *)(VECTOR_TABLE + 4);
    reset_handler |= 1; // set thumb bit
    ((void (*)(void))reset_handler)();
    //__asm__ volatile ("bx %0\n" :: "r" (reset_handler));
}
