/*
 * PD.cpp
 *
 *  Created on: Apr 20, 2018
 *      Author: max
 */

#include "driving/PDController.h"

PDController::PDController(float kP, float kD, float Ts) {
	b0 = (kP*Ts+kP*kD)/Ts;
	b1 = (-kP*kD)/Ts;
	e1 = 0;
}

float PDController::update(float e0) {
	float u = b0*e0+b1*e1;
	e1 = e0;
	return u;
}

void PDController::reset() {
	e1 = 0.0;
}
