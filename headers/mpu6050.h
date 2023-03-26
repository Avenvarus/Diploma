#ifndef MPU6050_H_
#define MPU6050_H_


#include "I2C.h"

 //define address
 #define SENS_ADDR 0b1101000 //7-bit address
 //register defines
 #define WHO_AM_I 0x75
 #define CONFIG 0x1A
 #define GYRO_CONFIG 0X1B
 #define ACCEL_CONFIG 0X1C
 #define SMPLRT_DIV 0x19
 #define ACCEL_XOUT_H 0x3B
 #define ACCEL_XOUT_L 0x3C
 #define ACCEL_YOUT_H 0x3D
 #define ACCEL_YOUT_L 0x3E
 #define ACCEL_ZOUT_H 0x3F
 #define ACCEL_ZOUT_L 0x40
 #define GYRO_XOUT_H 0x43
 #define GYRO_XOUT_L 0x44
 #define GYRO_YOUT_H 0x45
 #define GYRO_YOUT_L 0x46
 #define GYRO_ZOUT_H 0x47
 #define GYRO_ZOUT_L 0x48
 #define TEMP_OUT_H 0x41
 #define TEMP_OUT_L 0x42
 #define PWR_MGMT_1 0x6B
 #define USER_CTRL 0x6A
 
//NACK or ACK ?
#define LAST_BYTE 1
#define NON_LAST_BYTE 0
//read / write bits
#define WRITE 0
#define READ 1

void Gy86Init(void);
void GyroInit(void);
void TransmitDataToGy86(uint8_t reg_addr, uint8_t data);
uint8_t RecieveDataFromGy86(uint8_t reg_addr);

#endif /* MPU6050_H_ */