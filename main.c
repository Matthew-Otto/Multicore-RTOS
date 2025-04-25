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


// BOZO DEBUG

void t1(void) {
    int pin = 4;
    init_gpio(pin, GPIO_OUTPUT);
    int i = 0;
    while (1) {
        gpio_toggle(pin);
        i++;
        if (i == 100000) {
            i = 0;
            sleep(3);
        }
    }
}
void t11(void) {
    int pin = 5;
    init_gpio(pin, GPIO_OUTPUT);
    while (1) {
        gpio_toggle(pin);
    }
}
void t2(void) {
    int pin = 6;
    init_gpio(pin, GPIO_OUTPUT);
    while (1) {
        gpio_toggle(pin);
    }
}
void t3(void) {
    int pin = 7;
    init_gpio(pin, GPIO_OUTPUT);
    while (1) {
        gpio_toggle(pin);
    }
}

void main(void) {
    init_gpio(2, GPIO_OUTPUT);

    add_thread(&t11, 256, 1);
    add_thread(&t1, 256, 1);
    add_thread(&t2, 256, 2);
    add_thread(&t3, 256, 2);
    init_scheduler(1, true);
}



/********************* dhrystone *********************/
FIFO_t *ff;
uint8_t id = 1;

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
    kill();
}

void dhry_twice(void) {
    char output_buffer[100];
    uart_out_string("Starting benchmark (2 runs of basic dhrystone).\r\n");

    uint32_t tstart = get_raw_time_ms();
    add_thread(&dhry,2048,1);
    add_thread(&dhry,2048,2);

    // wait until both runs are complete
    uint8_t value;
    fifo_get(ff, &value);
    fifo_get(ff, &value);

    uint32_t tend = get_raw_time_ms();
    uint32_t duration = tend - tstart;

    snprintf(output_buffer, 100, "Dhrystone x2 took %d ms.\r\n", duration);

    uart_out_string(output_buffer);
    while (1) wait_for_interrupt();
}



/* void main(void) {
    ff = fifo_init(4, 1);

    init_gpio(4, GPIO_OUTPUT);

    add_thread(&dhry_twice,1024,1);

    init_scheduler(1, false);
} */


/********************* basic priority test *********************/

void high_prior1(void) {
    init_gpio(4, GPIO_OUTPUT);
    uint32_t x = 0;
    while (1) {
        gpio_toggle(4);
        x++;
        if (x == 100000) {
            x = 0;
            sleep(2);
        }
    }
}
void high_prior2(void) {
    init_gpio(5, GPIO_OUTPUT);
    uint32_t x = 0;
    while (1) {
        gpio_toggle(5);
        x++;
        if (x == 100000) {
            x = 0;
            sleep(7);
        }
    }
}

void low_prior1(void) {
    init_gpio(6, GPIO_OUTPUT);
    while (1) {
        gpio_toggle(6);
    }
}
void low_prior2(void) {
    init_gpio(7, GPIO_OUTPUT);
    while (1) {
        gpio_toggle(7);
    }
}
void low_prior3(void) {
    init_gpio(8, GPIO_OUTPUT);
    while (1) {
        gpio_toggle(8);
    }
}
void low_prior4(void) {
    init_gpio(9, GPIO_OUTPUT);
    while (1) {
        gpio_toggle(9);
    }
}

/* void main(void) {
    add_thread(&high_prior1,2048,1);
    add_thread(&high_prior2,2048,1);
    add_thread(&low_prior1,2048,2);
    add_thread(&low_prior2,2048,2);
    add_thread(&low_prior3,2048,2);
    add_thread(&low_prior4,2048,2);

    // initialize scheduler (starts OS, never returns)
    init_scheduler(1, true);
} */

/********************* basic scheduler test *********************/

void basicprop(void){
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

/* void main(void) {
    init_gpio(2, GPIO_OUTPUT);
    //init_gpio(9, GPIO_OUTPUT);

    add_thread(&interpreter,1024,1);
    add_thread(&basict,256,1);
    add_thread(&basict,256,1);
    add_thread(&basict,256,2);
    add_thread(&basict,256,1);
    add_thread(&basict,256,1);
    add_thread(&basict,256,1);
    add_thread(&basict,256,1);
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