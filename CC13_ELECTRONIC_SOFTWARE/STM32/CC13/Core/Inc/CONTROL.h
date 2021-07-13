/*
 * CONTROL.h
 *
 *  Created on: May 2, 2020
 *      Author: Karol Janic
 */

#ifndef CONTROL_H_
#define CONTROL_H_

#include "LED.h"
#include "MOTOR.h"

inline void setRGB(DigitalLED_RGB *led, int info);

inline void setServos(ServoMotor *topServo, ServoMotor *bottomServo, int infoX, int infoY);

inline void setMotors(DCMotor *motorR, DCMotor *motorL, int infoX, int infoY);

#endif /* CONTROL_H_ */
