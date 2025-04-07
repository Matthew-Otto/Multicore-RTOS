#include "hw/clock.h"
#include "hw/gpio.h"
#include "os/schedule.h"

int main(void) {
    // initialize system clock
    init_sysclock();

    // initialize gpio
    init_gpio(25);

    // reset subsys?



    // initialize scheduler (starts OS, never returns)
    init_scheduler();

    for (;;) {}
}