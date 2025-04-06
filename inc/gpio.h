#include <stdint.h>
#include "regs/addressmap.h"


#define GPIO_GPFSEL0    (*((volatile uint32_t *)IO_BANK0_BASE))
#define GPIO_GPFSEL1    (*((volatile uint32_t *)(IO_BANK0_BASE + 0x04)))
#define GPIO_GPSET0     (*((volatile uint32_t *)(IO_BANK0_BASE + 0x1C)))
#define GPIO_GPCLR0     (*((volatile uint32_t *)(IO_BANK0_BASE + 0x28)))
#define GPIO_GPLEV0     (*((volatile uint32_t *)(IO_BANK0_BASE + 0x34)))
#define GPIO_GPCLR1     (*((volatile uint32_t *)(IO_BANK0_BASE + 0x2C)))

void gpio_init(void) {
    GPIO_GPFSEL0 &= ~(0x7 << (15 % 10) * 3);     // Clear the previous setting
    GPIO_GPFSEL0 |= (0x1 << (15 % 10) * 3);      // Set GPIO15 as an output (bit 15-13)
}

void gpio_set(int pin) {
    GPIO_GPSET0 = 1 << pin;
}

void gpio_clear(int pin) {
    GPIO_GPCLR0 = 1 << pin;
}
