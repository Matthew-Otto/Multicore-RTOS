#include <stdint.h>
#include "os/schedule.h"
#include "hw/gpio.h"
#include "hw/sys.h"
#include "../hw/hwctrl.h"
#include "../hw/uart.h"
#include "os/semaphore.h"
#include "os/ipc.h"
#include "os/interpreter.h"




static uint32_t gpio = 2;
void basict(void){
    disable_interrupts();
    uint32_t pin = gpio++;
    init_gpio(pin, GPIO_OUTPUT);
    enable_interrupts();
    while (1) {
        if (proc_id()) for (int i = 10; i > 0; i--){__asm ("nop");};
        gpio_toggle(pin);
        //sleep(200);
    }
}

void main(void) {
    add_thread(&interpreter,1024,1);
    //add_thread(&basict,256,1);
    //add_thread(&basict,256,1);
    
    // initialize scheduler (starts OS, never returns)
    init_scheduler(1, true);
}


/********************* heap / semaphore test *********************/
/* 
void basict(void){
    disable_interrupts();
    uint32_t pin = gpio++;
    init_gpio(pin, GPIO_OUTPUT);
    enable_interrupts();
    while (1) {
        if (proc_id()) for (int i = 10; i > 0; i--){__asm ("nop");};
        gpio_toggle(pin);
    }
}

void heapthrash(void) {
    disable_interrupts();
    uint32_t pin = gpio++;
    init_gpio(pin, GPIO_OUTPUT);
    enable_interrupts();
    void *memptr;
    while (1) {
        gpio_set(pin);
        memptr = malloc(1024);
        gpio_clear(pin);
        free(memptr);
    }
}


void main(void) {
    add_thread(&basict,128,1);
    add_thread(&basict,128,1);
    add_thread(&basict,128,1);
    add_thread(&basict,128,1);
    add_thread(&heapthrash,128,1);
    add_thread(&heapthrash,128,1);
    
    // initialize scheduler (starts OS, never returns)
    init_scheduler(1, true);
} */


/********************* basic scheduler test *********************/
/* 
static uint32_t gpio = 2;

void basict(void){
    uint32_t pin = gpio++;
    init_gpio(pin, GPIO_OUTPUT);
    while (1) {
        if (proc_id()) for (int i = 10; i > 0; i--){__asm ("nop");};
        gpio_toggle(pin);
    }
}

void threadthrash(void) {
    uint32_t pin = gpio++;
    init_gpio(pin, GPIO_OUTPUT);
    while (1) {
        gpio_toggle(pin);
        suspend();
    }
}

void main(void) {
    add_thread(&threadthrash,128,1);
    add_thread(&threadthrash,128,1);
    add_thread(&threadthrash,128,1);
    add_thread(&threadthrash,128,1);
    add_thread(&threadthrash,128,1);
    add_thread(&threadthrash,128,1);
    add_thread(&threadthrash,128,1);
    add_thread(&threadthrash,128,1);
    
    // initialize scheduler (starts OS, never returns)
    init_scheduler(1, true);
} */