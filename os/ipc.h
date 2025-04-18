// ipc.h
// various primatives to faciliate inter-process communication

#ifndef IPC_H
#define IPC_H

#include <stdint.h>

/********* inter-core FIFO *********/
// send data to other core
void multicore_fifo_push_blocking(uint32_t data);
// retrieve value from other core
uint32_t multicore_fifo_pop_blocking(void);
// discard data from fifo
void multicore_fifo_drain(void);


/************* SPSC_FIFO *************/
// lock free fifo suitable for single-producer single-consumer
typedef struct FIFO32 FIFO32_t;
struct FIFO32 {
    uint32_t head;
    uint32_t tail;
    uint32_t size;
    uint32_t data[];
};

typedef struct FIFO8 FIFO8_t;
struct FIFO8 {
    uint32_t head;
    uint32_t tail;
    uint32_t size;
    uint8_t data[];
};

// single producer, single consumer 32 bit fifo
FIFO32_t *fifo32_init(uint32_t size);
uint32_t fifo32_put(FIFO32_t *fifo, uint32_t data);
uint32_t fifo32_get(FIFO32_t *fifo);
uint32_t fifo32_size(FIFO32_t *fifo);

// single producer, single consumer 8 bit fifo
FIFO8_t *fifo8_init(uint32_t size);
uint8_t fifo8_put(FIFO8_t *fifo, uint8_t data);
uint8_t fifo8_get(FIFO8_t *fifo);
uint8_t fifo8_size(FIFO8_t *fifo);

#endif // IPC_H