#include "queue.h"

void queue_push(queue *q, uint8_t x)
{
  if(!queue_is_full(q))
  {
    q->back = (q->back + 1) % QUEUE_SIZE;
    q->data[q->back] = x;
  }
}

int queue_pop(queue *q)
{
  if(queue_is_empty(q))
    return -1;
  
  q->front = (q->front + 1) % QUEUE_SIZE;
  
  return q->data[q->front];
}

bool queue_is_empty(queue *q)
{
  return (q->front == q->back);
}

bool queue_is_full(queue *q)
{
  return ((q->back + 1) % QUEUE_SIZE == q->front);
}

int queue_get_front(queue *q)
{
  if(queue_is_empty(q))
    return -1;
  else
    return q->data[(q->front + 1) % QUEUE_SIZE];
}

int queue_get_back(queue *q)
{
  if(queue_is_empty(q))
    return -1;
  else
    return q->data[q->back];
}

int queue_get_size(queue *q)
{
  int size;
  
  if(q->front < q->back)
    size = q->back - q->front;
  else
    size = QUEUE_SIZE - (q->front - q->back);
  
  return size;
}
