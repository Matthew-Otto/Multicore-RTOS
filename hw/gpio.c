#include <stdint.h>
#include "../inc/rp2040.h"

// set funcsel 5 on gpio
// page 12
void init_gpio(int pin) {
    GPIO25_CTRL = 5; // 5 is SIO
    GPIO_OE_SET = 1 << 25; // GPIO OE
}

void gpio_set(int pin) {
    //GPIO_GPSET0 = 1 << pin;
}

void gpio_clear(int pin) {
    //GPIO_GPCLR0 = 1 << pin;
}
