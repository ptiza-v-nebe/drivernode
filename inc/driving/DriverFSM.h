/*
 * DriverFSM.h
 *
 *  Created on: Apr 10, 2018
 *      Author: max
 */

#ifndef DRIVING_DRIVERFSM_H
#define DRIVING_DRIVERFSM_H

#include <driving/DriverBaseState.h>
#include <driving/PDT1.h>
#include <hal/Motor.h>

class DriverFSM {
private:
	DriverBaseState* currentState;
	Motor& motorLeft;
	Motor& motorRight;
	PDT1 positionControl;
	PDT1 angleControl;

public:
	DriverFSM(Motor& motorLeft, Motor& motorRight);
	void update();
	virtual ~DriverFSM();
};

#endif /* DRIVING_DRIVERFSM_H */
