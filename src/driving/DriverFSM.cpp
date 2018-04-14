/*
 * DriverFSM.cpp
 *
 *  Created on: Apr 10, 2018
 *      Author: max
 */

#include "driving/DriverFSM.h"
#include "driving/DriverBaseState.h"
#include "driving/DriverStates.h"
#include "constants.h"
#include "hal/Motor.h"
#include "position/PositionManager.h"
#include "util/util.h"

DriverFSM::DriverFSM(Motor& motorLeft, Motor& motorRight, PositionManager& pm) :
		currentState(nullptr),
		leftMotor(motorLeft),
		rightMotor(motorRight),
		positionControl(CONTROLLER_SYSTEM_POSITION_GAIN,
				CONTROLLER_SYSTEM_POSITION_DELAYTIME,
				CONTROLLER_SAMPLING_TIME),
		angleControl(
				CONTROLLER_SYSTEM_ANGLE_GAIN,
				CONTROLLER_SYSTEM_ANGLE_DELAYTIME,
				CONTROLLER_SAMPLING_TIME),
		pm(pm),
		targetPosition(pm.getPosition()),
		targetAngle(pm.getHeading()),
		speed(DriveSpeed::SLOW),
		direction(DriveDirection::FORWARD) {
	currentState = new Idle(*this);
}

void DriverFSM::update() {
	currentState->doAction();
}

void DriverFSM::updateControl() {
	// read inputs
	double distance = pm.getPosition().distanceTo(targetPosition);
	double angle = (targetAngle - pm.getHeading()).getAngleInDegrees();

	// calculate next value
	double linearSpeed = positionControl.calc(distance);
	double turningSpeed = angleControl.calc(angle);

	if(speed == DriveSpeed::SLOW) {
		linearSpeed = clamp(linearSpeed, -1000.0, 1000.0);
		turningSpeed = clamp(turningSpeed, -1000.0, 1000.0);
	} else {
		linearSpeed = clamp(linearSpeed, -10000.0, 10000.0);
		turningSpeed = clamp(turningSpeed, -10000.0, 10000.0);
	}

	if(direction == DriveDirection::BACKWARD) {
		linearSpeed = -linearSpeed;
	}

	// set speed
	leftMotor.setSpeed(linearSpeed-turningSpeed);
	rightMotor.setSpeed(linearSpeed+turningSpeed);
}

void DriverFSM::resetControl() {
	positionControl.reset();
	angleControl.reset();
}

void DriverFSM::stop() {
	leftMotor.stop();
	rightMotor.stop();
}

bool DriverFSM::reachedTargetPosition() {
	double distance = pm.getPosition().distanceTo(targetPosition);

	if(distance < 2.0) {
		return true;
	} else {
		return false;
	}
}

DriverFSM::~DriverFSM() {
	delete currentState;
}

void DriverFSM::setTargetPosition(Position targetPosition) {
	this->targetPosition = targetPosition;
}

void DriverFSM::setTargetAngle(Angle targetAngle) {
	this->targetAngle = targetAngle;
}

void DriverFSM::setDriveSpeed(DriveSpeed speed) {
	this->speed = speed;
}

void DriverFSM::setDriveDirection(DriveDirection direction) {
	this->direction = direction;
}
