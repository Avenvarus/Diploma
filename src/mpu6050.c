#include "mpu6050.h"

void TransmitDataToGy86(uint8_t reg_addr, uint8_t data)
{
	I2cStart();
	I2cSendByte((SENS_ADDR<<1) | WRITE);
	I2cSendByte(reg_addr);
	I2cSendByte(data);
	I2cStop();
}

//функция приёма байта по протоколу i2c
uint8_t RecieveDataFromGy86(uint8_t reg_addr)
{
	uint8_t res;
	I2cStart();
	I2cSendByte((SENS_ADDR<<1) | WRITE);
	I2cSendByte(reg_addr);
	I2cStart();
	I2cSendByte((SENS_ADDR<<1) | READ);
	res = I2cGetByte(LAST_BYTE);
	I2cStop();
	return res;
}
 
void Gy86Init(void)
{
	TWBR = 0x48; //TWBR = (F_CPU/2*SCLfreq)-8=72, SCLfreq=100KHz
}
 
void GyroInit(void)
{
	_delay_ms(150);
	TransmitDataToGy86(SMPLRT_DIV, 0x07);//делитель частоты дискретизации = 8 /* 1KHz sample rate */
	TransmitDataToGy86(CONFIG, 0x00);// Fs = 8KHz
	TransmitDataToGy86(GYRO_CONFIG, 0x18);// Full scale range +/- 2000 degree/C
	TransmitDataToGy86(ACCEL_CONFIG, 0x00);// Full scale range +/- 2g
	TransmitDataToGy86(PWR_MGMT_1, 0x01); //configured one of the gyroscopes as the clock reference for improved stability
}

