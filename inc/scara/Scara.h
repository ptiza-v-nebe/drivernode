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

//static constexpr double ZBase = 0;
//static constexpr double ZLimit = 7100;

//using Pose = std::vector<std::vector<double>>;
//using Q = std::vector<std::vector<double>>;

//using Pose = std::vector<float>;
//using Q = std::vector<float>;

struct Pose {
	float x;
	float y;
	float z;
	float phi;
	float theta;
};

class Scara {
private:
	ScaraLift lift;
	DynamixelAX12A *servos;
	OutputPin& scaraPump;
	OutputPin& scaraValve;
	OutputPin* storagePumps;

	bool runOnce;
	QTrajectory qTrj;
	Trajectory trj;
	int i;
	double lastTime;
	double currentTime;
	int j;
	bool positionSet;
	ScaraBaseState* currentState;
	Pose pLUT[12];
	StorageSpace storageSpace;

public:
	Scara(ScaraHardware& hw);
	virtual ~Scara();
	void executeTrajectory();
	void tick();
	void initialize();
	void cancelExecute(); //unterbrechung jeglicher Aktionen
	void generateTrajectoryForCube(float x, float y, float phi, StorageSpace storage);
	void task();
	void commandReceived(const ScaraActionMessage& sam);
	void park();
	void scaraPumpValveControl(bool on);
	void storagePumpsControl(StorageSpace sam);
	void disableStoragePumps();
	bool isValid(vector<float> qTrjP);
	void showQPoint(vector<double> qTrj);
	void generateTrajectoryForLiftUp();
	vector<float> readMotorAngles();
};

#endif /* SCARA_SCARA_H_ */
