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

#include <control/Clocked.h>
#include <control/ClockedInitializable.h>

#include "serial/MessageDispatcher.h"

class Scara : public Clocked, public ClockedInitializable {
private:
	ScaraLift& lift;
	DynamixelAX12A *servos;
	OutputPin& scaraPump;
	OutputPin& scaraValve;
	OutputPin* storagePumps;

	MessageDispatcher& md;

	bool runOnce;
	Trajectory trj;
	int timeStep;
	ScaraBaseState* currentState;
	Pose pLUT[12];

	Angles currentAnglesPosition;
	int actualTrajectoryStep;
	int actualInterpolationStep;

	TimedAngles itaActual;
	TimedAngles itaLast;
	StorageSpace storageSpace;

	float currentTime;

public:
	Scara(ScaraHardware& hw, ScaraLift& lift, MessageDispatcher& md);
	virtual ~Scara();
	void executeTrajectory();

	void tick() override;
	void startInitializing() override;
	bool tickInit() override;

	void cancelExecute(); //unterbrechung jeglicher Aktionen
	void generatePickCubeTrajectory(float x, float y, float phi, StorageSpace storage);
	void generatePreventAttachTrajectory();
	void generateParkTrajectory();
	void generateTickSwitchTrajectory();
	void commandReceived(const ScaraActionMessage& sam);
	void park();
	void scaraPumpValveControl(bool on);
	void storagePumpsControl(StorageSpace sam);
	void disableStoragePumps();
	bool isValidSequence(TimedAngles qTrjP);
	bool isValidSingle(TimedAngles qTrjP);
	void showQPoint(TimedAngles qTrj);
	void finalPark();
	void startPark();
	void monitoring();
	void tickSwitch();

	void sendFinishedMessage();

	TimedAngles calculateQ(int trjElement, int interElement, int numOfInterPoints);

	TimedAngles readMotorAngles();
	void clearTrajectory();
};

#endif /* SCARA_SCARA_H_ */
