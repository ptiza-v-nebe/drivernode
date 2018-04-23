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
	void newTargetPosition() override;
	void newTargetAngle() override;
};

STATE(Driving)
	void entryAction() override;
	void doAction() override;
	void exitAction() override;
	void newTargetPosition() override;
	void stop() override;
};

STATE(Turning)
	void entryAction() override;
	void doAction() override;
	void exitAction() override;
};

STATE(Stop)
	void entryAction() override;
};
#endif /* DRIVING_DRIVERSTATES_H */
