/*
 * PD.cpp
 *
 *  Created on: Apr 20, 2018
 *      Author: max
 */

#include <driving/PD.h>

PD::PD(float kP, float Tv, float Ts) {
	b0 = (kP*Ts+kP*Tv)/Ts;
	b1 = (-kP*Tv)/Ts;
	e1 = 0;
}

float PD::update(float e) {
	float u = b1*e+b0*e1;
	e1 = e;
	return u;
}

void PD::reset() {
	e1 = 0.0;
}
