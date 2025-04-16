#include <stdint.h>
#include "hw/gpio.h"
#include "os/schedule.h"

#include "os/semaphore.h"
#include "os/ipc.h" // test


// debug

uint32_t Count1;   // number of times thread1 loops
uint32_t Count2;   // number of times thread2 loops
uint32_t Count3;   // number of times thread3 loops
void Thread1(void){
    Count1 = 0;          
    for(;;){
        gpio_toggle(2);
        Count1++;
    }
}
void Thread2(void){
    Count2 = 0;          
    for(;;){
        gpio_toggle(3);
        Count2++;
    }
}
void Thread3(void){
    Count3 = 0;          
    for(;;){
        gpio_toggle(4);
        Count3++;
    }
}

// end debug


void main(void) {

    init_gpio(25, GPIO_OUTPUT);
    
    //init_gpio(2, GPIO_OUTPUT);
    //init_gpio(3, GPIO_OUTPUT);
    //init_gpio(4, GPIO_OUTPUT);
    //add_thread(&Thread1,128,1);
    //add_thread(&Thread2,128,1);
    //add_thread(&Thread3,128,1);
    
    // initialize scheduler (starts OS, never returns)
    init_scheduler(126, true); // 126ms timeslice (maximum at 133MHz clock)

    for (;;) {}
}