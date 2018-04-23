/*
 * DriverStates.cpp
 *
 *  Created on: Apr 10, 2018
 *      Author: max
 */

#include <driving/DriverStates.h>

void Idle::entryAction() {
	ctx.disableMotors();
}

void Idle::exitAction() {
	ctx.resetControl();
	ctx.enableMotors();
}

// Idle actions
void Idle::newPosition() {
	if(ctx.isErrorAngleToBig()) {
		CHANGE_STATE(Turning);
	} else {
		CHANGE_STATE(Driving);
	}
}

void Idle::newAngle() {
	CHANGE_STATE(Turning);
}

// Driving actions
void Driving::entryAction() {
	ctx.resetControl();
	ctx.enableMotors();
}

void Driving::doAction() {
	if(ctx.referencePositionReached()) {
		CHANGE_STATE(Idle)
	} else {
		ctx.updateControl();
	}
}

void Driving::newPosition() {
	if(ctx.isErrorAngleToBig()) {
		CHANGE_STATE(Turning);
	} else {
		CHANGE_STATE(Driving);
	}
}

void Driving::newAngle() {
	CHANGE_STATE(Turning);
}

void Driving::stop() {
	CHANGE_STATE(Idle);
}

// Turning actions
void Turning::entryAction() {
	ctx.resetControl();
}

void Turning::doAction() {
	if(ctx.referenceAngleReached()) {
		CHANGE_STATE()
	} else {
		ctx.updateControl();
	}
}

void Turning::exitAction() {
}

void Turning::newPosition() {
	if(ctx.isErrorAngleToBig()) {
		CHANGE_STATE(Turning);
	} else {
		CHANGE_STATE(Driving);
	}
}

void Turning::stop() {
	CHANGE_STATE(Idle);
}
// Stop actions

