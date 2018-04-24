/*
 * PIDController.cpp
 *
 *  Created on: Apr 21, 2018
 *      Author: max
 */

#include "driving/PIDController.h"

PIDController::PIDController(float kP, float Ti, float kD, float Ts) {
	b2 = kP+kP*Ts/(2*Ti)+kP*kD/Ts;
	b1 = -kP+kP*Ts/(2*Ti)-2*kP*kD/Ts;
	b0 = kP*kD/Ts;
	u1 = 0.0;
	e1 = 0.0;
	e2 = 0.0;
}

float PIDController::update(float e) {
	float u = 0.0;

	u = u1+b2*e+b1*e1+b0*e2;
	e2 = e1;
	e1 = e;
	u1 = u;

	return u;
}

void PIDController::reset() {
	u1 = 0.0;
	e1 = 0.0;
	e2 = 0.0;
}
