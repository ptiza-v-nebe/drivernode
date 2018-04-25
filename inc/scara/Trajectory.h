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

const double L1=82.5;
const double L2=67.5;
const double L3=67.5;


using namespace std;

using Pose = std::vector<double>;
using Q = std::vector<double>;
using QTrajectory = std::vector<std::vector<double> >;
using XTrajectory = std::vector<std::vector<double> >;

enum class TimeFactors {
	FAST, MEDIUM, SLOW
};

class Trajectory {
private:
	std::vector<double> time;
	std::vector<std::vector<double> > data;
	std::vector<std::vector<double> > trj;
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

	std::vector<std::vector<double> > interpolate(
			std::vector<double> timedStartPose,
			std::vector<double> timedEndPose);
	long map(long x, long in_min, long in_max, long out_min, long out_max);
	Q ik1(Pose pose);
	Q ik2(Pose pose);
	QTrajectory buildJointspace();

	std::vector<double> FK(std::vector<double> q);

	void showXTrajectory(XTrajectory xTrj);
	void showQTrajectory(QTrajectory qTrj);
};

#endif /* TRAJECTORY_H_ */
