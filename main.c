#include <mpu6050.h>
#include <UART.h>
#define RAD_TO_DEG 57.295779
#define G_LSB 16.4
#define A_LSB 16384.0

void TimerInit(void);
void PrintRawValues(void);
void PrintAngles(void);
void PrintAccAngles(void);
void PrintGyroAngleZ(void);
void ReadRawValue(void);
void CalculateError(void);
void PrintRollPitchYaw(void);
void PrintQ(void);


float Xa = 0, Ya = 0, Za = 0, t = 0, Xg = 0, Yg = 0, Zg = 0; // accelerometer and gyroscope raw values
float GyroErrorX = 0, GyroErrorY = 0, GyroErrorZ = 0; //gyroscope average values
float accAngle = 0, gyroAngleX = 0, gyroAngleY = 0, gyroAngleZ = 0; // accelerometer and gyroscope angles in degrees
float roll = 0.0, pitch = 0.0, yaw = 0.0;
static uint8_t millis = 0;  
	
	

int main (void)
{
	Gy86Init();
	GyroInit();
	UartInit();
	CalculateError();
	_delay_ms(20);
	TimerInit();
	sei();
	
	while(1)
	{
		
		ReadRawValue();
		if (millis >= 11)
		{
			// Calculating Roll and Pitch from the accelerometer data
			accAngle = (atan2f(Xa, Ya)) * RAD_TO_DEG;
			// Correct the outputs with the calculated error values
			Xg = Xg - GyroErrorX;
			Yg = Yg - GyroErrorY;
			Zg = Zg - GyroErrorZ;
			// Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by seconds (s) to get the angle in degrees
			gyroAngleZ = gyroAngleZ + Zg * millis * 0.001;
			// Complementary filter - combine accelerometer and gyro angle values
			yaw =  0.98 * gyroAngleZ + 0.02 * accAngle;
			
			//PrintAngles();
			PrintRawValues();		
			//PrintAccAngles();
			//PrintGyroAngleZ();
			
			millis = 0;
			
		}
	}
}


void ReadRawValue(void)
{
	Xa = (((int)RecieveDataFromGy86(ACCEL_XOUT_H)<<8) | (int)RecieveDataFromGy86(ACCEL_XOUT_L)) / A_LSB;
	Ya = (((int)RecieveDataFromGy86(ACCEL_YOUT_H)<<8) | (int)RecieveDataFromGy86(ACCEL_YOUT_L)) / A_LSB;
	Za = (((int)RecieveDataFromGy86(ACCEL_ZOUT_H)<<8) | (int)RecieveDataFromGy86(ACCEL_ZOUT_L)) / A_LSB;
	
	Xg = (((int)RecieveDataFromGy86(GYRO_XOUT_H)<<8) | (int)RecieveDataFromGy86(GYRO_XOUT_L)) / G_LSB;
	Yg = (((int)RecieveDataFromGy86(GYRO_YOUT_H)<<8) | (int)RecieveDataFromGy86(GYRO_YOUT_L)) / G_LSB;
	Zg = (((int)RecieveDataFromGy86(GYRO_ZOUT_H)<<8) | (int)RecieveDataFromGy86(GYRO_ZOUT_L)) / G_LSB;
	
	t = ((((int)RecieveDataFromGy86(TEMP_OUT_H)<<8) | (int)RecieveDataFromGy86(TEMP_OUT_L))/340.00)+36.53;
}

void CalculateError(void)
{
	// We can call this function in the setup section to calculate the accelerometer and gyro data error.
	// From here we will get the error values used in the above equations printed on the Monitor.
	// Note that we should place the IMU flat in order to get the proper values, so that we then can the correct values
	uint8_t c = 0;
	// Read gyro values 200 times
	
	while (c < 200)
	{
		Xg = (((int)RecieveDataFromGy86(GYRO_XOUT_H)<<8) | (int)RecieveDataFromGy86(GYRO_XOUT_L)) / G_LSB;
		Yg = (((int)RecieveDataFromGy86(GYRO_YOUT_H)<<8) | (int)RecieveDataFromGy86(GYRO_YOUT_L)) / G_LSB;
		Zg = (((int)RecieveDataFromGy86(GYRO_ZOUT_H)<<8) | (int)RecieveDataFromGy86(GYRO_ZOUT_L)) / G_LSB;
		// Sum all readings
		GyroErrorX = GyroErrorX + Xg;
		GyroErrorY = GyroErrorY + Yg;
		GyroErrorZ = GyroErrorZ + Zg;
		c++;
	}
	//Divide the sum by 200 to get the error value
	GyroErrorX = GyroErrorX / 200;
	GyroErrorY = GyroErrorY / 200;
	GyroErrorZ = GyroErrorZ / 200;
}

void TimerInit(void)
{
	TCCR0B = (1<<CS00) | (1<<CS01); // F_CPU/64 from prescaler
	TIMSK0 = (1<<TOIE0); //local interrupt 
	TCNT0 = 6;	
}

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 6; // 256-250 = 6
	millis++;
}

void PrintRawValues(void)
{
	char buffer[20], float_[10];
	/* Take values in buffer to send all parameters over USART */
	dtostrf( Xa, 3, 2, float_ );
	sprintf(buffer," Ax = %s g\t",float_);
	UartSendString(buffer);
	
	dtostrf( Ya, 3, 2, float_ );
	sprintf(buffer," Ay = %s g\t",float_);
	UartSendString(buffer);
	
	dtostrf( Za, 3, 2, float_ );
	sprintf(buffer," Az = %s g\t",float_);
	UartSendString(buffer);
	
	dtostrf( t, 3, 2, float_ );
	/* 0xF8 Ascii value of degree on serial */
	sprintf(buffer," T = %s C\t",float_);
	UartSendString(buffer);
	
	dtostrf( Xg, 3, 2, float_ );
	sprintf(buffer," Gx = %s d/s\t",float_);
	UartSendString(buffer);
	
	dtostrf( Yg, 3, 2, float_ );
	sprintf(buffer," Gy = %s d/s\t",float_);
	UartSendString(buffer);
	
	dtostrf( Zg, 3, 2, float_ );
	sprintf(buffer," Gz = %s d/s\r\n",float_);
	UartSendString(buffer);
	
	UartSendChar('\n');
}


void PrintAngles(void)
{
	char buffer[20], float_[10];
	
	dtostrf( yaw, 5, 3, float_ );
	sprintf(buffer," yaw = %s d\t",float_);
	UartSendString(buffer);
	
	UartSendChar('\n');
}

void PrintAccAngles(void)
{
	char buffer[20], float_[10];
	dtostrf( accAngle, 5, 3, float_ );
	sprintf(buffer," accAngle = %s d\t",float_);
	UartSendString(buffer);
	
	UartSendChar('\n');
}

void PrintGyroAngleZ(void)
{
	char buffer[20], float_[10];
	dtostrf( gyroAngleZ, 5, 3, float_ );
	sprintf(buffer," GyroAngleZ = %s d\t",float_);
	UartSendString(buffer);
	
	UartSendChar('\n');
}
