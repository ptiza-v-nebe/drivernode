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
		ctx.disableMotors();
	}
}

void Idle::exitAction() {
	ctx.resetControl();
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
	} else {
		ctx.updateControl();
	}
}

void Driving::exitAction() {

}

void Driving::newPosition() {
	ctx.calculateDistance();
}

void Driving::newAngle() {
	ctx.calculateAngle();
}

void Driving::stop() {
	ctx.stopMotors();
	CHANGE_STATE(Idle);
}
