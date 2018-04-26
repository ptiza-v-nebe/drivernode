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

const float L1=82.5;
const float L2=67.5;
const float L3=67.5;


using namespace std;

//using Pose = std::vector<float>;
using Q = std::vector<float>;
using QTrajectory = std::vector<std::vector<float> >;
using XTrajectory = std::vector<std::vector<float> >;

enum class TimeFactors {
	FAST, MEDIUM, SLOW
};

class Trajectory {
private:
	std::vector<float> time;
	std::vector<std::vector<float> > data;
	std::vector<std::vector<float> > trj;
	std::vector<float> dFactors;
	float actionTime;
	float sumTime;
	int k;

public:
	Trajectory();
	virtual ~Trajectory();

	void startPose(const vector<float>& pose);
	void addPose(TimeFactors dTimeFactor, vector<float> pose);
	void setActionTime(float actiontime);
	float getActionTime();


	std::vector<std::vector<float> > interpolate(
			std::vector<float> timedStartPose,
			std::vector<float> timedEndPose);
	int map(int x, int in_min, int in_max, int out_min, int out_max);
	Q ik1(vector<float> pose);
	Q ik2(vector<float> pose);
	QTrajectory buildJointspace();
	std::vector<float> linspace(float start, float end, int num_in);

	std::vector<float> FK(const std::vector<float>& q);

	void showXTrajectory(XTrajectory xTrj);
	void showQTrajectory(QTrajectory qTrj);
};

#endif /* TRAJECTORY_H_ */
