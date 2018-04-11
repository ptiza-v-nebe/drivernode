/*
 * DrivingState.h
 *
 *  Created on: Apr 10, 2018
 *      Author: max
 */

#ifndef DRIVING_DRIVERBASESTATE_H
#define DRIVING_DRIVERBASESTATE_H

#include <driving/DriverFSM.h>

#define STATE(name) \
class name: public DriverBaseState {\
public:\
	name(DriverFSM& context) :\
			DriverBaseState(context) {\
	}
// end of #define

#define CHANGE_STATE(newState) \
        exitAction(); new (this) newState(ctx); entryAction();

class DriverFSM;

class DriverBaseState {
protected:
	DriverFSM& ctx;
public:
	DriverBaseState(DriverFSM& ctx) : ctx(ctx) {}
	virtual ~DriverBaseState() = default;

	virtual void entryAction() {}
	virtual void doAction() {}
	virtual void exitAction() {}
};

#endif /* DRIVING_DRIVERBASESTATE_H */
