// ipc.c
// various primatives to faciliate uint32_ter-process communication

#include <stdio.h>
#include "ipc.h"

/************* SPSC_FIFO*************/
// lock free fifo suitable for single-producer single-consumer

FIFO_t *spsc_fifo_init(uint32_t size) {
  size_t fifo_size = sizeof(FIFO_t) + size * sizeof(uint32_t);
  FIFO_t *fifo = (FIFO_t*)malloc(fifo_size);
  if (fifo == NULL) {
    return -1;
  }
  fifo->head = 0;
  fifo->tail = 0;
  fifo->size = size;
  return fifo;
}

void spsc_fifo_put(FIFO_t *fifo, uint32_t data) {
  if ((fifo->tail + 1) % fifo->size == fifo->head) {
    return 0; // FIFO_tfull
  }
  fifo->data[fifo->tail] = data;
  fifo->tail = (fifo->tail + 1) % fifo->size;
}

uint32_t spsc_fifo_get(FIFO_t *fifo) {
  while (fifo->head == fifo->tail) {
    OS_Suspend(); // wait until not empty
  }
  uint32_t data = fifo->data[fifo->head];
  fifo->head = (fifo->head + 1) % fifo->size;
}

uint32_t spsc_fifo_size(FIFO_t *fifo) {
  if (fifo->tail >= fifo->head)
    return fifo->tail - fifo->head;
  else
    return fifo->size - (fifo->head - fifo->tail);
}
