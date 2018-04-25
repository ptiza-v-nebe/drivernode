/*
 * Scara.cpp
 *
 *  Created on: 22 Apr 2018
 *      Author: ivan-zarevich
 */

#include "scara/Scara.h"

Scara::Scara(ScaraHardware& hw) :
		lift(hw.getLiftMotor(), hw.getLiftEncoder()), servos(hw.getArmServos()), runOnce(
				false),i(0),qTrj(),j(0),currentTime(0),lastTime(0) {
	servos[0].enable();
	servos[1].enable();
	servos[2].enable();
	servos[3].enable();
	lift.initialize();
}

Scara::~Scara() {

}

void Scara::execute() {
	currentTime = i*0.010;
	double goalTime = qTrj[j][0];

	//vector<double> q = trj.ik1({0,30,93,0,M_PI*0.95,0});
	//vector<double> q = trj.ik1({0,30,150,0,M_PI*3/4,0});
	//vector<double> q = trj.ik1({0,20,211,0,M_PI/2,0});
	//printf("q1:%f,q2:%f,q3:%f,q4:%f", q[1],q[2],q[3],q[4]);

	//	servos[0].moveTo(q[1]+150*M_PI/180);
	//	servos[1].moveTo(q[2]+150*M_PI/180);
	//	servos[2].moveTo(q[3]+60*M_PI/180);
	//	servos[3].moveTo(q[4]+150*M_PI/180);

	if((currentTime > goalTime) && (j < qTrj.size()-1)){
		servos[0].moveTo(qTrj[j][1]+150*M_PI/180);
		servos[1].moveTo(qTrj[j][2]+150*M_PI/180);
		servos[2].moveTo(qTrj[j][3]+60*M_PI/180);
		servos[3].moveTo(qTrj[j][4]+150*M_PI/180);
		lift.moveTo(qTrj[j][5]);
		j++;
	}
	i++;
	lift.tick();

}

void Scara::initialize() {
	trj.setActionTime(10);
	trj.startPose({150, 67.5,37,M_PI/2,0});

    trj.addPose(TimeFactors::FAST,{50,93,50,M_PI*0.95,0});
    trj.addPose(TimeFactors::MEDIUM,{30,93,50,M_PI*0.95,0});
    trj.addPose(TimeFactors::FAST,{50,93,50,M_PI*0.95,0});

    trj.addPose(TimeFactors::FAST,{50,150,100,M_PI*3/4,0});
    trj.addPose(TimeFactors::MEDIUM,{30,150,100,M_PI*3/4,0});
    trj.addPose(TimeFactors::FAST,{50,150,100,M_PI*3/4,0});

    trj.addPose(TimeFactors::FAST,{40,211,200,M_PI/2,0});
    trj.addPose(TimeFactors::MEDIUM,{30,211,200,M_PI/2,0});
    trj.addPose(TimeFactors::FAST,{40,211,200,M_PI/2,0});

	qTrj = trj.buildJointspace();
	runOnce = true;
}
