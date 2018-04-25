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

#include <control/Clocked.h>

class DriverBaseState;

class DriverFSM : public Clocked {
private:
	DriverBaseState* currentState;
	Motor& leftMotor;
	Motor& rightMotor;
	PIController leftWheelControl;
	PIController rightWheelControl;
	PositionManager& pm;
	MessageDispatcher& md;

	Position referencePosition;
	float currentDistance;
	float rampDistance;
	float distanceError;

	Angle referenceAngle;
	Angle angleError;
	Angle startAngle;
	Angle rampAngle;

	float leftMotorVelocity;
	float rightMotorVelocity;
	DriveSpeed driveSpeed;
	DriveDirection driveDirection;
	DriveAccuracy driveAccuracy;

	float referenceSpeedLeft;
	float referenceSpeedRight;

	float speed;
	int direction;
	float targetRadius;
	bool turningAngle;
	int n = 0;

public:
	DriverFSM(Motor& motorLeft, Motor& motorRight, PositionManager& pm, MessageDispatcher& md);
	void tick() override;
	void updateControl();
	void resetControl();
	void enableMotors();
	void disableMotors();
	void stopMotors();
	bool referencePositionReached();
	void calculateDistance();
	void calculateAngle();
	bool isAccuracyHigh();
	bool isRobotStuck();
	int pointOnWhichSideOfLine(Vector ro, Vector rd, Position p);
	void sendFinishedMessage();
	void sendStuckMessage();
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
