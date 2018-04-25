/*
 * PD.h
 *
 *  Created on: Apr 20, 2018
 *      Author: max
 */

#ifndef DRIVING_PDCONTROLLER_H_
#define DRIVING_PDCONTROLLER_H_

class PDController {
private:
	float b0, b1;
	float e1;
public:
	PDController(float kP, float kD, float Ts);
	float update(float e);
	void reset();
};

#endif /* DRIVING_PDCONTROLLER_H_ */
