/*
 * Scara.cpp
 *
 *  Created on: 22 Apr 2018
 *      Author: ivan-zarevich
 */

#include "scara/Scara.h"

Scara::Scara(ScaraHardware& hw) :
		lift(hw.getLiftMotor(), hw.getLiftEncoder()), servos(hw.getArmServos()), runOnce(
				false),i(0),qTrj() {
	servos[0].enable();
	servos[1].enable();
	servos[2].enable();
	servos[3].enable();
	lift.initialize();
}

Scara::~Scara() {

}

QTrajectory Scara::buildTask() {
	Trajectory xtrj1;

	xtrj1.setActionTime(10);
    xtrj1.startPose({180,0,0,0}); //maybe you can get the actual angles

    xtrj1.addPose(TimeFactors::FAST,{160,20,1,2});
    xtrj1.addPose(TimeFactors::SLOW,{180,20,0,0});
    xtrj1.addPose(TimeFactors::SLOW,{180,40,0,0});
    xtrj1.addPose(TimeFactors::MEDIUM,{180,40,0,0});

	QTrajectory qtrj1 = xtrj1.buildJointspace();
	return qtrj1;
}

void Scara::execute() {
	Trajectory trj;
	//Q q = trj.ik2({0,180,0,0,0});

	printf("Execute invoked. q1: %f, q2: %f, q3: %f, q4: %f\n\r",qTrj[i][1] ,qTrj[i][2],qTrj[i][3],qTrj[i][4]);
	servos[0].moveTo(qTrj[i][1]+150*M_PI/180);
	servos[1].moveTo(qTrj[i][2]+150*M_PI/180);
	servos[2].moveTo(qTrj[i][3]+60*M_PI/180);
	i++;

//	servos[0].moveTo(M_PI/8+150*M_PI/180);
//	servos[1].moveTo(0+150*M_PI/180);
//	servos[2].moveTo(0+60*M_PI/180);

	//servos[3].moveTo(q[3] + 150*M_PI/180);
//	if(runOnce){
//		lift.moveTo(q[4]);
//		runOnce = false;
//	}

}

void Scara::initialize() {
	//lift.moveTo(3000);
	qTrj = buildTask();

	runOnce = true;
}
