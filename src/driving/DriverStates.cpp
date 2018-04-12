/*
 * DriverStates.cpp
 *
 *  Created on: Apr 10, 2018
 *      Author: max
 */

#include <driving/DriverStates.h>

void Idle::doAction() {

}

void DrivingForward::entryAction() {
	ctx.resetControl();
}

void DrivingForward::doAction() {
	ctx.updateControl();
}

void DrivingBackward::doAction() {

}
