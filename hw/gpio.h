#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

typedef enum {
    GPIO_INPUT,
    GPIO_INPUT_PULLUP,
    GPIO_INPUT_PULLDOWN,
    GPIO_OUTPUT,
} gpio_mode_e;

void init_gpio(uint8_t pin, gpio_mode_e mode);

void gpio_set(int pin);
void gpio_clear(int pin);
void gpio_toggle(int pin);

#endif // GPIO_H
