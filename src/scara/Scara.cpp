/*
 * Scara.cpp
 *
 *  Created on: 22 Apr 2018
 *      Author: ivan-zarevich
 */

#include "scara/Scara.h"

Scara::Scara(ScaraHardware& hw) :
		lift(hw.getLiftMotor(), hw.getLiftEncoder()), servos(hw.getArmServos()), runOnce(
				false),i(0),qTrj(),j(0),currentTime(0),lastTime(0),positionSet(false),currentState(new Park(*this)),
				scaraPump(hw.getPump()),scaraValve(hw.getValve()), storagePumps(hw.getStoragePumps()),
					pLUT{{5,206,37,M_PI/2,M_PI/2},
						{5,206,103,M_PI/2,M_PI/2},
						{5,206,163,M_PI/2,M_PI/2},
						{5,206,226,M_PI/2,M_PI/2},

						{5,150,37,M_PI*3/4,M_PI/2},
						{5,150,99,M_PI*3/4,M_PI/2},
						{5,150,163,M_PI*3/4,M_PI/2},
						{5,150,226,M_PI*3/4,M_PI/2},

						{5,93,37,M_PI*0.95,M_PI/2},
						{5,93,99,M_PI*0.95,M_PI/2},
						{5,93,163,M_PI*0.95,M_PI/2},
						{5,93,226,M_PI*0.95,M_PI/2}}
{
	servos[0].enable();
	servos[1].enable();
	servos[2].enable();
	servos[3].enable();
	lift.initialize();

	scaraPump.enable();
	scaraValve.enable();

	storagePumps[0].enable();
	storagePumps[1].enable();
	storagePumps[2].enable();


}

void Scara::commandReceived(const ScaraActionMessage& sam){

	//reset servos to prevent getting false angles
	servos[0].disableAndStop();
	servos[1].disableAndStop();
	servos[2].disableAndStop();
	servos[3].disableAndStop();
	servos[0].enable();
	servos[1].enable();
	servos[2].enable();
	servos[3].enable();

	printf("[scara.cpp] command received, reset servos\r\n");

	currentState->commandReceived(sam);
}

void Scara::tick() {
	currentState->tick();
	lift.tick();
}

void Scara::park(){
	currentState->park();
}

vector<float> Scara::readMotorAngles(){
	Angle q0;
	Angle q1;
	Angle q2;
	Angle q3;
	vector<float> q;
	for(int i = 0; i< 10; i++){
		q0 = (servos[0].getAngle()-150_deg);
		q1 = (servos[1].getAngle()-150_deg);
		q2 = (servos[2].getAngle()-60_deg);
		q3 = (servos[3].getAngle()-105_deg);
		q.push_back(0);
		q.push_back(q0.getAngleInRadianAround(0));
		q.push_back(q1.getAngleInRadianAround(0));
		q.push_back(q2.getAngleInRadianAround(0));
		q.push_back(q3.getAngleInRadianAround(0));
		if(isValid(q)){
			break;
		}
	}

	return q;
}

void Scara::generateTrajectoryForLiftUp(){
	StorageSpace stg = storageSpace;
	qTrj.clear();
	trj.setActionTime(2);

	float cx = 0;
	float cy = 0;
	float cz = 0;
	float cphi = 0;
	float ctheta = 0;

	//move cube befor storage
	cx = pLUT[static_cast<int>(stg)].x;
	cy = pLUT[static_cast<int>(stg)].y;
	cz = pLUT[static_cast<int>(stg)].z;
	cphi = pLUT[static_cast<int>(stg)].phi;
	ctheta = pLUT[static_cast<int>(stg)].theta;
	trj.startPose({cx,cy,cz,cphi,ctheta});
	trj.addPose(TimeFactors::MEDIUM, { cx,cy,cz+20,cphi,ctheta});
	qTrj = trj.buildJointspace();

	j=0;
	i=0;
}

void Scara::generateTrajectoryForCube(float x, float y, float phi, StorageSpace stg) {
	storageSpace = stg;

	qTrj.clear();
	//set actionTime
	trj.setActionTime(10);

	vector<float> ma = readMotorAngles();
	vector<float> pos = trj.FK({ma[0],ma[1],ma[2],ma[3],lift.getPosition()});

	//move first to given x,y,phi
	//trj.startPose({pos[0],pos[1],pos[2],pos[3],pos[4]});
	trj.startPose({120,120,200,M_PI/4,M_PI/2});
	//trj.addPose(TimeFactors::SLOW, { pos[0],pos[1], pos[2]+50, pos[3],pos[4]});
	trj.addPose(TimeFactors::FAST, { x, y, 120, phi, M_PI/2});
	trj.addPose(TimeFactors::SLOW, { x, y, 37, phi, M_PI/2}); //runter auf klotz saugen
	trj.addPose(TimeFactors::FAST, { x, y, 120, phi, M_PI/2}); // um klotze nicht zu zerst hoch

	float cx = 0;
	float cy = 0;
	float cz = 0;
	float cphi = 0;
	float ctheta = 0;

	if(stg==StorageSpace::INNER_2 || stg==StorageSpace::MIDDLE_2 || stg==StorageSpace::OUTER_2){
		trj.addPose(TimeFactors::FAST, { x, y, 163, phi, M_PI/2}); // um klotze nicht zu zerst hoch
	}

	if(stg==StorageSpace::INNER_3 || stg==StorageSpace::MIDDLE_3 || stg==StorageSpace::OUTER_3){
			trj.addPose(TimeFactors::FAST, { x, y, 226, phi, M_PI/2}); // um klotze nicht zu zerst hoch
		}

	//move cube befor storage
	cx = pLUT[static_cast<int>(stg)].x;
	cy = pLUT[static_cast<int>(stg)].y;
	cz = pLUT[static_cast<int>(stg)].z;
	cphi = pLUT[static_cast<int>(stg)].phi;
	ctheta = pLUT[static_cast<int>(stg)].theta;
	trj.addPose(TimeFactors::MEDIUM, { cx+30,cy,cz,cphi,ctheta});

	//put in
	cx = pLUT[static_cast<int>(stg)].x;
	cy = pLUT[static_cast<int>(stg)].y;
	cz = pLUT[static_cast<int>(stg)].z;
	cphi = pLUT[static_cast<int>(stg)].phi;
	ctheta = pLUT[static_cast<int>(stg)].theta;
	trj.addPose(TimeFactors::SLOW, { cx,cy,cz,cphi,ctheta});

	//build trajectory
	qTrj = trj.buildJointspace();
	//trj.showQTrajectory(qTrj);

	//reset trajectory
	j=0;
	i=0;
}


void Scara::scaraPumpValveControl(bool on){
	if(on){
		scaraPump.setOn();
		scaraValve.setOn();
	} else {
		scaraPump.setOff();
		scaraValve.setOff();
	}
}

void Scara::storagePumpsControl(StorageSpace sam){
	if (sam == StorageSpace::INNER_0 || sam == StorageSpace::INNER_1
			|| sam == StorageSpace::INNER_2 || sam == StorageSpace::INNER_3) {
		storagePumps[0].setOn();
	}

	if (sam == StorageSpace::MIDDLE_0 || sam == StorageSpace::MIDDLE_1
				|| sam == StorageSpace::MIDDLE_2 || sam == StorageSpace::MIDDLE_3) {
			storagePumps[1].setOn();
	}
	if (sam == StorageSpace::OUTER_0 || sam == StorageSpace::OUTER_1
				|| sam == StorageSpace::OUTER_2 || sam == StorageSpace::OUTER_3) {
			storagePumps[2].setOn();
	}
}

void Scara::disableStoragePumps(){
	storagePumps[0].setOff();
	storagePumps[1].setOff();
	storagePumps[2].setOff();
}

Scara::~Scara() {
	delete currentState;
}

void Scara::executeTrajectory() {
	currentTime = i*0.010;
	double goalTime;

	//check if actual Joint angle is not nan

	if((j < qTrj.size())){
		goalTime = qTrj[j][0];
	}

	if ((currentTime > goalTime) && (j < qTrj.size())) {
		trj.showQPoint(qTrj[j]);

		if (!isValid(qTrj[j])) {
			printf("[Scara.cpp] not valid point!\r\n");
			currentState->cancelMovement();
		} else {
			servos[0].moveTo(qTrj[j][1] + 150 * M_PI / 180);
			servos[1].moveTo(qTrj[j][2] + 150 * M_PI / 180);
			servos[2].moveTo(qTrj[j][3] + 60 * M_PI / 180);
			servos[3].moveTo(qTrj[j][4] + 105 * M_PI / 180);
			lift.moveTo(qTrj[j][5]);
			j++;
		}
	}

	i++;

	if(j >= qTrj.size()){
		currentState->trajectoryEnd();
	}
}

void Scara::cancelExecute(){
	//printf("[Scara.cpp] trajectoryEnd!\r\n");
	currentState->trajectoryEnd();
}

bool Scara::isValid(vector<float> qTrjP){
	for(int i = 1; i < qTrjP.size(); i++){
		if(isnan(qTrjP[i])){
			return false;
		}
	}

	bool q1InRange = (qTrjP[1]>-1.57) && (qTrjP[1]<1.55);
	if(!q1InRange)
		return false;

	bool q2InRange = (qTrjP[2]>-2.0) && (qTrjP[2]<2.0);
	if(!q2InRange)
		return false;

	bool q3InRange = (qTrjP[3]>-2.82) && (qTrjP[3]<2.70);
	if(!q3InRange)
		return false;

	bool q4InRange = (qTrjP[4]>-1.84) && (qTrjP[4]<2.05);
	if(!q4InRange)
		return false;

	return true;
}

void Scara::initialize() {
	currentState->entry();
}
