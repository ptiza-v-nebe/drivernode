/*
 * Scara.cpp
 *
 *  Created on: 22 Apr 2018
 *      Author: ivan-zarevich
 */

#include "scara/Scara.h"

void Scara::commandReceived(const ScaraActionMessage& sam){
	currentState->commandReceived(sam);
}

void Scara::tick() {
	currentState->tick();
	lift.tick();
}

//unloadstoragepumpoffFunc

void Scara::park(){
	currentState->park();
}



void Scara::generateTrajectoryForCube(float x, float y, float phi, StorageSpace stg) {
	qTrj.clear();
	//set actionTime
	trj.setActionTime(8);

	//get actual motor angles
	Angle q0 = servos[0].getAngle()-150_deg;
	Angle q1 = servos[1].getAngle()-150_deg;
	Angle q2 = servos[2].getAngle()-60_deg;
	Angle q3 = servos[3].getAngle()-105_deg;

	//get actual position from FK
	vector<float> pos = trj.FK({q0.getAngleInRadianAround(0),q1.getAngleInRadianAround(0),q2.getAngleInRadianAround(0),q3.getAngleInRadianAround(0),lift.getPosition()});

	//move first to given x,y,phi
	trj.startPose({pos[0],pos[1],pos[2],pos[3],pos[4]});
	trj.addPose(TimeFactors::SLOW, { pos[0],pos[1], pos[2]+50, pos[3],pos[4]});
	trj.addPose(TimeFactors::FAST, { x, y, 120, phi, M_PI/2});
	trj.addPose(TimeFactors::SLOW, { x, y, 47, phi, M_PI/2}); //runter auf klotz saugen
	trj.addPose(TimeFactors::FAST, { x, y, 120, phi, M_PI/2}); // um klotze nicht zu zerst hoch

	//move cube to storage
	trj.addPose(TimeFactors::SLOW, { pLUT[static_cast<int>(stg)].x,pLUT[static_cast<int>(stg)].y,pLUT[static_cast<int>(stg)].z,pLUT[static_cast<int>(stg)].phi,pLUT[static_cast<int>(stg)].theta});

	//build trajectory
	qTrj = trj.buildJointspace();

	//reset trajectory
	j=0;
	i=0;
}


Scara::Scara(ScaraHardware& hw) :
		lift(hw.getLiftMotor(), hw.getLiftEncoder()), servos(hw.getArmServos()), runOnce(
				false),i(0),qTrj(),j(0),currentTime(0),lastTime(0),positionSet(false),currentState(new Park(*this)),
				scaraPump(hw.getPump()),scaraValve(hw.getValve()), storagePumps(hw.getStoragePumps()),
							pLUT{{23,211,43,M_PI/2,M_PI/2},
								{23,211,103,M_PI/2,M_PI/2},
								{23,211,163,M_PI/2,M_PI/2},
								{23,211,226,M_PI/2,M_PI/2},

								{23,150,43,M_PI*3/4,M_PI/2},
								{23,150,99,M_PI*3/4,M_PI/2},
								{23,150,163,M_PI*3/4,M_PI/2},
								{23,150,226,M_PI*3/4,M_PI/2},

								{23,93,43,M_PI*0.95,M_PI/2},
								{23,93,99,M_PI*0.95,M_PI/2},
								{23,93,163,M_PI*0.95,M_PI/2},
								{23,93,226,M_PI*0.95,M_PI/2}}
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

	if((j < qTrj.size())){
		goalTime = qTrj[j][0];
	}

	if((currentTime > goalTime) && (j < qTrj.size()) ){
		servos[0].moveTo(qTrj[j][1]+150*M_PI/180);
		servos[1].moveTo(qTrj[j][2]+150*M_PI/180);
		servos[2].moveTo(qTrj[j][3]+60*M_PI/180);
		servos[3].moveTo(qTrj[j][4]+105*M_PI/180);
		lift.moveTo(qTrj[j][5]);
		j++;
	}

	i++;


	if(j >= qTrj.size()){
		currentState->trajectoryEnd();
	}
}

void Scara::initialize() {
	currentState->entry();
}
