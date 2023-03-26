#include "UART.h"

void UartInit(void)
{
	//инициализируем UART на передачу данных, 8 бит данных, 1 стоп, без проверки четности
	UCSR0B = (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	UBRR0H = 0x00;
	UBRR0L = 0x67; //16000000/(16*9600)-1 = 103 = 0x67
	_delay_ms(500);
	UartSendString("UART OK");
	UartSendChar('\n');
}

void UartSendChar(uint8_t byte)
{
	// ожидаем выставления бита UDRE0, а затем кладем посылку в UDR0
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = byte;
}

void UartSendString(char* index)
{
	while(*index) UartSendChar(*index++);
}

void UartSendNumber(uint16_t value)
{
	uint8_t digit=0;
	if(value != 0)
	{
		while(!(Digit(value, ++digit)));
		for(uint8_t i=digit; i<=5; i++) UartSendChar(Digit(value, i) + '0');
		for(uint8_t i=1; i<(5-digit); i++) UartSendChar(' ');
	}
	else
	UartSendChar('0');
}

uint8_t Digit(uint16_t d, uint8_t m)
{
	uint8_t i = 5, a;
	while(i)
	{
		a = d%10;
		if(i-- == m) break;
		d /= 10;
	}
	return(a);
}