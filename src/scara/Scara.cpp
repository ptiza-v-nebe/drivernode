/*
 * Scara.cpp
 *
 *  Created on: 22 Apr 2018
 *      Author: ivan-zarevich
 */

#include "scara/Scara.h"

Scara::Scara(ScaraHardware& hw) :
		lift(hw.getLiftMotor(), hw.getLiftEncoder(), hw.getEndStop()), servos(hw.getArmServos()), runOnce(
				false),i(0),qTrj(),j(0),currentTime(0),lastTime(0),positionSet(false),currentState(new Park(*this)),
				scaraPump(hw.getPump()),scaraValve(hw.getValve()), storagePumps(hw.getStoragePumps()),
				currentAnglesPosition({0,0,0,0,0}),
					pLUT{{ 5,206,59,M_PI/2,M_PI/2},
						{ 5,206,119,M_PI/2,M_PI/2},
						{ 5,206,175,M_PI/2,M_PI/2},
						{ 5,206,233,M_PI/2,M_PI/2},

						{ 5,150,59,M_PI*3/4,M_PI/2},
						{ 5,150,119,M_PI*3/4,M_PI/2},
						{ 5,150,175,M_PI*3/4,M_PI/2},
						{ 5,150,233,M_PI*3/4,M_PI/2},

						{ 5,93,59,M_PI*0.95,M_PI/2},
						{ 5,93,119,M_PI*0.95,M_PI/2},
						{ 5,93,175,M_PI*0.95,M_PI/2},
						{ 5,93,233,M_PI*0.95,M_PI/2}}
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

	qTrj.reserve(50);
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

	//printf("[scara.cpp] command received, reset servos\r\n");

	currentState->commandReceived(sam);
}

void Scara::tick() {
	currentState->tick();
	lift.tick();
}

void Scara::startInitializing() {
    currentState->entry();
}

bool Scara::tickInit() {
    return lift.tickInit();
}

void Scara::park(){
	currentState->park();
}

TimedAngles Scara::readMotorAngles() {
	Angle q0;
	Angle q1;
	Angle q2;
	Angle q3;
	TimedAngles q;
	for (int i = 0; i < 10; i++) {
		q0 = (servos[0].getAngle() - 150_deg);
		q1 = (servos[1].getAngle() - 150_deg);
		q2 = (servos[2].getAngle() - 60_deg);
		q3 = (servos[3].getAngle() - 105_deg);

		q.t = 0;
		q.q1 = q0.getAngleInRadianAround(0);
		q.q2 = q0.getAngleInRadianAround(0);
		q.q3 = q0.getAngleInRadianAround(0);
		q.q4 = q0.getAngleInRadianAround(0);
		q.q5 = 0;

		if (isValid(q)) {
			break;
		}
	}

	return q;
}

void Scara::generateParkTrajectory(){
	qTrj.clear();
	trj.clear();

	trj.setActionTime(4);
	trj.startPose({82,135,100,M_PI/2,M_PI/2});
	trj.addPose(TimeFactors::MEDIUM, { 82,135,253,M_PI/2});
	trj.addPose(TimeFactors::MEDIUM, { 5,207,253,M_PI/2});
	qTrj = trj.buildJointspace();

	currentAnglesPosition = {qTrj[qTrj.size()-1].q1,qTrj[qTrj.size()-1].q2,
			qTrj[qTrj.size()-1].q3,qTrj[qTrj.size()-1].q4,qTrj[qTrj.size()-1].q5};

	j=0;
	i=0;
}

void Scara::generatePreventAttachTrajectory() {
	qTrj.clear();
	trj.setActionTime(2);

	Angles cap = {currentAnglesPosition.q1,currentAnglesPosition.q2,currentAnglesPosition.q3,currentAnglesPosition.q4,currentAnglesPosition.q5};
	TimedPose p = trj.FK({0,cap.q1,cap.q2,cap.q3,cap.q4,cap.q5});
	trj.startPose({p.x,p.y,p.z,p.phi,p.theta});

	trj.addPose(TimeFactors::MEDIUM, { p.x,p.y,p.z+20,p.phi,p.theta });
	qTrj = trj.buildJointspace();

	currentAnglesPosition = {qTrj[qTrj.size()-1].q1,qTrj[qTrj.size()-1].q2,
				qTrj[qTrj.size()-1].q3,qTrj[qTrj.size()-1].q4,qTrj[qTrj.size()-1].q5};

	trj.showQTrajectory(qTrj);
	j = 0;
	i = 0;
}

void Scara::generatePickCubeTrajectory(float x, float y, float phi, StorageSpace stg) {
	storageSpace = stg;

	qTrj.clear();
	//set actionTime
	trj.setActionTime(15);

	//TimedAngles ma = readMotorAngles();
	//TimedPose pos = trj.FK({ma.q1,ma.q2,ma.q3,ma.q4,lift.getPosition()});
	//trj.startPose({pos[0],pos[1],pos[2],pos[3],pos[4]});

	Angles cap = {currentAnglesPosition.q1,currentAnglesPosition.q2,currentAnglesPosition.q3,currentAnglesPosition.q4,currentAnglesPosition.q5};
	TimedPose p = trj.FK({0,cap.q1,cap.q2,cap.q3,cap.q4,cap.q5});
	trj.startPose({p.x,p.y,p.z,p.phi,p.theta});

	//move first to given x,y,phi
	trj.addPose(TimeFactors::FAST, { x, y, 100, phi, M_PI/2});
	trj.addPose(TimeFactors::MEDIUM, { x, y, 48, phi, M_PI/2}); //runter auf klotz saugen
	trj.addPose(TimeFactors::FAST, { x, y, 100, phi, M_PI/2}); // um klotze nicht zu zerst hoch


	float cx = 0;
	float cy = 0;
	float cz = 0;
	float cphi = 0;
	float ctheta = 0;

	if(stg==StorageSpace::INNER_1 || stg==StorageSpace::MIDDLE_1 || stg==StorageSpace::OUTER_1){
			trj.addPose(TimeFactors::FAST, { x, y, 125, phi, M_PI/2}); // um klotze nicht zu zerst hoch
		}

	if(stg==StorageSpace::INNER_2 || stg==StorageSpace::MIDDLE_2 || stg==StorageSpace::OUTER_2){
		trj.addPose(TimeFactors::FAST, { x, y, 184, phi, M_PI/2}); // um klotze nicht zu zerst hoch
	}

	if(stg==StorageSpace::INNER_3 || stg==StorageSpace::MIDDLE_3 || stg==StorageSpace::OUTER_3){
			trj.addPose(TimeFactors::FAST, { x, y, 243, phi, M_PI/2}); // um klotze nicht zu zerst hoch
		}

//	//move cube befor storage
	cx = pLUT[static_cast<int>(stg)].x;
	cy = pLUT[static_cast<int>(stg)].y;
	cz = pLUT[static_cast<int>(stg)].z;
	cphi = pLUT[static_cast<int>(stg)].phi;
	ctheta = pLUT[static_cast<int>(stg)].theta;
	trj.addPose(TimeFactors::MEDIUM, { cx+30,cy,cz,cphi,ctheta});
//
//	//put in
	cx = pLUT[static_cast<int>(stg)].x;
	cy = pLUT[static_cast<int>(stg)].y;
	cz = pLUT[static_cast<int>(stg)].z;
	cphi = pLUT[static_cast<int>(stg)].phi;
	ctheta = pLUT[static_cast<int>(stg)].theta;
	trj.addPose(TimeFactors::MEDIUM, { cx,cy,cz,cphi,ctheta});

	//build trajectory
	qTrj = trj.buildJointspace();
	currentAnglesPosition = {qTrj[qTrj.size()-1].q1,qTrj[qTrj.size()-1].q2,
				qTrj[qTrj.size()-1].q3,qTrj[qTrj.size()-1].q4,qTrj[qTrj.size()-1].q5};

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
		goalTime = qTrj[j].t;
	}

	if ((currentTime > goalTime) && (j < qTrj.size())) {
		//trj.showQPoint(qTrj[j]);

		if (!isValid(qTrj[j])) {
			printf("[Scara.cpp] Got non-valid point! t:%f,x:%f,y:%f,z:%f,phi:%f,theta:%f \r\n",
					qTrj[j].t,qTrj[j].q1,qTrj[j].q2,qTrj[j].q3,qTrj[j].q4,qTrj[j].q5);
			currentState->cancelExecute();
		} else {
			//calculate
			servos[0].moveTo(qTrj[j].q1 + 150 * M_PI / 180);
			servos[1].moveTo(qTrj[j].q2 + 150 * M_PI / 180);
			servos[2].moveTo(qTrj[j].q3 + 60 * M_PI / 180);
			servos[3].moveTo(qTrj[j].q4 + 105 * M_PI / 180);
			lift.moveTo(qTrj[j].q5);
			printf("q5: %f \r\n",qTrj[j].q5);
			currentAnglesPosition = {qTrj[j].q1,qTrj[j].q2,qTrj[j].q3,qTrj[j].q4,qTrj[j].q5};
			j++;
		}
	}

	i++;

	//at trajectory end then call trajectoryend
	if(j >= qTrj.size()){
		currentState->trajectoryEnd();
	}
}

void Scara::cancelExecute(){
	currentState->cancelExecute();
}

bool Scara::isValid(TimedAngles qTrjP){
	if(isnan(qTrjP.q1) || isnan(qTrjP.q2)|| isnan(qTrjP.q3)||isnan(qTrjP.q4)||isnan(qTrjP.q5)){
		return false;
	}

	bool q1InRange = (qTrjP.q1>-1.57) && (qTrjP.q1<1.55);
	if(!q1InRange)
		return false;

	bool q2InRange = (qTrjP.q2>-2.0) && (qTrjP.q2<2.0);
	if(!q2InRange)
		return false;

	bool q3InRange = (qTrjP.q3>-2.82) && (qTrjP.q3<2.70);
	if(!q3InRange)
		return false;

	bool q4InRange = (qTrjP.q4>-1.84) && (qTrjP.q4<2.05);

	if(!q4InRange)
		return false;

	return true;
}

void Scara::clearTrajectory(){
	qTrj.clear();
	trj.clear();
}
