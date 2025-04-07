// ipc.h
// various primatives to faciliate inter-process communication

#include <stdint.h>

/************* SPSC_FIFO *************/
// lock free fifo suitable for single-producer single-consumer
struct FIFO {
    uint32_t head;
    uint32_t tail;
    uint32_t size;
    uint32_t data[];
};
typedef struct FIFO FIFO_t;

FIFO_t *spsc_fifo_init(uint32_t size);
uint32_t spsc_fifo_put(FIFO_t *fifo, uint32_t data);
uint32_t spsc_fifo_get(FIFO_t *fifo);
uint32_t spsc_fifo_size(FIFO_t *fifo);
