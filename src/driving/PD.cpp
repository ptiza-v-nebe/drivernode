/*
 * PD.cpp
 *
 *  Created on: Apr 20, 2018
 *      Author: max
 */

#include "driving/PD.h"

PD::PD(float kP, float Tv, float Ts) {
	b0 = (kP*Ts+kP*Tv)/Ts;
	b1 = (-kP*Tv)/Ts;
	e1 = 0;
}

float PD::update(float e0) {
	float u = b0*e0+b1*e1;
	e1 = e0;
	return u;
}

void PD::reset() {
	e1 = 0.0;
}
