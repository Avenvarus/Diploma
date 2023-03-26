#ifndef I2C_H_
#define I2C_H_

#include "common.h"


void I2cStart(void);
void I2cStop(void);
void I2cSendByte(uint8_t data);
uint8_t I2cGetByte(uint8_t is_last_byte);

#endif /* I2C_H_ */