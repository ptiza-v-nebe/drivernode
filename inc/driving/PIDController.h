/*
 * PIDController.h
 *
 *  Created on: Apr 21, 2018
 *      Author: max
 */

#ifndef DRIVING_PIDCONTROLLER_H_
#define DRIVING_PIDCONTROLLER_H_

class PIDController {
private:
	float b2,b1,b0;
	float u1,e1,e2;
public:
	PIDController(float kP, float Ti, float kD, float Ts);
	float update(float e);
	void reset();
};

#endif /* DRIVING_PIDCONTROLLER_H_ */
