/*
 * CONTROL.cpp
 *
 *  Created on: May 2, 2020
 *      Author: Karol Janic
 */

#include <main.h>
#include <CONTROL.h>
#include <LED.h>
#include <MOTOR.h>
#include <MPU6050.h>

#include <math.h>

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

extern I2C_HandleTypeDef hi2c1;

extern GPIO_InitTypeDef gpio;

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

extern ADC_HandleTypeDef hadc1;


DigitalLED_RGB rgb1, rgb2;
ServoMotor servo1, servo2;
DCMotor motor1, motor2;
Encoder encoder1, encoder2;
I2Cdev i2cdev;
MPU6050 mpu;


uint8_t char2=0, char3=0, infoX=0, infoY=0;
float  m1Val=0, m2Val=0;
bool m1Dir=0, m2Dir=0;
float x, y;
float m1, m2;
float voltage;

uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];

uint32_t supplies[3];

inline float max(float x, float y){
	if(x<0)
		x = -x;
	if(y<0)
		y = -y;
	if(x>y)
		return x;
	else
		return y;
}


inline void setup(void)
{
	rgb1.Init(GPIOA, PA15_LED1R_Pin, GPIOB, PB3_LED1G_Pin, GPIOB, PB4_LED1B_Pin);
	rgb2.Init(GPIOB, PB5_LED2R_Pin, GPIOB, PB6_LED2G_Pin, GPIOB, PB7_LED2B_Pin);
	rgb1.Off();
	rgb2.Off();

	servo1.Init(&htim1, TIM_CHANNEL_3, 0, 180, 1);	// bottom servo
	servo2.Init(&htim1, TIM_CHANNEL_4, 0, 180, 1);	// top servo
	servo1.PwmClarify(1200, 2200);
	servo2.PwmClarify(1200, 2200);
	servo1.SetNormal();
	servo2.SetNormal();

	motor1.Init(&htim1, TIM_CHANNEL_1, GPIOB, PB13_MA_IN1_Pin, GPIOB, PB12_MA_IN2_Pin, 1);
	motor2.Init(&htim1, TIM_CHANNEL_2, GPIOB, PB14_MB_IN1_Pin, GPIOB, PB15_MB_IN2_Pin, 1);
	motor1.Off();
	motor2.Off();

	encoder1.Init(&htim2, TIM_CHANNEL_1, 1920);
	encoder2.Init(&htim3, TIM_CHANNEL_1, 1920);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

	HAL_UART_Receive_IT(&huart2, &char2, 1);
	HAL_UART_Receive_IT(&huart3, &char3, 1);

	HAL_ADC_Start(&hadc1);

	i2cdev.Init(&hi2c1);

	mpu.initialize();
	mpu.DMPInitialize();

	mpu.setFullScaleAccelerationRange(0x00);
	mpu.setFullScaleGyroscopeRange(0x00);

	mpu.setXGyroscopeOffset(0);
	mpu.setYGyroscopeOffset(0);
	mpu.setZGyroscopeOffset(0);

	mpu.setXAccelerationOffset(0);
	mpu.setYAccelerationOffset(0);
	mpu.setZAccelerationOffset(0);

	uint8_t packetSize = mpu.DMPGetFIFOPacketSize();


}

inline void loop(void){

	servo1.SetAngle(170);
	servo2.SetAngle(10);

	HAL_Delay(1000);

	servo2.SetAngle(170);
	servo1.SetAngle(10);

	HAL_Delay(1000);

}

inline void setRGB(DigitalLED_RGB *led, int info)
{
	led->On(1&info, 2&info, 4&info);
}

inline void setServos(ServoMotor *topServo, ServoMotor *bottomServo, int infoX, int infoY)
{
	x = 1.6*infoX;
	x+=10;
	topServo->SetAngleProcent((uint32_t)x);
	y = 0.9*infoY;
	y+=45;
	bottomServo->SetAngleProcent((uint32_t)y);
}

inline void setMotors(DCMotor *motorR, DCMotor *motorL, int infoX, int infoY)
{
	if(infoX == 0 && infoY == 0)
	{
		motorR->SetValue(0);
		motorR->SetDirection(1);

		motorL->SetValue(0);
		motorL->SetDirection(1);
		return;
	}

	x = 2*infoX - 100;
	if(x<0){
		x = (int)((-x)/10);
		x *= -10;
	}
	else{
		x = (int)((x)/10);
		x *= 10;
	}

	y = 2*infoY - 100;
	if(y<0){
		y = (int)((-y)/10);
		y *= -10;
	}
	else{
		y = (int)((y)/10);
		y *= 10;
	}


	if(x <= 20 && x >= -20 && y == 100){		// {12:00}
		m1 = 100;
		m2 = 100;
	}
	else if(x <= 20 && x >= -20 && y == -100){	// {6:00}
		m1 = -100;
		m2 = -100;
	}
	else if(x == 0){
	m1 = y;
	m2 = y;
	}
	else if(x == 100 && y >= -20 && y <= 20){	// {3:00}
		m1 = 50;
		m2 = -50;
	}
	else if(x == -100 && y >= -20 && y <= 20){	// {9:00}
		m1 = -50;
		m2 = 50;
	}
	else if(x >= 0 && y >= 0){
		m1 = 0.75*max(x,y);
		m2 = 0.25*max(x,y);
	}
	else if(x <= 0 && y >= 0){
		m1 = 0.25*max(x,y);
		m2 = 0.75*max(x,y);
	}
	else if(x <= 0 && y <= 0){
		m1 = -0.75*max(x,y);
		m2 = -0.25*max(x,y);
	}
	else{
		m1 = -0.25*max(x,y);
		m2 = -0.75*max(x,y);
	}

	if(m1 < 0){
		motorR->SetDirection(0);
		m1 = -m1;
		motorR->SetValue((uint8_t)m1);
	}
	else{
		motorR->SetDirection(1);
		motorR->SetValue((uint8_t)m1);
	}

	if(m2 < 0){
		motorL->SetDirection(0);
		m2 = -m2;
		motorL->SetValue((uint8_t)m2);
	}
	else{
		motorL->SetDirection(1);
		motorL->SetValue((uint8_t)m2);
	}

}

void sendSupplyVoltage(){

	uint8_t send_info[] = {100, 101, 102};
	HAL_UART_Transmit(&huart2, send_info, 3, 500);
}


inline void UART_Callback(UART_HandleTypeDef *huart){

	if(huart->Instance == USART2)
	{

		if(char2 == 10){

		}
		else if(char2 == 11){
			HAL_UART_Receive(&huart2, &infoX, 1, 5000);
			setRGB(&rgb1, infoX);
		}
		else if(char2 == 12){
			HAL_UART_Receive(&huart2, &infoX, 1, 5000);
			setRGB(&rgb2, infoX);
		}
		else if(char2 == 21)
		{
			HAL_UART_Receive(&huart2, &infoX, 1, 5000);
			HAL_UART_Receive(&huart2, &infoY, 1, 5000);
			setMotors(&motor1, &motor2, infoX, infoY);

		}
		else if(char2 == 22)
		{
			HAL_UART_Receive(&huart2, &infoX, 1, 5000);
			HAL_UART_Receive(&huart2, &infoY, 1, 5000);
			setServos(&servo2, &servo1, infoX, infoY);
		}
		else if(char2 == 51)
		{
			mpu.resetFIFO();
			fifoCount = mpu.getFIFOCount();
			while (fifoCount < packetSize)
				fifoCount = mpu.getFIFOCount();
			mpu.getFIFOBytes(fifoBuffer, packetSize);

			HAL_UART_Transmit(&huart2, &fifoBuffer[0], 1, 500);
			HAL_UART_Transmit(&huart2, &fifoBuffer[1], 1, 500);
			HAL_UART_Transmit(&huart2, &fifoBuffer[4], 1, 500);
			HAL_UART_Transmit(&huart2, &fifoBuffer[5], 1, 500);
			HAL_UART_Transmit(&huart2, &fifoBuffer[8], 1, 500);
			HAL_UART_Transmit(&huart2, &fifoBuffer[9], 1, 500);
			HAL_UART_Transmit(&huart2, &fifoBuffer[12], 1, 500);
			HAL_UART_Transmit(&huart2, &fifoBuffer[13], 1, 500);
			rgb1.Toggle();
		}

		HAL_UART_Receive_IT(&huart2, &char2, 1);
	}

	if(huart->Instance == USART3)
	{


	}
}


extern "C" {
void extern_setup(void) { setup(); }
void extern_loop(void)  { loop();  }
void extern_UART_Callback(UART_HandleTypeDef *huart)  { UART_Callback(huart); }
}
