/*
 * DriverFSM.cpp
 *
 *  Created on: Apr 10, 2018
 *      Author: max
 */

#include <driving/DriverFSM.h>
#include <driving/DriverBaseState.h>
#include <driving/DriverStates.h>
#include <constants.h>
#include <hal/Motor.h>

DriverFSM::DriverFSM(Motor& motorLeft, Motor& motorRight) :
		currentState(nullptr),
		motorLeft(motorLeft),
		motorRight(motorRight),
		positionControl(CONTROLLER_SYSTEM_POSITION_GAIN,
				CONTROLLER_SYSTEM_POSITION_DELAYTIME,
				CONTROLLER_SAMPLING_TIME),
		angleControl(
				CONTROLLER_SYSTEM_ANGLE_GAIN,
				CONTROLLER_SYSTEM_ANGLE_GAIN,
				CONTROLLER_SAMPLING_TIME)
		 {
	currentState = new Idle(*this);
}

DriverFSM::~DriverFSM() {
	delete currentState;
}
