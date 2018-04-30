/*
 * ScaraStates.cpp
 *
 *  Created on: 26 Apr 2018
 *      Author: ivan-zarevich
 */
//nichts in den Klassen der Zustaenden was merken!


#include <scara/ScaraStates.h>
#include "serial/messages/ScaraActionMessage.h"
#include "scara/Scara.h"

void Park::entry() {
	//generate trajectory for going to park
	//1.clear previous trajectory
	context.clearTrajectory();
	//2.generate new trajectory
	context.generateParkTrajectory();
}

void Park::tick() {
	context.executeTrajectory();
}

bool Park::tickInit() {
	tick();
	return false;
}

void Park::trajectoryEnd(){
	context.finalPark();
	CHANGE_STATE(IdleScara);
}

void Park::cancelExecute(){
	CHANGE_STATE(IdleScara);
}

//this state if entered will stop all movement
void IdleScara::commandReceived(const ScaraActionMessage& sam) {
	printf("[ScaraStates.cpp] command received\r\n");
	//generate trajectory
	context.generatePickCubeTrajectory(sam.getX(), sam.getY(), sam.getPhi().getAngleInRadian(), sam.getStorageSpace());
	//scara pumpe anschalten, scara ventil anschalten
	context.scaraPumpValveControl(true);
	//lagerpumpen anmachen, steckt im sam
	context.storagePumpsControl(sam.getStorageSpace());
	CHANGE_STATE(PickCube); // changestate ist das letzte in der funktion
}

void IdleScara::park() {
	CHANGE_STATE(Park);
}

void IdleScara::entry(){
	//clear previous trajectory
	context.clearTrajectory();
	//deactivate pumps
	context.scaraPumpValveControl(false);
}

//this state pick up cube and transport it storage

void PickCube::trajectoryEnd() {
	//scara pumpe aus, scara ventil auf
	context.scaraPumpValveControl(false);
	CHANGE_STATE(PreventAttach);
}

void PickCube::cancelExecute(){
	CHANGE_STATE(IdleScara);
}

void PickCube::tick() {
	context.executeTrajectory();
}

//this state will prevent attaching cube on next picking up session
void PreventAttach::trajectoryEnd() {
	CHANGE_STATE(IdleScara);
}

void PreventAttach::entry(){
	context.clearTrajectory();
	context.generatePreventAttachTrajectory();
}

void PreventAttach::cancelExecute(){
	CHANGE_STATE(IdleScara);
}

void PreventAttach::tick() {
	context.executeTrajectory();
}
