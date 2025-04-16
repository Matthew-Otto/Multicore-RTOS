#include <stdint.h>
#include "gpio.h"
#include "../inc/rp2040.h"
#include "sys.h"

void init_gpio(uint8_t pin, gpio_mode_e mode) {
    init_subsystem(IO_BANK0);
    init_subsystem(PADS_BANK0);

    (*((volatile uint32_t *)(IO_BANK0_BASE + 0x004 + pin * 8))) = 5; // function 5 = SIO

    volatile uint32_t *pad_ctrl_set = (volatile uint32_t *)(PADS_BANK0_BASE + 0x2004 + pin * 4);
    volatile uint32_t *pad_ctrl_clr = (volatile uint32_t *)(PADS_BANK0_BASE + 0x3004 + pin * 4);

    switch (mode) {
        case GPIO_INPUT:
            *pad_ctrl_clr = (1 << 3);
            *pad_ctrl_clr = (1 << 2);
            GPIO_OE_CLR = (1 << pin);
            break;
        case GPIO_INPUT_PULLUP:
            *pad_ctrl_set = (1 << 3);
            *pad_ctrl_clr = (1 << 2);
            GPIO_OE_CLR = (1 << pin);
            break;
        case GPIO_INPUT_PULLDOWN:
            *pad_ctrl_clr = (1 << 3);
            *pad_ctrl_set = (1 << 2);
            GPIO_OE_CLR = (1 << pin);
            break;
        case GPIO_OUTPUT:
            *pad_ctrl_clr = (1 << 3);
            *pad_ctrl_clr = (1 << 2);
            GPIO_OE_SET = (1 << pin);
            break;
    }
}

void gpio_set(int pin) {
    GPIO_OUT_SET = 1 << pin;
}

void gpio_clear(int pin) {
    GPIO_OUT_CLR = 1 << pin;
}

void gpio_toggle(int pin) {
    GPIO_OUT_XOR = 1 << pin;
}
