/*
 * DriverStates.cpp
 *
 *  Created on: Apr 10, 2018
 *      Author: max
 */

#include <driving/DriverStates.h>

// Idle actions
void Idle::newTargetPosition() {
	CHANGE_STATE(Driving);
}

void Idle::newTargetAngle() {
	CHANGE_STATE(Turning);
}

// Driving actions
void Driving::entryAction() {
	ctx.resetControl();
}

void Driving::doAction() {
	if(ctx.reachedTargetPosition()) {
		CHANGE_STATE(Idle)
	} else {
		ctx.updateControl();
	}
}

void Driving::exitAction() {
	ctx.stop();
}

void Driving::newTargetPosition() {
	CHANGE_STATE(Driving);
}

void Driving::stop() {
	CHANGE_STATE(Stop);
}

void Turning::entryAction() {
	ctx.resetControl();
}

void Turning::doAction() {
	ctx.updateControl();
}

void Turning::exitAction() {
	ctx.stop();
}

// Stop actions

