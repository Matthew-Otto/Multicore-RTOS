#include <stdint.h>
#include "os/schedule.h"
#include "hw/gpio.h"
#include "hw/sys.h"

#include "os/semaphore.h"
#include "os/ipc.h" // test


// debug
void Thread1(void){
    init_gpio(3, GPIO_OUTPUT);
    for(;;){
        if (proc_id()) for (int i = 10; i > 0; i--){__asm ("nop");};
        gpio_toggle(3);
    }
}
void Thread2(void){
    init_gpio(4, GPIO_OUTPUT);
    for(;;){
        if (proc_id()) for (int i = 10; i > 0; i--){__asm ("nop");};
        gpio_toggle(4);
    }
}
void Thread3(void){
    init_gpio(5, GPIO_OUTPUT);
    for(;;){
        if (proc_id()) for (int i = 10; i > 0; i--){__asm ("nop");};
        gpio_toggle(5);
    }
}
void Thread4(void){
    init_gpio(6, GPIO_OUTPUT);
    for(;;){
        if (proc_id()) for (int i = 10; i > 0; i--){__asm ("nop");};
        gpio_toggle(6);
    }
}
// end debug


void main(void) {

    init_gpio(25, GPIO_OUTPUT);
    
    add_thread(&Thread1,128,1);
    add_thread(&Thread2,128,1);
    //add_thread(&Thread3,128,1);
    //add_thread(&Thread4,128,1);
    //add_thread(&Thread2,128,1);
    //add_thread(&Thread3,128,1);
    
    // initialize scheduler (starts OS, never returns)
    init_scheduler(1, true); // 126ms timeslice (maximum at 133MHz clock)

    for (;;) {}
}