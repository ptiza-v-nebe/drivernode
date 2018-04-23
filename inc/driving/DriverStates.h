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
	void entryAction() override;
	void exitAction() override;
	void newPosition() override;
	void newAngle() override;
};

STATE(Driving)
	void entryAction() override;
	void doAction() override;
	void exitAction() override;
	void newPosition() override;
	void newAngle() override;
	void stop() override;
};

STATE(Turning)
	void entryAction() override;
	void doAction() override;
	void exitAction() override;
	void newPosition() override;
	void newAngle() override;
	void stop() override;
};

#endif /* DRIVING_DRIVERSTATES_H */
