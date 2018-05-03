/*
 * PIController.h
 *
 *  Created on: Apr 22, 2018
 *      Author: max
 */

#ifndef DRIVING_PICONTROLLER_H_
#define DRIVING_PICONTROLLER_H_

class PIController {
	float b0, b1;
	float e1, u1;
public:
	PIController(float kr, float Tr, float Ts);
	float update(float e0);
	void reset();
public:
	PIController();
};

#endif /* DRIVING_PICONTROLLER_H_ */
