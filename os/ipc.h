// ipc.h
// various primatives to faciliate inter-process communication

/************* SPSC_FIFO *************/
// lock free fifo suitable for single-producer single-consumer
struct FIFO {
    int head;
    int tail;
    int size;
    int data[];
};
typedef struct FIFO FIFO_t;

FIFO_t *spsc_fifo_init(int size);
void spsc_fifo_put(FIFO_t *fifo, int data);
int spsc_fifo_get(FIFO_t *fifo);
int spsc_fifo_size(FIFO_t *fifo);
