/*
 * Trajectory.h
 *
 *  Created on: 23 Apr 2018
 *      Author: ivan-zarevich
 */

#ifndef TRAJECTORY_H_
#define TRAJECTORY_H_

#include <vector>
#include <numeric>
#include <algorithm>
#include <math.h>
#include <cstdio>

using Pose = std::vector<double>;
using Q = std::vector<double>;
using QTrajectory = std::vector<std::vector<double>>;
using XTrajectory = std::vector<std::vector<double>>;

enum class TimeFactors {
	FAST, MEDIUM, SLOW
};

class Trajectory {
private:
	std::vector<double> time;
	std::vector<std::vector<double>> data;
	std::vector<std::vector<double>> trj;
	std::vector<double> dFactors;
	double actionTime;
	double sumTime;
	int k;

public:
	Trajectory();
	virtual ~Trajectory();

	void startPose(Pose pose);
	void addPose(TimeFactors dTimeFactor, Pose pose);
	void setActionTime(double actiontime);
	double getActionTime();

	std::vector<std::vector<double>> interpolate(
			std::vector<double> timedStartPose,
			std::vector<double> timedEndPose);
	long map(long x, long in_min, long in_max, long out_min, long out_max);
	Q ik1(Pose pose);
	Q ik2(Pose pose);
	XTrajectory buildJointspace();

	std::vector<double> getTimeValues();
	std::vector<double> getXValues();
	std::vector<double> getYValues();
	std::vector<double> getZValues();
	std::vector<double> getThetaValues();
	std::vector<double> getPumpValues();
};

#endif /* TRAJECTORY_H_ */
