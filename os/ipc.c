// ipc.c
// various primatives to faciliate inter-process communication

#include <stdio.h>
#include "ipc.h"
#include "schedule.h"
#include "../inc/rp2040.h"

/********* inter-core FIFO *********/
void multicore_fifo_push_blocking(uint32_t data) {
    while (!(FIFO_ST & 0b10));
    FIFO_WR = data;
    __asm ("sev");
}

uint32_t multicore_fifo_pop_blocking(void) {
    while (!(FIFO_ST & 0b1)) __asm("wfe");
    return FIFO_RD;
}

void multicore_fifo_drain(void) {
    while (FIFO_ST & 0b1) {
        (void)FIFO_RD;
    }
}


/************* MPMC_FIFO *************/
// blocking fifo with multicore mutex

FIFO_t *fifo_init(uint32_t fifo_size, uint8_t element_size) {
    size_t size = sizeof(FIFO_t) + fifo_size * element_size;

    FIFO_t *fifo = (FIFO_t*)malloc(size);
    if (fifo == NULL) return NULL;

    init_semaphore(&fifo->mutex, 0);
    init_semaphore(&fifo->full, fifo_size);
    init_semaphore(&fifo->empty, 0);

    fifo->head = 0;
    fifo->tail = 0;
    fifo->size = size;
    fifo->element_size = element_size;
    return fifo;
}

void fifo_put(FIFO_t *fifo, const void *value) {
    c_wait(&fifo->full);
    b_wait(&fifo->mutex);

    void *addr = (uint8_t *)fifo->data + (fifo->tail * fifo->element_size);
    switch (fifo->element_size) {
        case 1: *(uint8_t *)addr = *(const uint8_t *)value; break;
        case 2: *(uint16_t *)addr = *(const uint16_t *)value; break;
        case 4: *(uint32_t *)addr = *(const uint32_t *)value; break;
        case 8: *(uint64_t *)addr = *(const uint64_t *)value; break;
    }

    fifo->tail = (fifo->tail + 1) % fifo->size;

    b_signal(&fifo->mutex);
    c_signal(&fifo->empty);
}

void fifo_get(FIFO_t *fifo, void *value) {
    c_wait(&fifo->empty);
    b_wait(&fifo->mutex);

    const void *addr = (uint8_t *)fifo->data + (fifo->head * fifo->element_size);
    switch (fifo->element_size) {
        case 1: *(uint8_t *)value = *(const uint8_t *)addr; break;
        case 2: *(uint16_t *)value = *(const uint16_t *)addr; break;
        case 4: *(uint32_t *)value = *(const uint32_t *)addr; break;
        case 8: *(uint64_t *)value = *(const uint64_t *)addr; break;
    }
    fifo->head = (fifo->head + 1) % fifo->size;

    b_signal(&fifo->mutex);
    c_signal(&fifo->full);
}

// returns the number of elements in the fifo
uint32_t fifo_size(FIFO_t *fifo) {
    return fifo->empty.value;
}


/************* spsc-FIFO *************/
// lock free fifo suitable for single-producer single-consumer

// 32 bit ////////////////////////////////////////////////////

FIFO32_t *fifo32_init(uint32_t size) {
    size_t fifo_size = sizeof(FIFO32_t) + size * sizeof(uint32_t);

    FIFO32_t *fifo = (FIFO32_t*)malloc(fifo_size);
    if (fifo == NULL) return NULL;

    fifo->head = 0;
    fifo->tail = 0;
    fifo->size = size;
    return fifo;
}

uint32_t fifo32_put(FIFO32_t *fifo, uint32_t data) {
    if ((fifo->tail + 1) % fifo->size == fifo->head) {
        return 1; // FIFO full
    }
    fifo->data[fifo->tail] = data;
    fifo->tail = (fifo->tail + 1) % fifo->size;
    return 0;
}

uint32_t fifo32_get(FIFO32_t *fifo) {
    while (fifo->head == fifo->tail) {
        suspend(); // wait until not empty
    }
    uint32_t data = fifo->data[fifo->head];
    fifo->head = (fifo->head + 1) % fifo->size;
    return data;
}

uint32_t fifo32_size(FIFO32_t *fifo) {
    if (fifo->tail >= fifo->head)
        return fifo->tail - fifo->head;
    else
        return fifo->size - (fifo->head - fifo->tail);
}


// 8 bit ///////////////////////////////////////////////////

FIFO8_t *fifo8_init(uint32_t size) {
    size_t fifo_size = sizeof(FIFO8_t) + size * sizeof(uint8_t);

    FIFO8_t *fifo = (FIFO8_t*)malloc(fifo_size);
    if (fifo == NULL) return NULL;

    fifo->head = 0;
    fifo->tail = 0;
    fifo->size = size;
    return fifo;
}

uint8_t fifo8_put(FIFO8_t *fifo, uint8_t data) {
    if ((fifo->tail + 1) % fifo->size == fifo->head) {
        return 1; // FIFO full
    }
    fifo->data[fifo->tail] = data;
    fifo->tail = (fifo->tail + 1) % fifo->size;
    return 0;
}

uint8_t fifo8_get(FIFO8_t *fifo) {
    while (fifo->head == fifo->tail) {
        suspend(); // wait until not empty
    }
    uint8_t data = fifo->data[fifo->head];
    fifo->head = (fifo->head + 1) % fifo->size;
    return data;
}

uint8_t fifo8_size(FIFO8_t *fifo) {
    if (fifo->tail >= fifo->head)
        return fifo->tail - fifo->head;
    else
        return fifo->size - (fifo->head - fifo->tail);
}
