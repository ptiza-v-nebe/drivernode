/*
 * Trajectory.cpp
 *
 *  Created on: 23 Apr 2018
 *      Author: ivan-zarevich
 */

#include "scara/Trajectory.h"


Trajectory::Trajectory() :
		k(0), sumTime(0), actionTime(0),xTrj() {
		//dFactors.push_back(0);
}

Trajectory::~Trajectory() {
}

void Trajectory::setActionTime(float actiontime) {
	this->actionTime = actiontime;
}

void Trajectory::startPose(const Pose& pose) {
	dFactors.clear();
	dFactors.push_back(0);
	xTrj.clear();

	TimedPose tp(0,pose);
	xTrj.push_back(tp);
}

void Trajectory::clear(){
	dFactors.clear();
	dFactors.push_back(0);
	xTrj.clear();
}

//calculate new times, add null pose
int Trajectory::addTime(TimeFactors dTimeFactor) {
	float dTimeFactorNum = 0;

	if (dTimeFactor == TimeFactors::FAST)
		dTimeFactorNum = 5;

	if (dTimeFactor == TimeFactors::MEDIUM)
		dTimeFactorNum = 30;

	if (dTimeFactor == TimeFactors::SLOW)
		dTimeFactorNum = 200;

	dFactors.push_back(dTimeFactorNum / 100);

	//std::cout << "----" << std::endl;

	// for(int i=0; i<dFactors.size();i++){
	// 	std::cout << dFactors[i] << std::endl;
	// }

	float sumOfDFactors = 0;
	for (int i = 0; i < dFactors.size(); i++) {
		sumOfDFactors += dFactors[i];
	}
	//std::cout << "----" << std::endl;
	//std::cout << "sumOfDFactors" << sumOfDFactors << std::endl;

	//std::cout << "----" << std::endl;
	std::vector<float> newDFactors;
	for (int i = 0; i < dFactors.size(); i++) {
		newDFactors.push_back(dFactors[i] / sumOfDFactors);
	}
	sumOfDFactors = 0;

	TimedPose nullPose;
	nullPose.t = 0;
	nullPose.x = 0;
	nullPose.y = 0;
	nullPose.z = 0;
	nullPose.phi = 0;
	nullPose.theta = 0;

	xTrj.push_back(nullPose);

	// // //calculate times
	unsigned int i = 0;
	float accOfDFactors = 0;

	for (i = 0; i < newDFactors.size(); i++) {
		accOfDFactors = accOfDFactors + newDFactors[i];
		xTrj[i].t = actionTime * accOfDFactors;
		//std::cout << " accOfDFactors: " << accOfDFactors << " newDFactors[i]: " << newDFactors[i]  << " trj[ " << i << " ][0]: " << trj[i][0] <<endl;
	}
	return i;
}

void Trajectory::addPose(TimeFactors dTimeFactor, Pose pose) {
	int i = addTime(dTimeFactor);
	//copy poses
	xTrj[i-1].x = pose.x;
	xTrj[i-1].y = pose.y;
	xTrj[i-1].z = pose.z;
	xTrj[i-1].phi = pose.phi;
	xTrj[i-1].theta = pose.theta;
}

void Trajectory::addQ(TimeFactors dTimeFactor, Angles angles){
		int i = addTime(dTimeFactor);
		TimedPose pose = FK({0,angles.q1,angles.q2,angles.q3,angles.q4,angles.q5});
		//copy poses
		xTrj[i-1].x = pose.x;
		xTrj[i-1].y = pose.y;
		xTrj[i-1].z = pose.z;
		xTrj[i-1].phi = pose.phi;
		xTrj[i-1].theta = pose.theta;
}

vector<TimedAngles> Trajectory::buildJointspace() {
	vector<TimedPose> totalTrj;
	vector<TimedAngles> qTrj;

	for (unsigned int i = 0; i < xTrj.size()-1; i++) {
		//interpolate on each trj element paar
		vector<TimedPose> interTrj = interpolate(xTrj[i], xTrj[i + 1]);
		//showXTrajectory(interTrj);

		//copy row to total trajectory
		std::for_each(interTrj.begin(), interTrj.end(),
				[&](TimedPose& timedPose) {
					totalTrj.push_back(timedPose);
				});
	}

	showXTrajectory(totalTrj);
	printf("size of xtrajectory: %d\r\n",totalTrj.size());

	//inverse Kinematic
	std::for_each(totalTrj.begin(), totalTrj.end(),
			[&](TimedPose & timedPose) {
				qTrj.push_back(ik1(timedPose));
			});

	//copy action control bools

	//showQTrajectory(qTrj);
	printf("size of qtrajectory: %d\r\n",qTrj.size());

	return qTrj;
}

std::vector<TimedPose> Trajectory::interpolate(
		TimedPose timedStartPose, TimedPose timedEndPose) {

	int n = 10;

	float startTime = timedStartPose.t;
	float endTime = timedEndPose.t;

	float startX = timedStartPose.x;
	float endX = timedEndPose.x;

	float startY = timedStartPose.y;
	float endY = timedEndPose.y;

	float startZ = timedStartPose.z;
	float endZ = timedEndPose.z;

	float startPhi=timedStartPose.phi;
	float endPhi=timedEndPose.phi;

	float startTheta = timedStartPose.theta;
	float endTheta = timedEndPose.theta;

	std::vector<float> interTimes = linspace(startTime, endTime, n);
	std::vector<float> interPoseX = linspace(startX, endX, n);
	std::vector<float> interPoseY = linspace(startY, endY, n);
	std::vector<float> interPoseZ = linspace(startZ, endZ, n);
	std::vector<float> interPoseP = linspace(startPhi,endPhi,n);
	std::vector<float> interPoseT = linspace(startTheta, endTheta, n);

	std::vector<TimedPose> interTrajectory;

	for (unsigned int i = 0; i < interTimes.size(); i++) {
		TimedPose in;
		in.t = interTimes[i];
		in.x = interPoseX[i];
		in.y = interPoseY[i];
		in.z = interPoseZ[i];
		in.phi = interPoseP[i];
		in.theta = interPoseT[i];
		interTrajectory.push_back(in);
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

TimedAngles Trajectory::ik1(TimedPose& timedPose) {
	float t = timedPose.t;
	float x = timedPose.x;
	float y = timedPose.y;
	float z = timedPose.z;
	float phi = timedPose.phi; //how the last point is rotated
	float theta = timedPose.theta; //how the saug is rotated

	float L1 = 82.5;
	float L2 = 67.5;
	float L3 = 67.5;

	float xp = x - L3 * cos(phi);
	float yp = y - L3 * sin(phi);

	float gamma = atan2((-yp / sqrt(pow(xp, 2) + pow(yp, 2))),
			(-xp / sqrt(pow(xp, 2) + pow(yp, 2))));
	int sign = 1;


	float qOffset = 0;
	if(yp > 0){
		qOffset = 0;
	}
	if(yp<0){
		qOffset = -M_PI*2;
	}

	float q1 = gamma
				+ sign
						* acos(
								-(pow(xp, 2) + pow(yp, 2) + pow(L1, 2) - pow(L2, 2))
										/ (2 * L1 * sqrt(pow(xp, 2) + pow(yp, 2)))) + qOffset;


	float q2 = (atan2((yp - L1 * sin(q1)) / L2, (xp - L1 * cos(q1)) / L2) - q1);

	float q3 = phi - (q1 + q2);

	float q4 = -(theta - (q1 + q2 + q3));
	//float q5 = map(z, 53.0, 253, 0, 7150);
	float q5 = z;

	TimedAngles ta;
	ta.t = t;
	ta.q1 = q1;
	ta.q2 = q2;
	ta.q3 = q3;
	ta.q4 = q4;
	ta.q5 = q5 + verticalZOffset;

	return ta;
}

Angles Trajectory::ikNonTimed(Pose& pose){
	TimedPose tp(0,pose);
	ik1(tp);
}

TimedAngles Trajectory::ik2(TimedPose timedPose) {
	float t = timedPose.t;
	float x = timedPose.x;
	float y = timedPose.y;
	float z = timedPose.z;
	float phi = timedPose.phi; //how the last point is rotated
	float theta = timedPose.theta; //how the saug is rotated

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

	TimedAngles ta;
	ta.t = t;
	ta.q1 = q1;
	ta.q2 = q2;
	ta.q3 = q3;
	ta.q4 = q4;
	ta.q5 = q5;

	return ta;
}

float Trajectory::map(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//TODO z!
TimedPose Trajectory::FK(const TimedAngles& q){
	float q1 = q.q1;
	float q2 = q.q2;
	float q3 = q.q3;
	float q4 = q.q4;
	float q5 = q.q5;

	TimedPose tp;

	tp.x = L1*cos(q1) + L2*cos(q1+q2) + L3*cos(q1+q2+q3);
	tp.y = L1*sin(q1) + L2*sin(q1+q2) + L3*sin(q1+q2+q3);
	tp.z = q.q5;
	tp.phi = q1+q2+q3;
	tp.theta = q1+q2+q3+q4;

	return tp;
}

void Trajectory::showXTrajectory(vector<TimedPose> xTrj){
	printf("---------------------\r\n");
	for(int i =0; i< xTrj.size();i++){
		printf("[%d] t: %f,x: %f,y: %f,z: %f, ph: %f,th: %f \r\n",i,xTrj[i].t,xTrj[i].x,xTrj[i].y,xTrj[i].z,xTrj[i].phi,xTrj[i].theta);
	}
}

void Trajectory::showPosition(TimedPose xTrj){
	printf("t: %f,x: %f,y: %f,z: %f, ph: %f,th: %f \r\n",xTrj.t,xTrj.x,xTrj.y,xTrj.z,xTrj.phi,xTrj.theta);
}

void Trajectory::showQTrajectory(vector<TimedAngles> qTrj){
	//printf("---------------------\n");
	for(int i =0; i< qTrj.size();i++){
		printf("[%d] t: %f, q1: %f, q2: %f, q3: %f,q4:%f,q5:%f \r\n",i,qTrj[i].q1,qTrj[i].q2,qTrj[i].q3,qTrj[i].q4,qTrj[i].q5);
	}
}

void Trajectory::showQPoint(TimedAngles qTrj){
	//printf("---------------------\n");
	printf("t: %f, q1: %f, q2: %f, q3: %f,q4:%f,q5:%f \r\n",qTrj.q1,qTrj.q2,qTrj.q3,qTrj.q4,qTrj.q5);
}

float Trajectory::getActionTime() {
	return actionTime;
}

Vector3 Trajectory::vectorSubstract(Vector3 v1, Vector3 v2){
	return {v1.x-v2.x, v1.y-v2.y, v1.z-v2.z};
}

float Trajectory::vectorAbsolut(Vector3 v){
	return sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
}

Vector3 Trajectory::vectorUnit(Vector3 v){
	return {v.x/vectorAbsolut(v), v.y/vectorAbsolut(v), v.z/vectorAbsolut(v)};
}

TimedPose Trajectory::interpolateStep(TimedPose& trjActual, TimedPose& trjNext, int n, int i){
	//calculate Position
	float startX = trjActual.x;
	float startY = trjActual.y;
	float startZ = trjActual.z;

	Vector3 b = {trjNext.x, trjNext.y, trjNext.z};
	Vector3 a = {trjActual.x, trjActual.y, trjActual.z};
	Vector3 dV = vectorSubstract(b,a);

	//make length of result = length
	float lVector = vectorAbsolut(dV);

	//divide length/n
	float KVector = lVector/n;

	//multiply ki * e = trjn
	Vector3 e = vectorUnit(dV);

	//calculate orientation
	float lPhi = trjNext.phi - trjActual.phi;
	float KPhi = lPhi/n;
	float startPhi = trjActual.phi;

	float lTheta = trjNext.theta - trjActual.theta;
	float KTheta = lTheta/n;
	float startTheta = trjActual.theta;

	//calculate time

	float lTime = trjNext.t - trjActual.t;
	float KTime = lTime/n;
	float startTime = trjActual.t;

	TimedPose tp(i*KTime+startTime,
			{i*KVector*e.x+startX,
			i*KVector*e.y+startY,
			i*KVector*e.z+startZ,
			i*KPhi+startPhi,
			i*KTheta+startTheta});

	return tp;
}




