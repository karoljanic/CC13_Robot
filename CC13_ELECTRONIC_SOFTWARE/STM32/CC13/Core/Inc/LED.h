#ifndef LED_H_
#define LED_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"
#include <math.h>

#ifdef __cplusplus
}
#endif


//#######################################################
// class  DigitalLED
//#######################################################
// Diode connection:
//  a) Diode connected with the cathode to the uC pin and the anode to the VCC
//          Init(port, pin [, false]);
//
//  b) Diode connected with the cathode to the GND and the anode to the uC pin
//          Init(port, pin, true);
//
//  STM32Cube configuration:
//     GPIO_Output (Output Push Pull: no poll-up and no-pull down)
//#######################################################

class DigitalLED {
private:
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;
	bool inverse;

public:
	inline void Init(GPIO_TypeDef *_GPIOx, uint16_t _GPIO_Pin, bool _inverse = false){
		GPIOx = _GPIOx; GPIO_Pin = _GPIO_Pin;
		inverse = _inverse;
	}

	inline void On(){ HAL_GPIO_WritePin(GPIOx, GPIO_Pin, inverse ? GPIO_PIN_SET: GPIO_PIN_RESET); }
	inline void Off(){ HAL_GPIO_WritePin(GPIOx, GPIO_Pin, inverse ? GPIO_PIN_RESET: GPIO_PIN_SET); }
	inline void Toggle(){ HAL_GPIO_TogglePin(GPIOx, GPIO_Pin); }
};


//#######################################################
// class  DigitalLED_RGB
//#######################################################
// Diode connection:
//  a) Diode connected with the cathode to the uC pin and the anode to the VCC
//          Init(port_R, pin_R, port_G, pin_G, port_B, pin_B [, false]);
//
//  b) Diode connected with the cathode to the GND and the anode to the uC pin
//          Init(port_R, pin_R, port_G, pin_G, port_B, pin_B, true);
//
//  STM32Cube configuration:
//     GPIO_Output (Output Push Pull: no poll-up and no-pull down)
//#######################################################


class DigitalLED_RGB {
private:
	GPIO_TypeDef *GPIOx_R, *GPIOx_G, *GPIOx_B;
	uint16_t GPIO_Pin_R, GPIO_Pin_G, GPIO_Pin_B;
	bool inverse;

public:
	inline void Init(GPIO_TypeDef *_GPIOx_R, uint16_t _GPIO_Pin_R,
			         GPIO_TypeDef *_GPIOx_G, uint16_t _GPIO_Pin_G,
					 GPIO_TypeDef *_GPIOx_B, uint16_t _GPIO_Pin_B,
					 bool _inverse = false){
		GPIOx_R = _GPIOx_R; GPIO_Pin_R = _GPIO_Pin_R;
		GPIOx_G = _GPIOx_G; GPIO_Pin_G = _GPIO_Pin_G;
		GPIOx_B = _GPIOx_B; GPIO_Pin_B = _GPIO_Pin_B;
		inverse = _inverse;
	}

	inline void On_R(){ HAL_GPIO_WritePin(GPIOx_R, GPIO_Pin_R, inverse ? GPIO_PIN_SET: GPIO_PIN_RESET); }
	inline void On_G(){ HAL_GPIO_WritePin(GPIOx_G, GPIO_Pin_G, inverse ? GPIO_PIN_SET: GPIO_PIN_RESET); }
	inline void On_B(){ HAL_GPIO_WritePin(GPIOx_B, GPIO_Pin_B, inverse ? GPIO_PIN_SET: GPIO_PIN_RESET); }
	void On();
	void On(bool r, bool g, bool b);

	inline void Off_R(){ HAL_GPIO_WritePin(GPIOx_R, GPIO_Pin_R, inverse ? GPIO_PIN_RESET: GPIO_PIN_SET); }
	inline void Off_G(){ HAL_GPIO_WritePin(GPIOx_G, GPIO_Pin_G, inverse ? GPIO_PIN_RESET: GPIO_PIN_SET); }
	inline void Off_B(){ HAL_GPIO_WritePin(GPIOx_B, GPIO_Pin_B, inverse ? GPIO_PIN_RESET: GPIO_PIN_SET); }
	void Off();

	inline void Toggle_R(){ HAL_GPIO_TogglePin(GPIOx_R, GPIO_Pin_R); }
	inline void Toggle_G(){ HAL_GPIO_TogglePin(GPIOx_G, GPIO_Pin_G); }
	inline void Toggle_B(){ HAL_GPIO_TogglePin(GPIOx_B, GPIO_Pin_B); }
	void Toggle();
};


//#######################################################
// class  AnalogLED
//#######################################################
// Diode connection:
//  a) cathode -> pin uC (PWM Output), anode -> VCC
//     AnalogLED(timer, channel [, false]);
//
//  b) cathode -> GND, anode -> pin uC (PWM Output)
//     AnalogLED(timer, channel [, false]);
//
//  c) cathode -> pin uC (PWMN Output), anode -> VCC
//     AnalogLED(timer, channel, true);
//
//  d) cathode -> GND, anode -> pin uC (PWMN Output)
//     AnalogLED(timer, channel, true);
//
//  STM32Cube configuration(x - timer number, y - channel number):
//     TIMx_CHy (Channel_y PWM Generation CHy)
//
//        TIMx
//          Clock Source:    Internal Clock
//          Channel_y:        PWM Generation CHy
//        Counter Mode:    Up
//		  Counter Period:  period    (the counter counts from 0 to the period)
//#######################################################

class AnalogLED {
private:
	TIM_HandleTypeDef *htim;
	uint32_t channel;
	uint32_t period;

public:
	inline void Init(TIM_HandleTypeDef *_htim, uint32_t _channel,
			bool _complementary_output = false) {
		htim = _htim;
		channel = _channel;
		period = _htim->Init.Period;
		if (_complementary_output)
			HAL_TIMEx_PWMN_Start(htim, channel);
		else
			HAL_TIM_PWM_Start(htim, channel);
	}

	inline void On() { __HAL_TIM_SET_COMPARE(htim, channel, period); }
	inline void Off(){ __HAL_TIM_SET_COMPARE(htim, channel, 0); }
	inline void Set(uint32_t value){
		if (value > period) value = period;
		__HAL_TIM_SET_COMPARE(htim, channel, value);
	}
	inline uint32_t Get(){ return __HAL_TIM_GET_COMPARE(htim, channel); }
	inline void Toggle(){ Set(period - Get()); }
	void SetN(double x);
	double GetN();
	inline void ToggleN(){ SetN(1.0 - GetN()); }
	inline uint32_t GetPeriod(){ return period; }
};


//#######################################################
// class  AnalogLED_RGB
//#######################################################
// Diode connection:
//  a) cathode -> pin uC (PWM Output), anode -> VCC
//     AnalogLED(timer, channel [, false]);
//
//  b) cathode -> GND, anode -> pin uC (PWM Output)
//     AnalogLED(timer, channel [, false]);
//
//  c) cathode -> pin uC (PWMN Output), anode -> VCC
//     AnalogLED(timer, channel, true);
//
//  d) cathode -> GND, anode -> pin uC (PWMN Output)
//     AnalogLED(timer, channel, true);
//
//  STM32Cube configuration(x - timer number, y - channel number):
//     TIMx_CHy (Channel PWM Generation CHy)
//        TIMx
//          Clock Source:    Internal Clock
//          Channel_y:        PWM Generation CHy
//        Counter Mode:    Up
//		  Counter Period:  period    (the counter counts from 0 to the period)
//#######################################################

class AnalogLED_RGB {
private:
	TIM_HandleTypeDef *htim;
	uint32_t channel_R, channel_G, channel_B;
	uint32_t period;

public:
	inline void Init(TIM_HandleTypeDef *_htim,
			uint32_t _channel_R, uint32_t _channel_G, uint32_t _channel_B,
			bool _complementary_output = false) {
		htim = _htim;
		channel_R = _channel_R;
		channel_G = _channel_G;
		channel_B = _channel_B;
		period = _htim->Init.Period;
		if (_complementary_output) {
			HAL_TIMEx_PWMN_Start(htim, channel_R);
			HAL_TIMEx_PWMN_Start(htim, channel_G);
			HAL_TIMEx_PWMN_Start(htim, channel_B);
		} else {
			HAL_TIM_PWM_Start(htim, channel_R);
			HAL_TIM_PWM_Start(htim, channel_G);
			HAL_TIM_PWM_Start(htim, channel_B);
		}
	}

	inline void Init(TIM_HandleTypeDef *_htim,
				uint32_t _channel_R, uint32_t _channel_G, uint32_t _channel_B,
				bool _comp_out_R, bool _comp_out_G, bool _comp_out_B) {
			htim = _htim;
			channel_R = _channel_R;
			channel_G = _channel_G;
			channel_B = _channel_B;
			period = _htim->Init.Period;
			if (_comp_out_R) { HAL_TIMEx_PWMN_Start(htim, channel_R); }
				else { HAL_TIM_PWM_Start(htim, channel_R); }
			if (_comp_out_G) { HAL_TIMEx_PWMN_Start(htim, channel_G); }
							else { HAL_TIM_PWM_Start(htim, channel_G); }
			if (_comp_out_B) { HAL_TIMEx_PWMN_Start(htim, channel_B); }
							else { HAL_TIM_PWM_Start(htim, channel_B); }
		}

	inline void On_R() { __HAL_TIM_SET_COMPARE(htim, channel_R, period); }
	inline void On_G() { __HAL_TIM_SET_COMPARE(htim, channel_G, period); }
	inline void On_B() { __HAL_TIM_SET_COMPARE(htim, channel_B, period); }
	void On();

	inline void Off_R(){ __HAL_TIM_SET_COMPARE(htim, channel_R, 0); }
	inline void Off_G(){ __HAL_TIM_SET_COMPARE(htim, channel_G, 0); }
	inline void Off_B(){ __HAL_TIM_SET_COMPARE(htim, channel_B, 0); }
	void Off();

	inline void Set_R(uint32_t value){
		if (value > period) value = period;
		__HAL_TIM_SET_COMPARE(htim, channel_R, value);
	}
	inline void Set_G(uint32_t value){
			if (value > period) value = period;
			__HAL_TIM_SET_COMPARE(htim, channel_G, value);
		}
	inline void Set_B(uint32_t value){
			if (value > period) value = period;
			__HAL_TIM_SET_COMPARE(htim, channel_B, value);
		}
	void Set(uint32_t value_r, uint32_t value_g, uint32_t value_b);
	void Set(uint32_t value);
	inline uint32_t Get_R(){ return __HAL_TIM_GET_COMPARE(htim, channel_R); }
	inline uint32_t Get_G(){ return __HAL_TIM_GET_COMPARE(htim, channel_G); }
	inline uint32_t Get_B(){ return __HAL_TIM_GET_COMPARE(htim, channel_B); }

	inline void Toggle_R(){ Set_R(period - Get_R()); }
	inline void Toggle_G(){ Set_G(period - Get_G()); }
	inline void Toggle_B(){ Set_B(period - Get_B()); }
	inline void Toggle(){ Toggle_R(); Toggle_G(); Toggle_B(); }

	inline void SetN_R(double x){ Set_R((uint32_t)(powf(x, 2.2) * period)); }
	inline void SetN_G(double x){ Set_G((uint32_t)(powf(x, 2.2) * period)); }
	inline void SetN_B(double x){ Set_B((uint32_t)(powf(x, 2.2) * period)); }
	void SetN(double r, double g, double b);
	void SetN(double x);
	double GetN_R();
	double GetN_G();
	double GetN_B();
	inline void ToggleN_R(){ SetN_R(1.0 - GetN_R()); }
	inline void ToggleN_G(){ SetN_G(1.0 - GetN_G()); }
	inline void ToggleN_B(){ SetN_B(1.0 - GetN_B()); }
	inline void ToggleN(){ ToggleN_R(); ToggleN_G(); ToggleN_B(); }


	inline uint32_t GetPeriod(){ return period; }
};

#endif /* LED_H_ */
