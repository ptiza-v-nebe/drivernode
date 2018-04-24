/*
 * Scara.h
 *
 *  Created on: 22 Apr 2018
 *      Author: ivan-zarevich
 */

#ifndef SCARA_SCARA_H_
#define SCARA_SCARA_H_
#include <vector>
#include "scara/ScaraLift.h"
#include "hal/DynamixelAX12A.h"
#include "hal/ScaraHardware.h"
#include <math.h>
#include "scara/Trajectory.h"

static constexpr double ZBase = 0;
static constexpr double ZLimit = 7100;

//using Pose = std::vector<std::vector<double>>;
//using Q = std::vector<std::vector<double>>;

using Pose = std::vector<double>;
using Q = std::vector<double>;

class Scara {
private:
	ScaraLift lift;
	DynamixelAX12A *servos;
	bool runOnce;
	long map(long x, long in_min, long in_max, long out_min, long out_max);
	QTrajectory qTrj;
	int i;

public:
	Scara(ScaraHardware& hw);
	virtual ~Scara();
	void execute();
	void initialize();
	void cancelExecute(); //unterbrechung jeglicher Aktionen
	void task();
	QTrajectory buildTask();
};

#endif /* SCARA_SCARA_H_ */
