#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include "main.h"

/* change the size of the queue */
#define QUEUE_SIZE 512

typedef struct
{
  uint8_t data[QUEUE_SIZE];
  volatile unsigned int front;
  volatile unsigned int back;
} queue;

#include "queue.h"

void queue_push(queue *q, uint8_t x);

int queue_pop(queue *q);

bool queue_is_empty(queue *q);

bool queue_is_full(queue *q);

int queue_get_front(queue *q);

int queue_get_back(queue *q);

int queue_get_size(queue *q);

#endif /* UART_QUEUE_H */
