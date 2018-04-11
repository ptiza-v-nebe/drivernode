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

Motor& DriverFSM::getLeftMotor() {
	return leftMotor;
}

Motor& DriverFSM::getRightMotor() {
	return rightMotor;
}

PDT1& DriverFSM::getPositionController() {
	return positionControl;
}

PDT1& DriverFSM::getAngleController() {
	return angleControl;
}

PositionManager& DriverFSM::getPositionManager() {
	return pm;
}

Position& DriverFSM::getTargetPosition() {
	return targetPosition;
}

Angle& DriverFSM::getTargetAngle() {
	return targetAngle;
}

DriveSpeed& DriverFSM::getDriveSpeed() {
	return speed;
}

DriveDirection& DriverFSM::getDriveDirection() {
	return direction;
}

DriverFSM::~DriverFSM() {
	delete currentState;
}
