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
		driveAccuracy(DriveAccuracy::LOW),
		speed(0.2),
		direction(1),
		turningAngle(false)
		{
	currentState = new Idle(*this);
}

void DriverFSM::tick() {
	currentState->doAction();
}

void DriverFSM::updateControl() {
	// recalculate referenceAngl
	if(!turningAngle) {
		Vector targetVector(pm.xum*0.001, pm.yum*0.001);
		referenceAngle = (referencePosition.asVectorFromOrigin() - targetVector).getPolarAngle();
	}

	if(direction < 0) { // if driving backward
		referenceAngle = referenceAngle-180_deg;
	}

	angleError = referenceAngle - pm.getHeading();

	printf("%f %f %f\r\n", angleError.getAngleInRadianAround(0.0), referenceAngle.getAngleInRadianAround(0.0), pm.getHeading().getAngleInRadianAround(0.0));

	if(fabs(angleError.getAngleInDegreesAround(0.0)) < 10.0) {
		if(fabs(rampDistance) <= startDistance)
			rampDistance = rampDistance + direction*speed*CONTROLLER_SAMPLING_TIME; // ramp up to reference Position
	}

	if(turningAngle) {
		distanceError = 0;
	} else {
		float currentDistance = (startDistance - referencePosition.distanceTo(pm.getPosition())*0.001)*direction;
		distanceError = rampDistance - currentDistance;
	}

	float referenceSpeedLeft  = distanceError-(TRACK_WIDTH_UM*0.000002)*0.25*angleError.getAngleInRadianAround(0.0);
	float referenceSpeedRight = distanceError+(TRACK_WIDTH_UM*0.000002)*0.25*angleError.getAngleInRadianAround(0.0);

	float leftMotorVelocity = leftWheelControl.update(referenceSpeedLeft - pm.leftWheelVelocity);
	float rightMotorVelocity = rightWheelControl.update(referenceSpeedRight - pm.rightWheelVelocity);

	leftMotorVelocity = clamp(leftMotorVelocity, -1.5, 1.5);
	rightMotorVelocity = clamp(rightMotorVelocity, -1.5, 1.5);

	leftMotor.setSpeed(leftMotorVelocity*MOTORCONSTANT);
	rightMotor.setSpeed(rightMotorVelocity*MOTORCONSTANT);

	n++; // counting tick up
	//printf("%f %f %f %f %f\r\n",n*CONTROLLER_SAMPLING_TIME,angleError.getAngleInRadianAround(0.0), rampDistance, currentDistance, pm.getHeading().getAngleInRadianAround(0.0));
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

void DriverFSM::stopMotors() {
	leftMotor.stop();
	rightMotor.stop();
}

bool DriverFSM::referencePositionReached() {
	float distance = pm.getPosition().distanceTo(referencePosition);

	if(turningAngle) {
		if(fabs(angleError.getAngleInDegreesAround(0.0)) < 1.0) {
			return true;
		} else {
			return false;
		}
	} else {
		if((distance < 2.5 && fabs(angleError.getAngleInDegreesAround(0.0)) < 1.0 ) ||
				(1 == pointOnWhichSideOfLine(referencePosition.asVectorFromOrigin()-Vector(startAngle,3.5),Vector(startAngle+90_deg,1), pm.getPosition()))) {
			return true;
		} else {
			return false;
		}
	}

}

void DriverFSM::calculateDistance() {
	turningAngle = false;
	rampDistance = 0;
	distanceError = 0;
	angleError = 0;
	startDistance = (referencePosition.distanceTo(pm.getPosition()))*0.001;
	Vector targetVector(pm.xum*0.001, pm.yum*0.001);
	referenceAngle = (referencePosition.asVectorFromOrigin() - targetVector).getPolarAngle();

	if(direction < 0) { // if driving backward
		referenceAngle = referenceAngle-180_deg;
	}

	startAngle = referenceAngle - pm.getHeading();
}

void DriverFSM::calculateAngle() {
	turningAngle = true;
	// calculate angleError
	angleError = referenceAngle - pm.getHeading();

	// set distance control to zero
	distanceError = 0;
	rampDistance = 0;
	startDistance = 0;

	/*turningAngle = true;
	angleError = referenceAngle - pm.getHeading();
	printf("%f %f\n", angleError.getAngleInRadianAround(0.0),referenceAngle.getAngleInRadianAround(0.0));
	referencePosition = pm.getPosition()+Vector(pm.getHeading()+referenceAngle,1.5);

	rampDistance = 0;
	distanceError = 0;
	startDistance = 0;
	startAngle = (referenceAngle - pm.getHeading()).getAngleInRadianAround(0.0);*/
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

void DriverFSM::newAngle() {
	currentState->newAngle();
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
	if(speed == DriveSpeed::FAST) {
		this->speed = 0.5; // meters per second
	} else {
		this->speed = 0.2; // meters per second
	}
	this->driveSpeed = speed;
}

void DriverFSM::setDriveDirection(DriveDirection direction) {
	if(direction == DriveDirection::BACKWARD) {
		this->direction = -1;
	} else {
		this->direction = 1;
	}
	this->driveDirection = direction;
}

void DriverFSM::setDriveAccuracy(DriveAccuracy accuracy) {
	this->driveAccuracy = accuracy;
}

int DriverFSM::pointOnWhichSideOfLine(Vector ro, Vector rd, Position p) {
	return signum(rd.y*(p.x-ro.x)-rd.x*(p.y-ro.y));
}
