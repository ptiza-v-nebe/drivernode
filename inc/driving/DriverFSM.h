/*
 * DriverFSM.h
 *
 *  Created on: Apr 10, 2018
 *      Author: max
 */

#ifndef DRIVING_DRIVERFSM_H
#define DRIVING_DRIVERFSM_H

#include "driving/DriverBaseState.h"
#include "driving/PIDController.h"
#include "driving/PDController.h"
#include "driving/PIController.h"
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
	PIDController positionControl;
	PIDController angleControl;
	PIController leftWheelControl;
	PIController rightWheelControl;
	PositionManager& pm;
	Position targetPosition;
	Angle targetAngle;
	DriveSpeed driveSpeed;
	DriveDirection driveDirection;

	int n = 0;
	float lastSpeedLeft = 0;
	float lastSpeedRight = 0;
	float sollLeft = 0;
	float sollRight = 0;
	float sollDistance = 0;
	float lastDistance = 0;
	float targetDistance = 0;

public:
	DriverFSM(Motor& motorLeft, Motor& motorRight, PositionManager& pm);
	void update();
	void updateControl();
	void resetControl();
	void stop();
	bool reachedTargetPosition();
	virtual ~DriverFSM();

	// transitions
	void newTargetPosition();

	void setTargetPosition(Position targetPosition);
	void setTargetAngle(Angle targetAngle);
	void setDriveSpeed(DriveSpeed speed);
	void setDriveDirection(DriveDirection direction);
};

#endif /* DRIVING_DRIVERFSM_H */
