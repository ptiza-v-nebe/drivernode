/*
 * DriverStates.h
 *
 *  Created on: Apr 10, 2018
 *      Author: max
 */

#ifndef DRIVING_DRIVERSTATES_H
#define DRIVING_DRIVERSTATES_H

#include <driving/DriverBaseState.h>

STATE(Idle)
	void doAction() override;
};

STATE(DrivingForward)
	void entryAction() override;
	void doAction() override;
};

STATE(DrivingBackward)
	void doAction() override;
};

#endif /* DRIVING_DRIVERSTATES_H */
