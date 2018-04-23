/*
 * DriverFSM.cpp
 *
 *  Created on: Apr 10, 2018
 *      Author: max
 */

#include <cmath>
#include "driving/DriverFSM.h"
#include "driving/DriverBaseState.h"
#include "driving/DriverStates.h"
#include "constants.h"
#include "hal/Motor.h"
#include "position/PositionManager.h"
#include "position/calibration_parameters.h"
#include "util/util.h"

DriverFSM::DriverFSM(Motor& motorLeft, Motor& motorRight, PositionManager& pm) :
		currentState(nullptr),
		leftMotor(motorLeft),
		rightMotor(motorRight),
		positionControl(
				CONTROLLER_POSITION_KP,
				CONTROLLER_SYSTEM_DELAYTIME,
				0.0005,
				CONTROLLER_SAMPLING_TIME),
		angleControl(
				CONTROLLER_ANGLE_KP,
				CONTROLLER_SYSTEM_DELAYTIME,
				0.0,
				CONTROLLER_SAMPLING_TIME),
		leftWheelControl(
				CONTROLLER_SYSTEM_LEFT_WHEEL_KP,
				CONTROLLER_SYSTEM_LEFT_WHEEL_DELAYTIME,
				CONTROLLER_SAMPLING_TIME),
		rightWheelControl(
				CONTROLLER_SYSTEM_RIGHT_WHEEL_KP,
				CONTROLLER_SYSTEM_RIGHT_WHEEL_DELAYTIME,
				CONTROLLER_SAMPLING_TIME),
		pm(pm),
		referencePosition(pm.getPosition()),
		referenceAngle(pm.getHeading()),
		driveSpeed(DriveSpeed::SLOW),
		driveDirection(DriveDirection::FORWARD),
		driveAccuracy(DriveAccuracy::LOW)
		{
	currentState = new Idle(*this);
}

void DriverFSM::update() {
	currentState->doAction();
}

void DriverFSM::updateControl() {
	// distanceError
	if(distanceError < referenceDistance) {
		distanceError = distanceError * 0.5*CONTROLLER_SAMPLING_TIME; // ramp up to reference Position
	}

	// angleError
	Vector targetVector(pm.xum*0.001, pm.yum*0.001);
	referenceAngle = (referencePosition.asVectorFromOrigin() - targetVector).getPolarAngle();
	angleError = referenceAngle - pm.getHeading();

	float referenceSpeedLeft = distanceError-angleError;
	float referenceSpeedRight = distanceError+angleError;

	float leftMotorVelocity = leftWheelControl.update(referenceSpeedLeft - pm.leftWheelVelocity);
	float rightMotorVelocity = rightWheelControl.update(referenceSpeedRight - pm.rightWheelVelocity);

	leftMotorVelocity = clamp(leftMotorVelocity, -1.5, 1.5);
	rightMotorVelocity = clamp(rightMotorVelocity, -1.5, 1.5);

	leftMotor.setSpeed(leftMotorVelocity*MOTORCONSTANT);
	rightMotor.setSpeed(rightMotorVelocity*MOTORCONSTANT);

	n++; // counting tick up
	printf("%f %d %d %f %f %f %f %f\r\n",n*CONTROLLER_SAMPLING_TIME, pm.getPosition().x, pm.getPosition().y, sollDistance, distanceToTarget, angleError, pm.getHeading().getAngleInRadianAround(0.0));
}

void DriverFSM::resetControl() {
	positionControl.reset();
	angleControl.reset();
}

void DriverFSM::enableMotors() {
	leftMotor.enable();
	rightMotor.enable();
}

void DriverFSM::disableMotors() {
	leftMotor.disableAndStop();
	rightMotor.disableAndStop();
}

bool DriverFSM::referencePositionReached() {
	float distance = pm.getPosition().distanceTo(referencePosition);
	Vector targetVector(pm.xum*0.001, pm.yum*0.001);
	Angle soll = (referencePosition.asVectorFromOrigin() - targetVector).getPolarAngle();
	Angle angleError = (soll - pm.getHeading());

	if(distance < 2.5 && angleError.getAngleInDegreesAround(0.0) < 1.0) {
		return true;
	} else {
		return false;
	}
}

bool DriverFSM::referenceAngleReached() {
	if(driveAccuracy == DriveAccuracy::HIGH) {
		if(angleError)
	}
}

bool DriverFSM::isErrorAngleToBig() {
	if(fabs((referenceAngle - pm.getHeading()).getAngleInDegreesAround(0.0)) > 10.0) {
		return true;
	} else {
		return false;
	}
}

DriverFSM::~DriverFSM() {
	delete currentState;
}

void DriverFSM::newPosition() {
	currentState->newPosition();
}

void DriverFSM::newAngle() {
	currentState->newAngle();
}

void DriverFSM::stop() {
	currentState->stop();
}

void DriverFSM::setReferencePosition(Position position) {
	this->referencePosition = position;
}

void DriverFSM::setTargetAngle(Angle angle) {
	this->referenceAngle = angle;
}

void DriverFSM::setDriveSpeed(DriveSpeed speed) {
	this->driveSpeed = speed;
}

void DriverFSM::setDriveDirection(DriveDirection direction) {
	this->driveDirection = direction;
}

void DriverFSM::setDriveAccuracy(DriveAccuracy accuracy) {
	this->driveAccuracy = accuracy;
}
