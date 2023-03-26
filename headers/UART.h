
#ifndef UART_H_
#define UART_H_
#include <common.h>
void UartInit(void);
void UartSendChar(uint8_t byte);
void UartSendString(char* index);
void UartSendNumber(uint16_t value);
uint8_t Digit(uint16_t d, uint8_t m);



#endif /* UART_H_ */