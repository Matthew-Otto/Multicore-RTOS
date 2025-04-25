#include <stdio.h>
#include <stdint.h>
#include "os/schedule.h"
#include "hw/gpio.h"
#include "hw/sys.h"
#include "hw/hwctrl.h"
#include "hw/uart.h"
#include "hw/timer.h"
#include "os/semaphore.h"
#include "os/ipc.h"
#include "os/interpreter.h"

#include "benchmarks/dry.h"


/********************* dhrystone *********************/
volatile FIFO_t *ff;
volatile uint8_t id = 1;

void dhry(void) {
    if (proc_id()) {
        gpio_set(5);
        gpio_clear(5);
        gpio_set(4);
        gpio_clear(4);
        gpio_set(5);
        gpio_clear(5);
    } else {
        gpio_set(4);
        gpio_clear(4);
        gpio_clear(5);
        gpio_set(5);
        gpio_set(4);
        gpio_clear(4);
    }

    dry_main();
    fifo_put(ff, &id);
    id++;
    //while(1);
    kill();
}

void dhry_twice(void) {
    uint32_t tstart = get_raw_time_ms();
    add_thread(&dhry,2048,1);
    add_thread(&dhry,2048,2);

    // wait until both runs are complete
    uint8_t value;
    fifo_get(ff, &value);
    fifo_get(ff, &value);

    uint32_t tend = get_raw_time_ms();
    uint32_t duration = tend - tstart;

    char output_buffer[100];
    snprintf(output_buffer, 100, "Dhrystonex2 took %d ms.\r\n", duration);

    while (1) {
        uart_out_string(output_buffer);
        sleep(1000);
    }
}



void main(void) {
    ff = fifo_init(4, 1);

    gpio_toggle(4);
    gpio_toggle(4);
    gpio_toggle(4);
    gpio_toggle(4);

    add_thread(&dhry_twice,1024,1);

    init_scheduler(1, true);
}


/********************* basic scheduler test *********************/

/* void basicprop(void){
    volatile uint64_t x = 0;
    for (int i = 0; i < 10000; ++i) {
        if (proc_id()) for (int i = 10; i > 0; i--){__asm ("nop");};
        x = i + x - 1;
        //sleep(1);
    }
    add_thread(&basicprop,256,1);
    kill();
}

static uint32_t gpio = 4;
void basict(void){
    disable_interrupts();
    uint32_t pin = gpio++;
    init_gpio(pin, GPIO_OUTPUT);
    enable_interrupts();
    int i = 0;
    while (1) {
        if (proc_id()) for (int i = 10; i > 0; i--){__asm ("nop");};
        gpio_toggle(pin);
        if (++i == 100000) {
            i = 0;
            gpio_set(pin);
            for (int i = 10; i > 0; i--){__asm ("nop");};
            gpio_clear(pin);
            sleep(5);
        }
    }
}

void main(void) {
    init_gpio(8, GPIO_OUTPUT);
    init_gpio(9, GPIO_OUTPUT);

    //add_thread(&interpreter,1024,1);
    add_thread(&basict,256,1);
    add_thread(&basict,256,1);
    add_thread(&basict,256,2);
    //add_thread(&basict,256,1);
    //add_thread(&basict,256,1);
    //add_thread(&basict,256,1);
    //add_thread(&basict,256,1);
    //add_thread(&basicprop,256,1);
    //add_thread(&basicprop,256,1);

    // initialize scheduler (starts OS, never returns)
    init_scheduler(1, true);
} */


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