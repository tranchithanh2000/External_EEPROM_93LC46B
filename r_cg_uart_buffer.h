#ifndef R_CG_UART_BUFFER_H
#define R_CG_UART_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

#define RX_BUFFER_SIZE 7

extern volatile uint8_t RX_Data;                 // Latest received byte
extern volatile uint8_t RX_Buffer[RX_BUFFER_SIZE]; // Buffer for incoming messages
extern volatile uint8_t RX_Index;               // Buffer index

#endif // R_CG_UART_BUFFER