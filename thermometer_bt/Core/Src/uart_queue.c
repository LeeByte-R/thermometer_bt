/*
 * UartRingbuffer.c
 *
 *  Created on: 10-Jul-2019
 *      Author: Controllerstech
 */

#include "uart_queue.h"
#include "queue.h"
#include <string.h>

queue device_rx = { { 0 }, 0, 0};
queue pc_rx = { { 0 }, 0, 0};

uint8_t device_c, pc_c;

void uart_queue_init(void)
{
  HAL_UART_Receive_IT(device_uart, &device_c, 1);
  HAL_UART_Receive_IT(pc_uart, &pc_c, 1);
}

bool look_for(char *s, char *buffer)
{
  int string_n = strlen(s);
  int buffer_n = strlen(buffer);
  int i, j;
  
  for(i=0; i <= buffer_n - string_n; i++)
  {
    for(j=0; j < string_n; j++)
    {
      if(s[j] != buffer[i+j])
        break;
    }
    
    if(j == string_n)
      return true;
  }
  
  return false;
}

void uart_flush(UART_HandleTypeDef *uart)
{
  if(uart == device_uart)
  {
    device_rx.front = 0;
    device_rx.back = 0;
  }
  else if(uart == pc_uart)
  {
    pc_rx.front = 0;
    pc_rx.back = 0;
  }
}

int uart_read(UART_HandleTypeDef *uart)
{
  if(uart == device_uart)
  {
    if(queue_is_empty(&device_rx))
      return -1;
    else
      return queue_pop(&device_rx);
  }
  else if(uart == pc_uart)
  {
    if(queue_is_empty(&pc_rx))
      return -1;
    else
      return queue_pop(&pc_rx);
  }
  else
  {
    return -1;
  }
}

int uart_peek(UART_HandleTypeDef *uart)
{
  if(uart == device_uart)
  {
    if(queue_is_empty(&device_rx))
      return -1;
    else
      return queue_get_front(&device_rx);
  }
  else if(uart == pc_uart)
  {
    if(queue_is_empty(&pc_rx))
      return -1;
    else
      return queue_get_front(&pc_rx);
  }
  else
  {
    return -1;
  }
}

void uart_write(int c, UART_HandleTypeDef *uart)
{
	if(c >= 0)
    HAL_UART_Transmit(uart, (uint8_t *)&c, 1, HAL_MAX_DELAY);
}

bool is_data_available(UART_HandleTypeDef *uart)
{
  if(uart == device_uart)
    return !queue_is_empty(&device_rx);
  else if(uart == pc_uart)
    return !queue_is_empty(&pc_rx);
  else
    return false;
}

void get_after(char *s, uint8_t n, char *buffer, UART_HandleTypeDef *uart)
{
  wait_for(s, uart);
  
  for(int i=0; i < n; i++)
  {
    while(!is_data_available(uart));
    buffer[i] = uart_read(uart);
  }
}

void uart_send_string(const char *s, UART_HandleTypeDef *uart)
{
  HAL_UART_Transmit(uart, (uint8_t *)s, strlen(s), HAL_MAX_DELAY);
}

void copy_upto(char *s, char *buffer, UART_HandleTypeDef *uart)
{
	int so_far = 0, i;
	int string_n = strlen(s);
  
  while(true)
  {
    while(!is_data_available(uart));
    while(uart_peek(uart) != s[so_far])
    {
      buffer[so_far] = uart_read(uart);
      so_far++;
      while(!is_data_available(uart));
    }
    
    for(i=0; i < string_n; i++)
    {
      if(uart_peek(uart) != s[so_far])
        break;
      buffer[so_far] = uart_read(uart);
      so_far++;
      while(!is_data_available(uart));
    }
    
    if(i == string_n)
      return ;
  }
}

void wait_for(char *s, UART_HandleTypeDef *uart)
{
  int string_n = strlen(s);
  int i;
    
  while(true)
  {
    while(uart_read(uart) != s[0]);
    
    for(i=1; i < string_n; i++)
    {
      while(!is_data_available(uart));
      if(uart_read(uart) != s[i])
        break;
    }
    
    if(i == string_n)
      return ;
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *uart) {
  if(uart == device_uart)
  {
    queue_push(&device_rx, device_c);
    HAL_UART_Receive_IT(device_uart, &device_c, 1);
  }
  else if(uart == pc_uart)
  {
    queue_push(&pc_rx, pc_c);
    HAL_UART_Receive_IT(pc_uart, &pc_c, 1);
  }
}
