/*
 * PIController.cpp
 *
 *  Created on: Apr 22, 2018
 *      Author: max
 */

#include <PIController.h>

/**
 * PI-Controller in Parallelform
 */
PIController::PIController(float kP, float Ti, float Ts) {
	b0 = kP+(kP*Ts)/(2*Ti);
	b1 = (kP*Ts)/(2*Ti)-kP;
	e1 = 0;
	u1 = 0;
}

float PIController::update(float e0) {
	float u = b0*e0+b1*e1+u1;
	e1 = e0;
	u1 = u;
	return u;
}

void PIController::reset() {
	e1 = 0.0;
	u1 = 0.0;
}

