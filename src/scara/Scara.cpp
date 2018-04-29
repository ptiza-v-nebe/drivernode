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

void Scara::commandReceived(const ScaraActionMessage& sam) {
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

void Scara::generateParkTrajectory() {
	trj.setActionTime(4);
	trj.startPose( { 82, 135, 100, M_PI / 2, M_PI / 2 });
	trj.addPose(TimeFactors::MEDIUM, { 82, 135, 250, M_PI / 2 });
	trj.addPose(TimeFactors::MEDIUM, { 0, 210, 250, M_PI / 2 });
	//trj.addQ(TimeFactors::MEDIUM,{pi/2, pi/2, pi*3/4,pi/10,pi/3}); //interpolieren in Gelenkkoordinaten
}

void Scara::generatePreventAttachTrajectory() {
	trj.setActionTime(2);

	Angles cap = { currentAnglesPosition.q1, currentAnglesPosition.q2,
			currentAnglesPosition.q3, currentAnglesPosition.q4,
			currentAnglesPosition.q5 };
	TimedPose p = trj.FK( { 0, cap.q1, cap.q2, cap.q3, cap.q4, cap.q5 });
	trj.startPose( { p.x, p.y, p.z, p.phi, p.theta });

	trj.addPose(TimeFactors::MEDIUM, { p.x, p.y, p.z + 20, p.phi, p.theta });
}

void Scara::generatePickCubeTrajectory(float x, float y, float phi,
		StorageSpace stg) {
	//set actionTime
	trj.setActionTime(15);

	Angles cap = { currentAnglesPosition.q1, currentAnglesPosition.q2,
			currentAnglesPosition.q3, currentAnglesPosition.q4,
			currentAnglesPosition.q5 };
	TimedPose p = trj.FK( { 0, cap.q1, cap.q2, cap.q3, cap.q4, cap.q5 });
	trj.startPose( { p.x, p.y, p.z, p.phi, p.theta });

	//move first to given x,y,phi
	trj.addPose(TimeFactors::FAST, { x, y, 100, phi, M_PI / 2 });
	trj.addPose(TimeFactors::SLOW, { x, y, 48, phi, M_PI / 2 }); //runter auf klotz saugen
	trj.addPose(TimeFactors::FAST, { x, y, 100, phi, M_PI / 2 }); // um klotze nicht zu zerst hoch

	float cx = 0;
	float cy = 0;
	float cz = 0;
	float cphi = 0;
	float ctheta = 0;

	if (stg == StorageSpace::INNER_1 || stg == StorageSpace::MIDDLE_1
			|| stg == StorageSpace::OUTER_1) {
		trj.addPose(TimeFactors::FAST, { x, y, 128, phi, M_PI / 2 }); // um klotze nicht zu zerst hoch
	}

	if (stg == StorageSpace::INNER_2 || stg == StorageSpace::MIDDLE_2
			|| stg == StorageSpace::OUTER_2) {
		trj.addPose(TimeFactors::FAST, { x, y, 196, phi, M_PI / 2 }); // um klotze nicht zu zerst hoch
	}

	if (stg == StorageSpace::INNER_3 || stg == StorageSpace::MIDDLE_3
			|| stg == StorageSpace::OUTER_3) {
		trj.addPose(TimeFactors::FAST, { x, y, 250, phi, M_PI / 2 }); // um klotze nicht zu zerst hoch
	}

//	//move cube befor storage
	cx = pLUT[static_cast<int>(stg)].x;
	cy = pLUT[static_cast<int>(stg)].y;
	cz = pLUT[static_cast<int>(stg)].z;
	cphi = pLUT[static_cast<int>(stg)].phi;
	ctheta = pLUT[static_cast<int>(stg)].theta;
	trj.addPose(TimeFactors::MEDIUM, { cx + 30, cy, cz, cphi, ctheta });

//	//put in
	cx = pLUT[static_cast<int>(stg)].x;
	cy = pLUT[static_cast<int>(stg)].y;
	cz = pLUT[static_cast<int>(stg)].z;
	cphi = pLUT[static_cast<int>(stg)].phi;
	ctheta = pLUT[static_cast<int>(stg)].theta;
	trj.addPose(TimeFactors::MEDIUM, { cx, cy, cz, cphi, ctheta });
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
	//trj.qTrj[trjElement]
	return trj.ik1(interPose);
}

void Scara::executeTrajectory() {
	currentTime = timeStep * 0.010;

	int numOfInterSteps = 10;

	//calculate first angle configuration
	if((trj.xTrj.size() > 0) && (actualTrajectoryStep == 0)){
		itaLast = calculateQ(actualTrajectoryStep, actualInterpolationStep,
						numOfInterSteps);
	}

	//calculate next angle configuration
	if ((trj.xTrj.size() > 0) && (actualTrajectoryStep > 0)) {
		itaActual = calculateQ(actualTrajectoryStep, actualInterpolationStep+1,
				numOfInterSteps);
	}

	//send command of position and speed to motor
	if ((currentTime > itaActual.t) && (trj.xTrj.size() > 0)) {
		if (!isValid(itaActual)) {
			printf("[Non Valid]: t:%f,q1:%f,q1:%f,q1:%f,q1:%f,q1:%f \r\n",itaActual.t,itaActual.q1,itaActual.q2,itaActual.q3,itaActual.q4,itaActual.q5);
			currentState->cancelExecute();
		} else {
			float tpm = (itaActual.t-itaLast.t)/(2*M_PI);

			servos[0].moveTo(itaActual.q1 + 150 * M_PI / 180);
			servos[0].setRPM((itaActual.q1-itaLast.q1)/tpm);

			servos[1].moveTo(itaActual.q2 + 150 * M_PI / 180);
			servos[1].setRPM((itaActual.q2-itaLast.q2)/tpm);

			servos[2].moveTo(itaActual.q3 + 60 * M_PI / 180);
			servos[2].setRPM((itaActual.q1-itaLast.q1)/tpm);

			servos[3].moveTo(itaActual.q4 + 105 * M_PI / 180);
			servos[3].setRPM((itaActual.q1-itaLast.q1)/tpm);

			lift.moveTo(itaActual.q5);

			actualInterpolationStep++;
			itaLast = itaActual;
		}
	}

	//increment trajectory step
	if (actualInterpolationStep > numOfInterSteps - 1) {
		actualTrajectoryStep++;
		actualInterpolationStep = 0;
	}

	//at trajectory end then call trajectoryend transition
	if (actualTrajectoryStep >= trj.xTrj.size()) {
		trj.clear();
		actualTrajectoryStep = 0;
		actualInterpolationStep = 0;
		currentState->trajectoryEnd();
	}
	timeStep++;
}



void Scara::cancelExecute() {
	currentState->cancelExecute();
}

bool Scara::isValid(TimedAngles qTrjP) {
	if (isnan(qTrjP.q1) || isnan(qTrjP.q2) || isnan(qTrjP.q3) || isnan(qTrjP.q4)
			|| isnan(qTrjP.q5)) {
		return false;
	}

	bool q1InRange = (qTrjP.q1 > -1.57) && (qTrjP.q1 < 1.55);

	if (!q1InRange)
		return false;

	bool q2InRange = (qTrjP.q2 > -2.0) && (qTrjP.q2 < 2.0);
	if (!q2InRange)
		return false;

	bool q3InRange = (qTrjP.q3 > -2.82) && (qTrjP.q3 < 2.70);
	if (!q3InRange)
		return false;

	bool q4InRange = (qTrjP.q4 > -1.84) && (qTrjP.q4 < 2.05);

	if (!q4InRange)
		return false;

	bool q5InRange = (qTrjP.q5 > 59) && (qTrjP.q5 < 253);

	if (!q5InRange)
		return false;

	return true;
}

void Scara::clearTrajectory() {
	trj.clear();
}
