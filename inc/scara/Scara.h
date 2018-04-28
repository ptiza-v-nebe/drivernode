/*
 * Scara.h
 *
 *  Created on: 22 Apr 2018
 *      Author: ivan-zarevich
 */

#ifndef SCARA_SCARA_H_
#define SCARA_SCARA_H_
#include <vector>
#include "scara/ScaraLift.h"
#include "hal/DynamixelAX12A.h"
#include "hal/ScaraHardware.h"
#include <math.h>
#include "scara/Trajectory.h"
#include "serial/messages/ScaraActionMessage.h"
#include "scara/ScaraStates.h"



class Scara {
private:
	ScaraLift lift;
	DynamixelAX12A *servos;
	OutputPin& scaraPump;
	OutputPin& scaraValve;
	OutputPin* storagePumps;

	bool runOnce;
	vector<TimedAngles> qTrj;
	Trajectory trj;
	int i;
	double lastTime;
	double currentTime;
	int j;
	bool positionSet;
	ScaraBaseState* currentState;
	Pose pLUT[12];
	StorageSpace storageSpace;

	Angles currentAnglesPosition;

public:
	Scara(ScaraHardware& hw);
	virtual ~Scara();
	void executeTrajectory();
	void tick();
	void initialize();
	void cancelExecute(); //unterbrechung jeglicher Aktionen
	void generatePickCubeTrajectory(float x, float y, float phi, StorageSpace storage);
	void generatePreventAttachTrajectory();
	void generateParkTrajectory();
	void task();
	void commandReceived(const ScaraActionMessage& sam);
	void park();
	void scaraPumpValveControl(bool on);
	void storagePumpsControl(StorageSpace sam);
	void disableStoragePumps();
	bool isValid(TimedAngles qTrjP);
	void showQPoint(TimedAngles qTrj);

	TimedAngles readMotorAngles();
	void clearTrajectory();
};

#endif /* SCARA_SCARA_H_ */
