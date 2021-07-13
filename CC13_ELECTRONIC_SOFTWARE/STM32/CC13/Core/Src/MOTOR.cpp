/*
 * MOTOR.cpp
 *
 *  Created on: May 2, 2020
 *      Author: Karol Janic
 */

#include <math.h>
#include "MOTOR.h"


/// ServoMotor

void ServoMotor::Init(TIM_HandleTypeDef *_htim, uint32_t _channel, uint32_t _angle_min, uint32_t _angle_max, bool _mode){

	htim = _htim;
	channel = _channel;

	angle_min = _angle_min;
	angle_max = _angle_max;

	pwm_min = 1000;
	pwm_max = 2000;

	mode = _mode;

	HAL_TIM_PWM_Start(htim, channel);

	SetNormal();
}

void ServoMotor::SetAngle(uint32_t angle){

	if(angle>angle_max)
		angle = angle_max;
	if(angle<angle_min)
		angle = angle_min;

	double step = double(pwm_max-pwm_min)/double(angle_max-angle_min);

	if(mode)
		pwm = uint32_t(pwm_min + (angle-angle_min)*step+0.5);
	else
		pwm = uint32_t(pwm_max - (angle-angle_min)*step-0.5);

	__HAL_TIM_SET_COMPARE(htim, channel, pwm);
}

void ServoMotor::SetAngleProcent(uint32_t procent){

	uint32_t power = procent;
	if(power<0)
		power = 0;
	if(power>100)
		power = 100;

	double step = 1.8;
	double result = step*power;

	SetAngle(result);
}

void ServoMotor::SetNormal(){

	SetAngle(uint32_t((angle_max-angle_min)/2)+0.5);
}

void ServoMotor::ChangeMode(){

	mode = !mode;
}

void ServoMotor::PwmClarify(uint32_t _pwm_min, uint32_t _pwm_max){

	pwm_min = _pwm_min;
	pwm_max = _pwm_max;
}


/// DCMotor

void DCMotor::Init(TIM_HandleTypeDef *_htim,  uint32_t _channel,
			  GPIO_TypeDef *_GPIOx_Dir1, uint16_t _GPIO_Pin_Dir1,
			  GPIO_TypeDef *_GPIOx_Dir2, uint16_t _GPIO_Pin_Dir2, bool _mode){

	htim = _htim;
	channel = _channel;
	period = htim->Init.Period;

	GPIOx_Dir1 = _GPIOx_Dir1;
	GPIO_Pin_Dir1 = _GPIO_Pin_Dir1;

	GPIOx_Dir2 = _GPIOx_Dir2;
	GPIO_Pin_Dir2 = _GPIO_Pin_Dir2;

	mode = _mode;
	currentMode = _mode;

	currentValue = 0;

	HAL_TIM_PWM_Start(htim, channel);

	Off();
}

void DCMotor::SetValue(uint8_t _value){

	value = _value;
}

void DCMotor:: ChangeDirection(){

	mode = !mode;
}

void DCMotor:: SetDirection(bool direction){

	mode = direction;
}

void DCMotor::Off(){

	HAL_GPIO_WritePin(GPIOx_Dir1, GPIO_Pin_Dir1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOx_Dir2, GPIO_Pin_Dir2, GPIO_PIN_RESET);
	__HAL_TIM_SET_COMPARE(htim, channel, 0);
}

void DCMotor::On(uint32_t power){

	if(power<0)
		power = 0;
	if(power>100)
		power = 100;

	period  = 20000;
	double value = period/100.0*power;


	if(mode){

		HAL_GPIO_WritePin(GPIOx_Dir1, GPIO_Pin_Dir1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOx_Dir2, GPIO_Pin_Dir2, GPIO_PIN_SET);
		__HAL_TIM_SET_COMPARE(htim, channel, uint32_t(value));
	}
	else {

		HAL_GPIO_WritePin(GPIOx_Dir1, GPIO_Pin_Dir1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOx_Dir2, GPIO_Pin_Dir2, GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE(htim, channel, uint32_t(value));
	}
}

void DCMotor::Update(){

	if(value == currentValue && mode == currentMode)
		return;

/*	uint8_t x;
	if(mode){
		if(value < currentValue){
			x = currentValue-10;
			if(currentValue < 10 && currentValue >= 0)
				ChangeDirection();
			if(x<value)
				x = value;
			currentValue = x;
		}
		else{
			x = currentValue+10;
			if(currentValue < 0 && currentValue >= -10)
				ChangeDirection();
			if(x>value)
				x = value;
			currentValue = x;
		}
	}

	On(currentValue);	*/

	float a,b,x;

	if(mode)
		a = value;
	else
		a = -value;

	if(currentMode)
		b = currentValue;
	else
		b = -currentValue;

	if(a < b){
		x = b-10;
		if(x<a)
			x = a;
		b = x;
	}
	else{
		x = b+10;
		if(x>a)
			x = a;
		b = x;
	}

	if(b < 0){
		currentMode = 0;
		currentValue = -b;
		SetDirection(0);
		On(-b);
	}
	else{
		currentMode = 1;
		currentValue = b;
		SetDirection(1);
		On(b);
	}

}


// Encoder

void Encoder::Init(TIM_HandleTypeDef *_htim, uint32_t first_channel, uint32_t _max_impulse){

	htim = _htim;
	channel = first_channel;
	max_impulse = _max_impulse;
	max_impulse--;

	HAL_TIM_Encoder_Start(htim, TIM_CHANNEL_ALL);
	htim -> Instance->ARR = max_impulse;
}


uint32_t Encoder::Read(){

	return HAL_TIM_ReadCapturedValue(htim, channel);
}



