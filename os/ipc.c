// ipc.c
// various primatives to faciliate inter-process communication

#include <stdio.h>
#include "ipc.h"
#include "schedule.h"

/************* FIFO *************/
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
}

uint32_t fifo32_size(FIFO32_t *fifo) {
  if (fifo->tail >= fifo->head)
    return fifo->tail - fifo->head;
  else
    return fifo->size - (fifo->head - fifo->tail);
}


// 8 bit ///////////////////////////////////////////////////

FIFO8_t *fifo8_init(uint8_t size) {
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
}

uint8_t fifo8_size(FIFO8_t *fifo) {
  if (fifo->tail >= fifo->head)
    return fifo->tail - fifo->head;
  else
    return fifo->size - (fifo->head - fifo->tail);
}