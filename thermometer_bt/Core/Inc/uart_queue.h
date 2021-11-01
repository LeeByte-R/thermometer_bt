#ifndef UART_QUEUE_H
#define UART_QUEUE_H

#include "main.h"
#include <stdbool.h>

/*  Define the device uart and pc uart below according to your setup  */
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

#define device_uart &huart1
#define pc_uart &huart2


int uart_read(UART_HandleTypeDef *uart);

void uart_write(int c, UART_HandleTypeDef *uart);

int uart_peek(UART_HandleTypeDef *uart);

/* function to send the string to the uart */
void uart_send_string(const char *s, UART_HandleTypeDef *uart);

void uart_queue_init(void);

void uart_flush(UART_HandleTypeDef *uart);

/* checks if the data is available to read in the rx_buffer of the uart */
bool is_data_available(UART_HandleTypeDef *uart);

/* look for whether a particular string is sub-string in the given buffer */
bool look_for(char *s, char *buffer);

/*
 * Copy the data from the Rx buffer into the buffer, Upto and including the entered string
 * This copying will take place in the blocking mode, so you won't be able to perform any other operations
 */
void copy_upto(char *s, char *buffer, UART_HandleTypeDef *uart);

/* Copies the entered number of characters (blocking mode) from the Rx buffer into the buffer, after some particular string is detected */
void get_after(char *s, uint8_t n, char *buffer, UART_HandleTypeDef *uart);

/* Wait until a paricular string is detected in the Rx Buffer, blocking mode */
void wait_for(char *s, UART_HandleTypeDef *uart);

#endif /* UART_QUEUE_H */
