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

void DriverFSM::updateControl() {
	// read inputs
	double distance = pm.getPosition().distanceTo(targetPosition);
	double angle = (targetAngle - pm.getHeading()).getAngleInRadian();

	// calculate next value
	double linearSpeed = positionControl.calc(distance);
	double turningSpeed = angleControl.calc(angle);

	// set speed
	leftMotor.setSpeed(linearSpeed-turningSpeed);
	rightMotor.setSpeed(linearSpeed+turningSpeed);
}

void DriverFSM::resetControl() {
	positionControl.reset();
	angleControl.reset();
}

DriverFSM::~DriverFSM() {
	delete currentState;
}
