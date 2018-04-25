/*
 * Scara.cpp
 *
 *  Created on: 22 Apr 2018
 *      Author: ivan-zarevich
 */

#include "scara/Scara.h"

Scara::Scara(ScaraHardware& hw) :
		lift(hw.getLiftMotor(), hw.getLiftEncoder()), servos(hw.getArmServos()), runOnce(
				false),i(0),qTrj(),j(0) {
	servos[0].enable();
	servos[1].enable();
	servos[2].enable();
	servos[3].enable();
	//lift.initialize();
}

Scara::~Scara() {

}

QTrajectory Scara::buildTask() {
	Trajectory xtrj1;

	xtrj1.setActionTime(10);
    xtrj1.startPose({180,0,51,0}); //maybe you can get the actual angles

    xtrj1.addPose(TimeFactors::FAST,{0,110,200,2});
    xtrj1.addPose(TimeFactors::MEDIUM,{180,20,220,0});
    xtrj1.addPose(TimeFactors::SLOW,{180,40,180,0});
    xtrj1.addPose(TimeFactors::FAST,{180,0,150,0});

	QTrajectory qtrj1 = xtrj1.buildJointspace();
	return qtrj1;
}

void Scara::execute() {
	currentTime = i*0.010;

	double goalTime = qTrj[j][0];

	if(currentTime > goalTime){
		servos[0].moveTo(qTrj[j][1]+150*M_PI/180);
		servos[1].moveTo(qTrj[j][2]+150*M_PI/180);
		servos[2].moveTo(qTrj[j][3]+60*M_PI/180);
		j++;
	}
	i++;
}

void Scara::initialize() {

	qTrj = buildTask();

	runOnce = true;
}
