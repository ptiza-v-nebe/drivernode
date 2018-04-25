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

	//vector<double> ap =
	//xtrj1.FK({q1,q2,q3,q4});
    //xtrj1.startPose({ap[0],ap[1],0,ap[3]}); //maybe you can get the actual angles
	xtrj1.startPose({82.5, 67.5,0,0});

    xtrj1.addPose(TimeFactors::FAST,{40,100,0,0});
    xtrj1.addPose(TimeFactors::SLOW,{0,100,0,0});
    xtrj1.addPose(TimeFactors::FAST,{40,100,0,0});

    xtrj1.addPose(TimeFactors::FAST,{40,160,0,0});
    xtrj1.addPose(TimeFactors::SLOW,{0,160,0,0});
    //xtrj1.addPose(TimeFactors::FAST,{40,160,0,0});

//    xtrj1.addPose(TimeFactors::FAST,{100,100,0,0});
//    xtrj1.addPose(TimeFactors::FAST,{82.5, 67.5,0,0});

    //xtrj1.addPose(TimeFactors::FAST,{40,210,0,0});
    //xtrj1.addPose(TimeFactors::SLOW,{0,210,0,0});
    //xtrj1.addPose(TimeFactors::FAST,{40,210,0,0});

	QTrajectory qtrj1 = xtrj1.buildJointspace();

	xtrj1.showQTrajectory(qtrj1);
	return qtrj1;
}

void Scara::execute() {
	currentTime = i*0.010;

	double goalTime = qTrj[j][0];

//	servos[0].moveTo(qTrj[j][1]+150*M_PI/180);
//	servos[1].moveTo(qTrj[j][2]+150*M_PI/180);
//	servos[2].moveTo(qTrj[j][3]+60*M_PI/180);
//	servos[3].moveTo(qTrj[j][4]+150*M_PI/180);

	if(currentTime > goalTime){
		servos[0].moveTo(qTrj[j][1]+150*M_PI/180);
		servos[1].moveTo(qTrj[j][2]+150*M_PI/180);
		servos[2].moveTo(qTrj[j][3]+60*M_PI/180);
		servos[3].moveTo(qTrj[j][4]+150*M_PI/180);
		j++;
	}
	i++;
}

void Scara::initialize() {

	qTrj = buildTask();

	runOnce = true;
}
