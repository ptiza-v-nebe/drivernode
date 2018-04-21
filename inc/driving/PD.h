/*
 * PD.h
 *
 *  Created on: Apr 20, 2018
 *      Author: max
 */

#ifndef DRIVING_PD_H_
#define DRIVING_PD_H_

class PD {
private:
	float b0, b1;
	float e1;
public:
	PD(float kP, float Tv, float Ts);
	float update(float e);
	void reset();
};

#endif /* DRIVING_PD_H_ */
