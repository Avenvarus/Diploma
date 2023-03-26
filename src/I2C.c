#include <I2C.h>

void I2cStart(void) //СТАРТ состояние
{
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
}

void I2cStop(void) //СТОП состояние
{
	TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
}

void I2cSendByte(uint8_t data) //посылка байта с МК
{
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
}

uint8_t I2cGetByte(uint8_t is_last_byte) //приём байта МК-ом
{
	if(is_last_byte)
	TWCR = (1<<TWINT) | (1<<TWEN);// NACK
	else
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);// ACK
	while(!(TWCR & (1<<TWINT)));
	return(TWDR);
}
