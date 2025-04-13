// simple bootloader for raspberry pi pico
// loaded by first-stage bootloader. copies entire program to SRAM for execution
// does not configure flash for XIP (execute-in-place)

#include <stdint.h>
#include "../inc/rp2040.h"

#define FLASH_START     0x10000100
#define SRAM_START      0x20000000
#define APP_SIZE        0x00004000  // 16KB app size

__attribute__((naked, noreturn, section(".boot2")))
void _start(void) {
    // initialize SPI
    SSIENR = 0x00000000;
    BAUDR  = 0x00000004;
    CTRLR0 = 0x001F0300;
    SPI_CTRLR0 = 0x03000218;
    SSIENR = 0x00000001;

    // enable XIP

    // Copy program to SRAM
    uint32_t* flash_ptr = (uint32_t *)FLASH_START;
    uint32_t* sram_ptr = (uint32_t *)SRAM_START;
    while (flash_ptr < (uint32_t *)(FLASH_START+APP_SIZE))
        *sram_ptr++ = *flash_ptr++;

    // set VTOR
    VTOR = SRAM_START;

    // set stack pointer
    uint32_t SP = *(uint32_t *)SRAM_START;
    __asm("MSR msp, %0" :: "r" (SP));

    // Jump to reset_handler
    uint32_t reset_handler = *(uint32_t *)(SRAM_START + 4);
    reset_handler |= 1; // set thumb bit
    ((void (*)(void))reset_handler)();
    //__asm__ volatile ("bx %0\n" :: "r" (reset_handler));
}
