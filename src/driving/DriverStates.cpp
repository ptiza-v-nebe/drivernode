/*
 * DriverStates.cpp
 *
 *  Created on: Apr 10, 2018
 *      Author: max
 */

#include <driving/DriverStates.h>

void Idle::newTargetPosition() {
	CHANGE_STATE(DrivingForward);
}

void DrivingForward::entryAction() {
	ctx.resetControl();
}

void DrivingForward::doAction() {
	if(ctx.reachedTargetPosition()) {
		CHANGE_STATE(Idle)
	} else {
		ctx.updateControl();
	}
}

void DrivingForward::exitAction() {
	ctx.stop();
}

void DrivingBackward::entryAction() {
	ctx.resetControl();
}

void DrivingBackward::doAction() {
	ctx.updateControl();
}

void DrivingBackward::exitAction() {
	ctx.stop();
}
