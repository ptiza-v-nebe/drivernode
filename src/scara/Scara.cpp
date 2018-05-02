/*
 * Scara.cpp
 *
 *  Created on: 22 Apr 2018
 *      Author: ivan-zarevich
 */

#include "scara/Scara.h"

Scara::Scara(ScaraHardware& hw, ScaraLift& lift) :
		lift(lift), servos(hw.getArmServos()), runOnce(false), timeStep(0),currentTime(0), currentState(
				new Park(*this)), scaraPump(hw.getPump()), scaraValve(
				hw.getValve()), storagePumps(hw.getStoragePumps()), itaActual(
				{ 0, 0, 0, 0, 0 }),itaLast({ 0, 0, 0, 0, 0 }), actualInterpolationStep(0), actualTrajectoryStep(0) {

	//pLUT[i] = {X,Y,Z,PHI,THETA};
	//positions of INNER Storage places
	pLUT[0] = {5,206,59,M_PI/2,M_PI/2};
	pLUT[1] = {5,206,119,M_PI/2,M_PI/2};
	pLUT[2] = {5,206,175,M_PI/2,M_PI/2};
	pLUT[3] = {5,206,233,M_PI/2,M_PI/2};

	//positions of MIDDLE Storage places
	pLUT[4] = {5,150,59,M_PI*3/4,M_PI/2};
	pLUT[5] = {5,150,119,M_PI*3/4,M_PI/2};
	pLUT[6] = {5,150,175,M_PI*3/4,M_PI/2};
	pLUT[7] = {5,150,233,M_PI*3/4,M_PI/2};

	//positions of OUTER Storage places
	pLUT[8] = {5,93,59,M_PI*0.95,M_PI/2};
	pLUT[9] = {5,93,119,M_PI*0.95,M_PI/2};
	pLUT[10] = {5,93,175,M_PI*0.95,M_PI/2};
	pLUT[11] = {5,93,233,M_PI*0.95,M_PI/2};

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

void Scara::finalPark() {
	servos[0].moveTo((90 + 150) * M_PI / 180);
	servos[1].moveTo(0 + 150 * M_PI / 180);
	servos[2].moveTo(0 + 60 * M_PI / 180);
	servos[3].moveTo(0 + 105 * M_PI / 180);
}

void Scara::startPark() {
	servos[0].moveTo((0 + 150) * M_PI / 180);
	servos[1].moveTo((90 + 150) * M_PI / 180);
	servos[2].moveTo((0 + 60) * M_PI / 180);
	servos[3].moveTo((0 + 105) * M_PI / 180);
}

void Scara::commandReceived(const ScaraActionMessage& sam) {
	//reset servos to prevent getting false angles
//	servos[0].disableAndStop();
//	servos[1].disableAndStop();
//	servos[2].disableAndStop();
//	servos[3].disableAndStop();
//
//	servos[0].enable();
//	servos[1].enable();
//	servos[2].enable();
//	servos[3].enable();

	printf("[scara.cpp] command received\r\n");

	currentState->commandReceived(sam);
}

void Scara::tick() {
	currentState->tick();
}

void Scara::startInitializing() {
	currentState->entry();
}

bool Scara::tickInit() {
	static bool liftInitialized = false;

	if (!liftInitialized) {
		liftInitialized = lift.tickInit();
		return false;
	} else {
		return currentState->tickInit();
	}
}

void Scara::park() {
	currentState->park();
}



void Scara::generateParkTrajectory() {
	trj.setActionTime(2.5);
	trj.startPose( { 82, 135, 100, M_PI / 2, M_PI / 2 });
	trj.addPose(TimeFactors::MEDIUM, { 82, 135, 250, M_PI / 2 });
	trj.addPose(TimeFactors::MEDIUM, { 0, 210, 250, M_PI / 2 });
	timeStep = 0;
}

void Scara::generatePreventAttachTrajectory() {
	trj.setActionTime(2);

	TimedPose p = trj.FK( { 0, itaLast.q1, itaLast.q2, itaLast.q3, itaLast.q4, itaLast.q5 });
	trj.startPose( { p.x, p.y, p.z, p.phi, p.theta });

	trj.addPose(TimeFactors::MEDIUM, { p.x, p.y, p.z + 20, p.phi, p.theta });
	timeStep = 0;
}

void Scara::generatePickCubeTrajectory(float x, float y, float phi,
		StorageSpace stg) {
	printf("generate pick cube trajectory \r\n");
	//set actionTime
	trj.setActionTime(10);

	TimedPose p = trj.FK( { 0, itaLast.q1, itaLast.q2, itaLast.q3, itaLast.q4, itaLast.q5 });
	trj.startPose( { p.x, p.y, p.z, p.phi, p.theta });

	//move first to given x,y,phi
	trj.addPose(TimeFactors::FAST, { x, y, 140, phi, M_PI / 2 });
	trj.addPose(TimeFactors::SLOW, { x, y, 46, phi, M_PI / 2 }); //runter auf klotz saugen
	trj.addPose(TimeFactors::SLOW, { x, y, 140, phi, M_PI / 2 }); // um klotze nicht zu zerst hoch

	float cx = 0;
	float cy = 0;
	float cz = 0;
	float cphi = 0;
	float ctheta = 0;

	if (stg == StorageSpace::INNER_1 || stg == StorageSpace::MIDDLE_1
			|| stg == StorageSpace::OUTER_1) {
		trj.addPose(TimeFactors::FAST, { x, y, 133, phi, M_PI / 2 }); // um klotze nicht zu zerst hoch
	}

	if (stg == StorageSpace::INNER_2 || stg == StorageSpace::MIDDLE_2
			|| stg == StorageSpace::OUTER_2) {
		trj.addPose(TimeFactors::SLOW, { x, y, 196, phi, M_PI / 2 }); // um klotze nicht zu zerst hoch
	}

	if (stg == StorageSpace::INNER_3 || stg == StorageSpace::MIDDLE_3
			|| stg == StorageSpace::OUTER_3) {
		trj.addPose(TimeFactors::SLOW, { x, y, 253, phi, M_PI / 2 }); // um klotze nicht zu zerst hoch
	}

//	//move cube befor storage
	cx = pLUT[static_cast<int>(stg)].x;
	cy = pLUT[static_cast<int>(stg)].y;
	cz = pLUT[static_cast<int>(stg)].z;
	cphi = pLUT[static_cast<int>(stg)].phi;
	ctheta = pLUT[static_cast<int>(stg)].theta;
	trj.addPose(TimeFactors::FAST, { cx + 30, cy, cz, cphi, ctheta });

//	//put in
	cx = pLUT[static_cast<int>(stg)].x;
	cy = pLUT[static_cast<int>(stg)].y;
	cz = pLUT[static_cast<int>(stg)].z;
	cphi = pLUT[static_cast<int>(stg)].phi;
	ctheta = pLUT[static_cast<int>(stg)].theta;
	trj.addPose(TimeFactors::SLOW, { cx, cy, cz, cphi, ctheta });

	trj.showXTrajectory(trj.xTrj);

	timeStep = 0;
}

void Scara::scaraPumpValveControl(bool on) {
	if (on) {
		scaraPump.setOn();
		scaraValve.setOn();
	} else {
		scaraPump.setOff();
		scaraValve.setOff();
	}
}

void Scara::storagePumpsControl(StorageSpace sam) {
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

void Scara::disableStoragePumps() {
	storagePumps[0].setOff();
	storagePumps[1].setOff();
	storagePumps[2].setOff();
}

Scara::~Scara() {
	delete currentState;
}



TimedAngles Scara::calculateQ(int trjElement, int interElement,
		int numOfInterPoints) {
	TimedPose interPose = trj.interpolateStep(trj.xTrj[trjElement],
			trj.xTrj[trjElement + 1], numOfInterPoints, interElement);
	return trj.ik1(interPose);
}

TimedAngles Scara::readMotorAngles() {
//	servos[0].disableAndStop();
//	servos[1].disableAndStop();
//	servos[2].disableAndStop();
//	servos[3].disableAndStop();

//	servos[0].enable();
//	servos[1].enable();
//	servos[2].enable();
//	servos[3].enable();


	Angle q0 = (servos[0].getAngle() - 150_deg);
	Angle q1 = (servos[1].getAngle() - 150_deg);
	Angle q2 = (servos[2].getAngle() - 60_deg);
	Angle q3 = (servos[3].getAngle() - 105_deg);

	TimedAngles q;
	q.t = 0;
	q.q1 = q0.getAngleInRadianAround(0);
	q.q2 = q1.getAngleInRadianAround(0);
	q.q3 = q2.getAngleInRadianAround(0);
	q.q4 = q3.getAngleInRadianAround(0);
	q.q5 = 0;

	return q;
}

//void Scara::executeTrajectory() {
//	monitoring();
//}

void Scara::monitoring() {
	TimedAngles itaActual = readMotorAngles();

	printf("------------------------- \r\n");
	printf("[RE]: q1:%f,q2:%f,q3:%f,q4:%f \r\n",itaActual.q1, itaActual.q2, itaActual.q3, itaActual.q4);

	TimedPose p = trj.FK(itaActual);

	printf("[FK]: x:%f,y:%f, phi:%f \r\n",p.x,p.y,p.phi);

	TimedAngles ikq = trj.ik1(p);

	printf("[IK]: q1:%f,q2:%f,q3:%f,q4:%f \r\n",ikq.q1, ikq.q2, ikq.q3, ikq.q4);

	servos[0].moveTo(ikq.q1 + 150 * M_PI / 180);
	servos[0].setRPM(114);

	servos[1].moveTo(ikq.q2 + 150 * M_PI / 180);
	servos[1].setRPM(114);

	servos[2].moveTo(ikq.q3 + 60 * M_PI / 180);
	servos[2].setRPM(114);

	//servos[3].moveTo(ikq.q4 + 105 * M_PI / 180);
	//servos[3].setRPM(114);


	for(int i = 0; i<100000;i++){

	}
	lift.moveTo(80);
}

void Scara::executeTrajectory() {
	currentTime = timeStep * 0.010;

	int numOfInterSteps = 40; //40

	if ((trj.xTrj.size() > 0)) {
		itaActual = calculateQ(actualTrajectoryStep, actualInterpolationStep,
				numOfInterSteps);
	}

	//send command of position and speed to motor
	if ((currentTime > itaActual.t) && (trj.xTrj.size() > 0)) {
		if (!isValidSequence(itaActual)) {
			printf("[Non Valid]: t:%f,q1:%f,q2:%f,q3:%f,q4:%f,q5:%f \r\n",itaActual.t,itaActual.q1,itaActual.q2,itaActual.q3,itaActual.q4,itaActual.q5);
			currentState->cancelExecute();
		} else {
			//printf("[Valid]: t:%f,q1:%f,q2:%f,q3:%f,q4:%f,q5:%f \r\n",itaActual.t,itaActual.q1,itaActual.q2,itaActual.q3,itaActual.q4,itaActual.q5);

			float tpm = (itaActual.t-itaLast.t)*(2*M_PI)/60;

			servos[0].moveTo(itaActual.q1 + 150 * M_PI / 180);
			//servos[0].setRPM((itaActual.q1-itaLast.q1)/tpm);
			servos[0].setRPM(114);

			servos[1].moveTo(itaActual.q2 + 150 * M_PI / 180);
			//servos[1].setRPM((itaActual.q2-itaLast.q2)/tpm);
			servos[1].setRPM(114);

			servos[2].moveTo(itaActual.q3 + 60 * M_PI / 180);
			//servos[2].setRPM((itaActual.q3-itaLast.q3)/tpm);
			servos[2].setRPM(114);

			servos[3].moveTo(itaActual.q4 + 105 * M_PI / 180);
			//servos[3].setRPM((itaActual.q4-itaLast.q4)/tpm);
			servos[3].setRPM(114);

			lift.moveTo(itaActual.q5);

			actualInterpolationStep++;
			itaLast = itaActual;
		}
	}

	//increment trajectory step
	if (actualInterpolationStep > numOfInterSteps) {
		actualTrajectoryStep++;
		actualInterpolationStep = 0;
	}

	//at trajectory end then call trajectoryend transition
	if (actualTrajectoryStep == trj.xTrj.size()-1) {
		trj.clear();
		actualTrajectoryStep = 0;
		actualInterpolationStep = 0;
		timeStep = 0;
		currentState->trajectoryEnd();
	}
	timeStep++;
}



void Scara::cancelExecute() {
	currentState->cancelExecute();
}

bool Scara::isValidSequence(TimedAngles qTrjP) {
	if (isnan(qTrjP.q1) || isnan(qTrjP.q2) || isnan(qTrjP.q3) || isnan(qTrjP.q4)
			|| isnan(qTrjP.q5)) {
		return false;
	}

	bool q1InRange = (qTrjP.q1 > -1.68) && (qTrjP.q1 < 1.55);

	if (!q1InRange)
		return false;

//	bool q2InRange = (qTrjP.q2 > -2.0) && (qTrjP.q2 < 2.0);
//	if (!q2InRange)
//		return false;

//	bool q3InRange = (qTrjP.q3 > -1.0) && (qTrjP.q3 < 2.70);
//	if (!q3InRange)
//		return false;

	bool q4InRange = (qTrjP.q4 > -1.84) && (qTrjP.q4 < 2.05);

	if (!q4InRange)
		return false;

//	bool q5InRange = (qTrjP.q5 > 10) && (qTrjP.q5 < 255);
//
//	if (!q5InRange)
//		return false;

	return true;
}


bool Scara::isValidSingle(TimedAngles qTrjP) {
	if (isnan(qTrjP.q1) || isnan(qTrjP.q2) || isnan(qTrjP.q3) || isnan(qTrjP.q4)
			|| isnan(qTrjP.q5)) {
		return false;
	}

	bool q1InRange = (qTrjP.q1 > -1.68) && (qTrjP.q1 < 1.55);

	if (!q1InRange)
		return false;

	bool q2InRange = (qTrjP.q2 > -2.0) && (qTrjP.q2 < 2.0);
	if (!q2InRange)
		return false;

	bool q3InRange = (qTrjP.q3 > -1.0) && (qTrjP.q3 < 2.70);
	if (!q3InRange)
		return false;

	bool q4InRange = (qTrjP.q4 > -1.84) && (qTrjP.q4 < 2.05);

	if (!q4InRange)
		return false;

//	bool q5InRange = (qTrjP.q5 > 10) && (qTrjP.q5 < 255);
//
//	if (!q5InRange)
//		return false;

	return true;
}

void Scara::clearTrajectory() {
	trj.clear();
}
