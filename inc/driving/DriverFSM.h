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

	DriveSpeed driveSpeed;
	DriveDirection driveDirection;
	DriveAccuracy driveAccuracy;

	Position referencePosition;
	float distanceError = 0;
	float referenceDistance = 0;
	float distanceRamp = 0;

	Angle referenceAngle;
	float angleError = 0;

	int n = 0;

public:
	DriverFSM(Motor& motorLeft, Motor& motorRight, PositionManager& pm);
	void update();
	void updateControl();
	void resetControl();
	void enableMotors();
	void disableMotors();
	bool referencePositionReached();
	bool referenceAngleReached();
	bool isErrorAngleToBig();
	virtual ~DriverFSM();

	// transitions
	void newPosition();
	void newAngle();
	void stop();

	// setter
	void setReferencePosition(Position position);
	void setTargetAngle(Angle targetAngle);
	void setDriveSpeed(DriveSpeed speed);
	void setDriveDirection(DriveDirection direction);
	void setDriveAccuracy(DriveAccuracy accuracy);
};

#endif /* DRIVING_DRIVERFSM_H */
