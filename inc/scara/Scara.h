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

//using Pose = std::vector<float>;
//using Q = std::vector<float>;

class Scara {
private:
	ScaraLift lift;
	DynamixelAX12A *servos;
	bool runOnce;
	QTrajectory qTrj;
	Trajectory trj;
	int i;
	double lastTime;
	double currentTime;
	int j;

public:
	Scara(ScaraHardware& hw);
	virtual ~Scara();
	void execute();
	void initialize();
	void cancelExecute(); //unterbrechung jeglicher Aktionen
};

#endif /* SCARA_SCARA_H_ */
