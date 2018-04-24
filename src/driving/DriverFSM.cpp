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
#include "serial/messages/StatusMessage.h"

DriverFSM::DriverFSM(Motor& motorLeft, Motor& motorRight, PositionManager& pm, MessageDispatcher& md) :
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
		md(md),
		referencePosition(pm.getPosition()),
		referenceAngle(pm.getHeading()),
		angleError(0.0),
		startAngle(0.0),
		rampAngle(0.0),
		driveSpeed(DriveSpeed::SLOW),
		driveDirection(DriveDirection::FORWARD),
		driveAccuracy(DriveAccuracy::LOW)
		{
	currentState = new Idle(*this);
}

void DriverFSM::tick() {
	currentState->doAction();
}

void DriverFSM::updateControl() {
	// angleError
	Vector targetVector(pm.xum*0.001, pm.yum*0.001);
	referenceAngle = (referencePosition.asVectorFromOrigin() - targetVector).getPolarAngle();
	angleError = referenceAngle - pm.getHeading(); // angleError if angle < 10°
	float angleGain = 0.5;

	if(fabs(angleError.getAngleInDegreesAround(0.0)) < 10.0) {
		if(rampDistance < startDistance)
			rampDistance = rampDistance + 0.5*CONTROLLER_SAMPLING_TIME; // ramp up to reference Position
	} else {
		/*if(rampAngle.getAngleInRadianAround(0.0) < startAngle.getAngleInRadianAround(0.0)) {
			rampAngle = rampAngle+signum(angleError.getAngleInRadianAround(0.0))*0.05*CONTROLLER_SAMPLING_TIME; // ramp up angle
			Angle currentAngle = startAngle - referenceAngle;
			angleError = rampAngle - currentAngle;
		} else if(rampAngle.getAngleInRadianAround(0.0) > startAngle.getAngleInRadianAround(0.0)) {
			rampAngle = rampAngle-0.05*CONTROLLER_SAMPLING_TIME; // ramp up angle
			Angle currentAngle = startAngle - referenceAngle;
			angleError = rampAngle - currentAngle;
		}*/
		angleGain = 0.1;
	}

	float currentDistance = startDistance - referencePosition.distanceTo(pm.getPosition())*0.001;
	distanceError = rampDistance - currentDistance;

	float referenceSpeedLeft = distanceError-angleGain*angleError.getAngleInRadianAround(0.0);
	float referenceSpeedRight = distanceError+angleGain*angleError.getAngleInRadianAround(0.0);

	float leftMotorVelocity = leftWheelControl.update(referenceSpeedLeft - pm.leftWheelVelocity);
	float rightMotorVelocity = rightWheelControl.update(referenceSpeedRight - pm.rightWheelVelocity);

	leftMotorVelocity = clamp(leftMotorVelocity, -1.5, 1.5);
	rightMotorVelocity = clamp(rightMotorVelocity, -1.5, 1.5);

	leftMotor.setSpeed(leftMotorVelocity*MOTORCONSTANT);
	rightMotor.setSpeed(rightMotorVelocity*MOTORCONSTANT);

	n++; // counting tick up
	//printf("%f %f %f %f %f\r\n",n*CONTROLLER_SAMPLING_TIME,referenceAngle.getAngleInRadianAround(0.0),pm.getHeading().getAngleInRadianAround(0.0),rampAngle.getAngleInRadianAround(0.0), angleError.getAngleInRadianAround(0.0));
	//printf("%f %f %f\r\n", n*CONTROLLER_SAMPLING_TIME, distanceError, angleError.getAngleInRadianAround(0.0));
}
void DriverFSM::resetControl() {
	positionControl.reset();
	angleControl.reset();
	leftWheelControl.reset();
	rightWheelControl.reset();
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

void DriverFSM::calculateDistance() {
	rampDistance = 0;
	distanceError = 0;
	angleError = 0;
	startDistance = (referencePosition.distanceTo(pm.getPosition()))*0.001;
	Vector targetVector(pm.xum*0.001, pm.yum*0.001);
	referenceAngle = (referencePosition.asVectorFromOrigin() - targetVector).getPolarAngle();
	startAngle = referenceAngle - pm.getHeading();
}

void DriverFSM::calculateAngle() {
	rampDistance = 0;
	distanceError = 0;
	angleError = 0;
	startDistance = 0;
	startAngle = (referenceAngle - pm.getHeading()).getAngleInRadianAround(0.0);
}

void DriverFSM::sendFinishedMessage() {
	md.sendMessage(StatusMessage(Status::DRIVER_FINISHED));
}

DriverFSM::~DriverFSM() {
	delete currentState;
}

void DriverFSM::newPosition() {
	currentState->newPosition();
}

void DriverFSM::stop() {
	currentState->stop();
}

void DriverFSM::setReferencePosition(Position position) {
	this->referencePosition = position;
}

void DriverFSM::setReferenceAngle(Angle angle) {
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
