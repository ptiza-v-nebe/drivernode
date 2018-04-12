/*
 * DriverFSM.h
 *
 *  Created on: Apr 10, 2018
 *      Author: max
 */

#ifndef DRIVING_DRIVERFSM_H
#define DRIVING_DRIVERFSM_H

#include "driving/DriverBaseState.h"
#include "driving/PDT1.h"
#include "hal/Motor.h"
#include "position/PositionManager.h"
#include "position/Position.h"
#include "position/Angle.h"
#include "serial/messages/drive_types.h"

class DriverBaseState;

class DriverFSM {
private:
	DriverBaseState* currentState;
	Motor& leftMotor;
	Motor& rightMotor;
	PDT1 positionControl;
	PDT1 angleControl;
	PositionManager& pm;
	Position targetPosition;
	Angle targetAngle;
	DriveSpeed speed;
	DriveDirection direction;

public:
	DriverFSM(Motor& motorLeft, Motor& motorRight, PositionManager& pm);
	void update();
	void updateControl();
	void resetControl();
	virtual ~DriverFSM();
};

#endif /* DRIVING_DRIVERFSM_H */
