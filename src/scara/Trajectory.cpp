/*
 * Trajectory.cpp
 *
 *  Created on: 23 Apr 2018
 *      Author: ivan-zarevich
 */

#include "Trajectory.h"

Trajectory::Trajectory() :
		k(0), sumTime(0), actionTime(0),trj() {
}

Trajectory::~Trajectory() {
}

void Trajectory::startPose(Pose pose) {
	trj.push_back({0.0,pose[0],pose[1],pose[2],pose[3]});
	dFactors.push_back(0);
}

void Trajectory::addPose(TimeFactors dTimeFactor, Pose pose) {
	double dTimeFactorNum = 0;

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

	double sumOfDFactors;
	for(int i=0; i<dFactors.size(); i++){
		sumOfDFactors +=dFactors[i];
	}
	//std::cout << "----" << std::endl;
	//std::cout << "sumOfDFactors" << sumOfDFactors << std::endl;

	//std::cout << "----" << std::endl;
	std::vector<double> newDFactors;
	for(int i =0; i<dFactors.size();i++){
		newDFactors.push_back(dFactors[i]/sumOfDFactors);
	}
	sumOfDFactors = 0;

	trj.push_back({0,0,0,0,0});

	// // //clear time vector
	 for (unsigned int i = 0; i < trj.size()-1; i++) {
	  	trj[i][0] = 0.0;
	}

	// // //calculate times
	unsigned int i = 0;
	double accOfDFactors = 0;

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
}

void Trajectory::setActionTime(double actiontime) {
	this->actionTime = actiontime;
}

double Trajectory::getActionTime() {
	return actionTime;
}

static std::vector<double> linspace(double start, double end, int num_in) {

	std::vector<double> linspaced;
	double num = static_cast<double>(num_in);

	if (num_in == 0) {
		return linspaced;
	}
	if (num_in == 1) {
		linspaced.push_back(start);
		return linspaced;
	}

	double delta = (end - start) / (num - 1);

	for (int i = 0; i < num - 1; ++i) {
		linspaced.push_back(start + delta * i);
	}
	linspaced.push_back(end); // I want to ensure that start and end
							  // are exactly the same as the input
	return linspaced;
}

std::vector<std::vector<double> > Trajectory::interpolate(
		std::vector<double> timedStartPose, std::vector<double> timedEndPose) {
	int n = 100;

	double startTime = timedStartPose[0];
	double endTime = timedEndPose[0];

	double startX = timedStartPose[1];
	double endX = timedEndPose[1];

	double startY = timedStartPose[2];
	double endY = timedEndPose[2];

	double startZ = timedStartPose[3];
	double endZ = timedEndPose[3];

	double startTheta = timedStartPose[4];
	double endTheta = timedEndPose[4];

	std::vector<double> interTimes = linspace(startTime, endTime, n);
	std::vector<double> interPoseX = linspace(startX, endX, n);
	std::vector<double> interPoseY = linspace(startY, endY, n);
	std::vector<double> interPoseZ = linspace(startZ, endZ, n);
	std::vector<double> interPoseT = linspace(startTheta, endTheta, n);

	std::vector<std::vector<double> > interTrajectory;

	for (unsigned int i = 0; i < interTimes.size(); i++) {
		interTrajectory.push_back({interTimes[i],interPoseX[i],interPoseY[i],interPoseZ[i],interPoseT[i]});
	}

	return interTrajectory;
}


//TODO: correct phi!
Q Trajectory::ik1(std::vector<double> timedPose) {
	double t = timedPose[0];
	double x = timedPose[1];
	double y = timedPose[2];
	double z = timedPose[3];
	double phi = timedPose[4]; //how the last point is rotated
	double theta = timedPose[5]; //how the saug is rotated

	double L1 = 82.5;
	double L2 = 67.5;
	double L3 = 67.5;
	double L4 = 51.0;

	double xp = x - L3 * cos(phi);
	double yp = y - L3 * sin(phi);

	double gamma = atan2((-yp / sqrt(pow(xp, 2) + pow(yp, 2))),
			(-xp / sqrt(pow(xp, 2) + pow(yp, 2))));
	int sign = 1;
	double q1 = gamma
			+ sign
					* acos(
							-(pow(xp, 2) + pow(yp, 2) + pow(L1, 2) - pow(L2, 2))
									/ (2 * L1 * sqrt(pow(xp, 2) + pow(yp, 2))));

	double q2 = atan2((yp - L1 * sin(q1)) / L2, (xp - L1 * cos(q1)) / L2) - q1;

	double q3 = phi - (q1 + q2);

	double q4 = -(theta - (q1 + q2 + q3));
	double q5 = map(z, 37.0, 231.5, 0, 7000);

	return {t, q1, q2, q3, q4,q5};
}

Q Trajectory::ik2(std::vector<double> timedPose) {
	double t = timedPose[0];
	double x = timedPose[1];
	double y = timedPose[2];
	double z = timedPose[3];
	double theta = timedPose[4]; //how the saug is rotated

	//lengths of links
	double L1 = 82.5;
	double L2 = 67.5;
	double L3 = 67.5;
	double rs2 = L2 + L3;

	double vgAbs = sqrt(pow(x,2) + pow(y,2));

	double q1 = asin(y / sqrt(pow(x,2) + pow(y,2))) - acos((pow(rs2 * 0.8,2) - pow(L1,2)- pow(vgAbs,2)) / (-2 * L1 * vgAbs));

	//calculate q3 from dxdy
	double xvj2 = L1 * cos(q1);
	double yvj2 = L1 * sin(q1);

	double xvd = x - xvj2;
	double yvd = y - yvj2;

	double q3 = acos(
			(pow(xvd,2) + pow(yvd,2) - pow(L2,2) - pow(L3,2)) / (2 * L2 * L3));

	//calculate q2 from dxdy
	double sign = -1;
	if (q3 > 0)
		sign = -1;
	if (q3 < 0)
		sign = 1;

	//double part1 = asin(yvd / sqrt( pow(xvd,2) + pow(yvd,2)));
	double part1 = atan2(yvd,xvd);
	double part2 =
			sign
					* asin(
							sqrt(pow((2 * L2 * L3),2) - pow((xvd * xvd + yvd * yvd - L2 * L2 - L3 * L3),2)) / (2 * L2 * sqrt(pow(xvd,2) + pow(yvd,2))));

	double q2 = part1 + part2 - q1;


	//calculate q4 from q1,q2,q3
	double q4 = theta - (q1+q2+q3);
	double q5 = map(z, 37.0, 231.5, 0, 7000);

	return {t, q1, q2, q3, q4, q5};
}

long Trajectory::map(long x, long in_min, long in_max, long out_min,
		long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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
				[&](std::vector<double> & timedPose) {
					totalTrj.push_back(timedPose);
				});
	}

	//showXTrajectory(totalTrj);

	//inverse Kinematic
	std::for_each(totalTrj.begin(), totalTrj.end(),
			[&](std::vector<double> & timedPose) {
				qTrj.push_back(ik2(timedPose));
			});

	//showQTrajectory(qTrj);

	return qTrj;
}


void Trajectory::showXTrajectory(XTrajectory xTrj){
	printf("---------------------\n");
	for(int i =0; i< xTrj.size();i++){
		printf("[%d] t: %f,x: %f,y: %f,z: %f,th: %f \n",i,xTrj[i][0],xTrj[i][1],xTrj[i][2],xTrj[i][3],xTrj[i][4]);
	}
}

void Trajectory::showQTrajectory(QTrajectory qTrj){
	//printf("---------------------\n");
	for(int i =0; i< qTrj.size();i++){
		printf("[%d] t: %f,x: %f,y: %f,z: %f,th: %f \n",i,qTrj[i][0],qTrj[i][1],qTrj[i][2],qTrj[i][3],qTrj[i][4]);
	}
}
