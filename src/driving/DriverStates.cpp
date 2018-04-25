/*
 * DriverStates.cpp
 *
 *  Created on: Apr 10, 2018
 *      Author: max
 */

#include <driving/DriverStates.h>

// Idle actions
void Idle::entryAction() {
	if(ctx.isAccuracyHigh()) {
		ctx.stopMotors();
	}
}

void Idle::exitAction() {
	ctx.enableMotors();
}

void Idle::newPosition() {
	ctx.calculateDistance();
	CHANGE_STATE(Driving);
}

void Idle::newAngle() {
	ctx.calculateAngle();
	CHANGE_STATE(Driving);
}

// Driving actions
void Driving::doAction() {
	if(ctx.referencePositionReached()) {
		ctx.sendFinishedMessage();
		CHANGE_STATE(Idle);
	} else if(ctx.isRobotStuck()) {
		ctx.sendStuckMessage();
		ctx.stopMotors();
		ctx.resetControl();
		CHANGE_STATE(Idle);
	} else {
		ctx.updateControl();
	}
}

void Driving::newPosition() {
	ctx.calculateDistance();
}

void Driving::newAngle() {
	ctx.calculateAngle();
}

void Driving::stop() {
	ctx.stopMotors();
	ctx.resetControl();
	CHANGE_STATE(Idle);
}
