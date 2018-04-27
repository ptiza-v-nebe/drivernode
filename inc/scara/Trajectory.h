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

struct Pose {
	float x;
	float y;
	float z;
	float phi;
	float theta;
};

struct TimedPose {
	float t;
	float x;
	float y;
	float z;
	float phi;
	float theta;

	TimedPose() = default;
	TimedPose(float t, Pose p) : t(t), x(p.x), y(p.y), z(p.z), phi(p.phi), theta(p.theta) {}
};

struct Angles {
	float q1;
	float q2;
	float q3;
	float q4;
	float q5;
};

struct TimedAngles {
	float t;
	float q1;
	float q2;
	float q3;
	float q4;
	float q5;
};

enum class TimeFactors {
	FAST, MEDIUM, SLOW
};

class Trajectory {
private:
	std::vector<TimedPose> trj;
	std::vector<float> dFactors;
	float actionTime;
	float sumTime;
	int k;

public:
	Trajectory();
	virtual ~Trajectory();

	void startPose(const Pose& pose);
	void addPose(TimeFactors dTimeFactor, Pose pose);
	void setActionTime(float actiontime);
	float getActionTime();


	std::vector<TimedPose> interpolate(
			TimedPose timedStartPose,
			TimedPose timedEndPose);

	float map(float x, float in_min, float in_max, float out_min, float out_max);
	TimedAngles ik1(TimedPose pose);
	TimedAngles ik2(TimedPose pose);
	vector<TimedAngles> buildJointspace();

	std::vector<float> linspace(float start, float end, int num_in);

	TimedPose FK(const TimedAngles& q);

	void showXTrajectory(vector<TimedPose> xTrj);
	void showQTrajectory(vector<TimedAngles> qTrj);
	void showQPoint(TimedAngles qTrj);
	void showPosition(TimedPose xTrj);
	void clear();
};

#endif /* TRAJECTORY_H_ */
