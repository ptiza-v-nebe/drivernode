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
		targetPosition(pm.getPosition()),
		targetAngle(pm.getHeading()),
		driveSpeed(DriveSpeed::SLOW),
		driveDirection(DriveDirection::FORWARD)
		{
	currentState = new Idle(*this);
}

void DriverFSM::update() {
	currentState->doAction();
}

void DriverFSM::updateControl() {
	// read inputs
	Vector targetVector(pm.xum*0.001, pm.yum*0.001);

	Angle soll = (targetPosition.asVectorFromOrigin() - targetVector).getPolarAngle();

	float angle = (soll - pm.getHeading()).getAngleInRadianAround(0.0);

	float distanceToTarget = pm.getPosition().distanceTo(targetPosition)*0.001;

	if(sollDistance < targetDistance) {
		sollDistance = sollDistance + 0.5*CONTROLLER_SAMPLING_TIME;
	}

	if(angle > 10*(PI/180)) {
		sollDistance = 0;
	}

	sollLeft = (sollDistance - (targetDistance - distanceToTarget))-0.25*angle;
	sollRight = (sollDistance - (targetDistance - distanceToTarget))+0.25*angle;

	float leftMotorVelocity = leftWheelControl.update(sollLeft - pm.leftWheelVelocity);
	float rightMotorVelocity = rightWheelControl.update(sollRight - pm.rightWheelVelocity);

	leftMotorVelocity = clamp(leftMotorVelocity, -1.5, 1.5);
	rightMotorVelocity = clamp(rightMotorVelocity, -1.5, 1.5);

	leftMotor.setSpeed(leftMotorVelocity*MOTORCONSTANT);
	rightMotor.setSpeed(rightMotorVelocity*MOTORCONSTANT);

	n++; // counting tick up
	printf("%f %d %d %f %f %f %f %f\r\n",n*CONTROLLER_SAMPLING_TIME, pm.getPosition().x, pm.getPosition().y, sollDistance, distanceToTarget, soll.getAngleInRadianAround(0.0), pm.getHeading().getAngleInRadianAround(0.0), angle*0.25);
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
	float distance = pm.getPosition().distanceTo(targetPosition);
	Vector targetVector(pm.xum*0.001, pm.yum*0.001);
	Angle soll = (targetPosition.asVectorFromOrigin() - targetVector).getPolarAngle();
	Angle angleError = (soll - pm.getHeading());

	if(distance < 2.5 && angleError.getAngleInDegreesAround(0.0) < 1.0) {
		return true;
	} else {
		return false;
	}
}

DriverFSM::~DriverFSM() {
	delete currentState;
}

void DriverFSM::newTargetPosition() {
	leftMotor.enable();
	rightMotor.enable();
	targetDistance = targetPosition.distanceTo(pm.getPosition())*0.001;
	currentState->newTargetPosition();
}

void DriverFSM::setTargetPosition(Position targetPosition) {
	this->targetPosition = targetPosition;
}

void DriverFSM::setTargetAngle(Angle targetAngle) {
	this->targetAngle = targetAngle;
}

void DriverFSM::setDriveSpeed(DriveSpeed speed) {
	this->driveSpeed = speed;
}

void DriverFSM::setDriveDirection(DriveDirection direction) {
	this->driveDirection = direction;
}
