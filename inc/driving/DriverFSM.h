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
#include "serial/MessageDispatcher.h"

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
	MessageDispatcher& md;


	Position referencePosition;
	float distanceError = 0;
	float referenceDistance = 0;
	float rampDistance = 0;
	float startDistance = 0;

	Angle referenceAngle;
	Angle angleError;
	Angle startAngle;
	Angle rampAngle;

	DriveSpeed driveSpeed;
	DriveDirection driveDirection;
	DriveAccuracy driveAccuracy;

	int n = 0;

public:
	DriverFSM(Motor& motorLeft, Motor& motorRight, PositionManager& pm, MessageDispatcher& md);
	void update();
	void updateControl();
	void resetControl();
	void enableMotors();
	void disableMotors();
	bool referencePositionReached();
	void calculateDistance();
	void calculateAngle();
	void sendFinishedMessage();
	virtual ~DriverFSM();

	// transitions
	void newPosition();
	void newAngle();
	void stop();

	// setter
	void setReferencePosition(Position position);
	void setReferenceAngle(Angle angle);
	void setDriveSpeed(DriveSpeed speed);
	void setDriveDirection(DriveDirection direction);
	void setDriveAccuracy(DriveAccuracy accuracy);
};

#endif /* DRIVING_DRIVERFSM_H */
