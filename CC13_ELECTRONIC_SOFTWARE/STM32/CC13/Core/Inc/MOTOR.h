/*
 * MOTOR.h
 *
 *  Created on: May 2, 2020
 *      Author: Karol Janic
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"

#ifdef __cplusplus
}
#endif


//#######################################################
// class  SERVOMOTOR
//#######################################################
// przewód sygnału podłączony pod timer
// o częstotliwości 50 Hz.
//#######################################################

class ServoMotor
{

private:
	TIM_HandleTypeDef *htim;
	uint32_t channel;
	uint32_t range;
	uint32_t angle_min, angle_max;
	uint32_t pwm_min, pwm_max;
	double step;
	bool mode; // kierunek liczenia kąta
	uint32_t pwm;

public:
	void Init(TIM_HandleTypeDef *_htim, uint32_t _channel, uint32_t _angle_min, uint32_t _angle_max, bool _mode);

	void SetAngle(uint32_t angle);

	void SetAngleProcent(uint32_t procent);

	void SetNormal();

	void ChangeMode();

	void PwmClarify(uint32_t _pwm_min, uint32_t _pwm_max);
};


//#######################################################
// class  DC_MOTOR
//#######################################################
class DCMotor{

private:
	TIM_HandleTypeDef *htim;
	uint32_t channel;
	uint32_t period;
	GPIO_TypeDef * GPIOx_Dir1, *GPIOx_Dir2;
	uint16_t GPIO_Pin_Dir1, GPIO_Pin_Dir2;
	bool mode;
	bool currentMode;
	uint8_t value;
	uint8_t currentValue;


public:
	void Init(TIM_HandleTypeDef *_htim,  uint32_t channel,
			  GPIO_TypeDef *_GPIOx_Dir1, uint16_t _GPIO_Pin_Dir1,
			  GPIO_TypeDef *_GPIOx_Dir2, uint16_t _GPIO_Pin_Dir2, bool _clockwise);

	void SetValue(uint8_t _value);

	void ChangeDirection();

	void SetDirection(bool direction);

	void Off();

	void On(uint32_t power);

	void Update();
};


//#######################################################
// class ENCODER
//#######################################################
// Encoder Mode: Encoder Mode TI1
// Polarity: RisingEdge
// IC Selection: Direct
// Prescaler Division Ratio: No division
//#######################################################
class Encoder{

private:
	TIM_HandleTypeDef *htim;
	uint32_t channel;
	uint32_t max_impulse;


public:
	void Init(TIM_HandleTypeDef *_htim, uint32_t first_channel, uint32_t _max_impulse);

	uint32_t Read();

};



#endif /* MOTOR_H_ */

