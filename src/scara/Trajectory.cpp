/*
 * Trajectory.cpp
 *
 *  Created on: 23 Apr 2018
 *      Author: ivan-zarevich
 */

#include "scara/Trajectory.h"


Trajectory::Trajectory() :
		k(0), sumTime(0), actionTime(0),trj() {
		//dFactors.push_back(0);
}

Trajectory::~Trajectory() {
}

void Trajectory::setActionTime(float actiontime) {
	this->actionTime = actiontime;
}

void Trajectory::startPose(const vector<float>& pose) {
	dFactors.clear();
	dFactors.push_back(0);
	trj.clear();
	trj.push_back({0.0, pose[0], pose[1], pose[2], pose[3],pose[4]});
}

void Trajectory::addPose(TimeFactors dTimeFactor, Pose pose) {
	float dTimeFactorNum = 0;

	if (dTimeFactor == TimeFactors::FAST)
		dTimeFactorNum = 10;

	if (dTimeFactor == TimeFactors::MEDIUM)
		dTimeFactorNum = 25;

	if (dTimeFactor == TimeFactors::SLOW)
		dTimeFactorNum = 50;

	dFactors.push_back(dTimeFactorNum / 100);

	//std::cout << "----" << std::endl;

	// for(int i=0; i<dFactors.size();i++){
	// 	std::cout << dFactors[i] << std::endl;
	// }

	float sumOfDFactors = 0;
	for(int i=0; i<dFactors.size(); i++){
		sumOfDFactors +=dFactors[i];
	}
	//std::cout << "----" << std::endl;
	//std::cout << "sumOfDFactors" << sumOfDFactors << std::endl;

	//std::cout << "----" << std::endl;
	std::vector<float> newDFactors;
	for(int i =0; i<dFactors.size();i++){
		newDFactors.push_back(dFactors[i]/sumOfDFactors);
	}
	sumOfDFactors = 0;

	trj.push_back({0,0,0,0,0,0});

	// // //clear time vector
//	 for (unsigned int i = 0; i < trj.size(); i++) {
//	  	trj[i][0] = 0.0;
//	}

	// // //calculate times
	unsigned int i = 0;
	float accOfDFactors = 0;

	 for (i = 0; i < newDFactors.size(); i++) {
	 	accOfDFactors = accOfDFactors + newDFactors[i];
	 	trj[i][0] = actionTime * accOfDFactors;
		//std::cout << " accOfDFactors: " << accOfDFactors << " newDFactors[i]: " << newDFactors[i]  << " trj[ " << i << " ][0]: " << trj[i][0] <<endl;
	 }

	// // //copy poses
	trj[i-1][1] = pose[0];
	trj[i-1][2] = pose[1];
	trj[i-1][3] = pose[2];
	trj[i-1][4] = pose[3];
	trj[i-1][5] = pose[4];
}

QTrajectory Trajectory::buildJointspace() {
	XTrajectory totalTrj;
	QTrajectory qTrj;

	for (unsigned int i = 0; i < trj.size()-1; i++) {
		//interpolate on each trj element paar
		XTrajectory interTrj = interpolate(trj[i], trj[i + 1]);
		//showXTrajectory(interTrj);

		//copy row to total trajectory
		std::for_each(interTrj.begin(), interTrj.end(),
				[&](std::vector<float> & timedPose) {
					totalTrj.push_back(timedPose);
				});
	}

	//showXTrajectory(totalTrj);

	//inverse Kinematic
	std::for_each(totalTrj.begin(), totalTrj.end(),
			[&](std::vector<float> & timedPose) {
				qTrj.push_back(ik1(timedPose));
			});

	//copy action control bools


	showQTrajectory(qTrj);

	return qTrj;
}

std::vector<std::vector<float> > Trajectory::interpolate(
		std::vector<float> timedStartPose, std::vector<float> timedEndPose) {
	int n = 10;

	float startTime = timedStartPose[0];
	float endTime = timedEndPose[0];

	float startX = timedStartPose[1];
	float endX = timedEndPose[1];

	float startY = timedStartPose[2];
	float endY = timedEndPose[2];

	float startZ = timedStartPose[3];
	float endZ = timedEndPose[3];

	float startPhi=timedStartPose[4];
	float endPhi=timedEndPose[4];

	float startTheta = timedStartPose[5];
	float endTheta = timedEndPose[5];

	std::vector<float> interTimes = linspace(startTime, endTime, n);
	std::vector<float> interPoseX = linspace(startX, endX, n);
	std::vector<float> interPoseY = linspace(startY, endY, n);
	std::vector<float> interPoseZ = linspace(startZ, endZ, n);
	std::vector<float> interPoseP = linspace(startPhi,endPhi,n);
	std::vector<float> interPoseT = linspace(startTheta, endTheta, n);

	std::vector<std::vector<float> > interTrajectory;

	for (unsigned int i = 0; i < interTimes.size(); i++) {
		interTrajectory.push_back({interTimes[i],interPoseX[i],interPoseY[i],interPoseZ[i], interPoseP[i],interPoseT[i]});
	}

	return interTrajectory;
}


std::vector<float> Trajectory::linspace(float start, float end, int num_in) {

	std::vector<float> linspaced;
	float num = static_cast<float>(num_in);

	if (num_in == 0) {
		return linspaced;
	}
	if (num_in == 1) {
		linspaced.push_back(start);
		return linspaced;
	}

	float delta = (end - start) / (num - 1);

	for (int i = 0; i < num - 1; ++i) {
		linspaced.push_back(start + delta * i);
	}
	linspaced.push_back(end); // I want to ensure that start and end
							  // are exactly the same as the input
	return linspaced;
}




//TODO: correct phi!
Q Trajectory::ik1(std::vector<float> timedPose) {
	float t = timedPose[0];
	float x = timedPose[1];
	float y = timedPose[2];
	float z = timedPose[3];
	float phi = timedPose[4]; //how the last point is rotated
	float theta = timedPose[5]; //how the saug is rotated

	float L1 = 82.5;
	float L2 = 67.5;
	float L3 = 67.5;
	float L4 = 51.0;

	float xp = x - L3 * cos(phi);
	float yp = y - L3 * sin(phi);

	float gamma = atan2((-yp / sqrt(pow(xp, 2) + pow(yp, 2))),
			(-xp / sqrt(pow(xp, 2) + pow(yp, 2))));
	int sign = 1;
	float q1 = gamma
			+ sign
					* acos(
							-(pow(xp, 2) + pow(yp, 2) + pow(L1, 2) - pow(L2, 2))
									/ (2 * L1 * sqrt(pow(xp, 2) + pow(yp, 2))));

	float q2 = (atan2((yp - L1 * sin(q1)) / L2, (xp - L1 * cos(q1)) / L2) - q1);

	float q3 = phi - (q1 + q2);

	float q4 = -(theta - (q1 + q2 + q3));
	float q5 = map(z, 37.0, 231.5, 0, 7000);

	return {t, q1, q2, q3, q4, q5};
}

Q Trajectory::ik2(std::vector<float> timedPose) {
	float t = timedPose[0];
	float x = timedPose[1];
	float y = timedPose[2];
	float z = timedPose[3];
	float theta = timedPose[4]; //how the saug is rotated

	//lengths of links

	float rs2 = L2 + L3;

	float vgAbs = sqrt(pow(x,2) + pow(y,2));

	float q1 = asin(y / sqrt(pow(x,2) + pow(y,2))) - acos((pow(rs2 * 0.8,2) - pow(L1,2)- pow(vgAbs,2)) / (-2 * L1 * vgAbs));

	//calculate q3 from dxdy
	float xvj2 = L1 * cos(q1);
	float yvj2 = L1 * sin(q1);

	float xvd = x - xvj2;
	float yvd = y - yvj2;

	float q3 = acos(
			(pow(xvd,2) + pow(yvd,2) - pow(L2,2) - pow(L3,2)) / (2 * L2 * L3));

	//calculate q2 from dxdy
	float sign = -1;
	if (q3 > 0)
		sign = -1;
	if (q3 < 0)
		sign = 1;

	//float part1 = asin(yvd / sqrt( pow(xvd,2) + pow(yvd,2)));
	float part1 = atan2(yvd,xvd);
	float part2 =
			sign
					* asin(
							sqrt(pow((2 * L2 * L3),2) - pow((xvd * xvd + yvd * yvd - L2 * L2 - L3 * L3),2)) / (2 * L2 * sqrt(pow(xvd,2) + pow(yvd,2))));

	float q2 = part1 + part2 - q1;


	//calculate q4 from q1,q2,q3
	float q4 = theta - (q1 + q2 + q3);
	float q5 = map(z, 37.0, 231.5, 0, 7000);

	return {t, q1, q2, q3, q4, q5};
}

int Trajectory::map(int x, int in_min, int in_max, int out_min, int out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//TODO z!
vector<float> Trajectory::FK(const std::vector<float>& q){
	float x = L1*cos(q[0]) + L2*cos(q[0]+q[1]) + L3*cos(q[0]+q[1]+q[2]);
	float y = L1*sin(q[0]) + L2*sin(q[0]+q[1]) + L3*sin(q[0]+q[1]+q[2]);
	float z = map(q[4], 0, 7000, 37.0, 231.5);
	float phi = q[0]+q[1]+q[2];
	float th = q[0]+q[1]+q[2]+q[3];

	return {x, y, z, phi, th};
}

void Trajectory::showXTrajectory(XTrajectory xTrj){
	printf("---------------------\n");
	for(int i =0; i< xTrj.size();i++){
		printf("[%d] t: %f,x: %f,y: %f,z: %f, ph: %f,th: %f \r\n",i,xTrj[i][0],xTrj[i][1],xTrj[i][2],xTrj[i][3],xTrj[i][4],xTrj[i][5]);
	}
}

void Trajectory::showQTrajectory(QTrajectory qTrj){
	//printf("---------------------\n");
	for(int i =0; i< qTrj.size();i++){
		printf("[%d] t: %f, q1: %f, q2: %f, q3: %f,q4:%f,q5:%f \r\n",i,qTrj[i][0],qTrj[i][1],qTrj[i][2],qTrj[i][3],qTrj[i][4]);
	}
}

float Trajectory::getActionTime() {
	return actionTime;
}
