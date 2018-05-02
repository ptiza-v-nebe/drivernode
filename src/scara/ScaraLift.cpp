/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Scara
 * @{
 ******************************************************************************
 */

#include <scara/ScaraLift.h>
#include <scheduler/Scheduler.h>
#include <position/Angle.h>
#include "util/util.h"

static constexpr int16_t DELTA_MIN = 50;
static constexpr int16_t DELTA_MAX = 500;
static constexpr int16_t ACCURACY = 50;
static constexpr float DELTA_TO_SPEED = 8.0f;

static constexpr int16_t MAX_POSITION = 253; // mm from floor
static constexpr int16_t MIN_POSITION = 53; // mm from floor

static constexpr float MOTORCONSTANT = 32*200*(1/(2*PI*7)); // in mm
static constexpr float MM_PER_TICK = (253.0f-53)/7150;



ScaraLift::ScaraLift(Motor& motor, Encoder& encoder, InputPin& endStop) :
        motor(motor), encoder(encoder), endStop(endStop), startPosition(0), targetPosition(53), //
        initialized(false),  lastSpeed(0.0) , currentPosition(53){
}

void ScaraLift::tick() {
	if(!initialized){
		return;
	}
	currentPosition = encoder.getTick()*MM_PER_TICK+53;

	float speed = 0;
	//float MAX_ACCELERATION = 1000;

	//if go up, we can use more power on controller
	if((targetPosition-currentPosition) > 0){
		//MAX_ACCELERATION = 3000; //3000
		speed = 4*(targetPosition-currentPosition); //P = 2
	}

	//if go down we have earth acceleration, we have to go slower with controller
	if((targetPosition-currentPosition) < 0){
		//MAX_ACCELERATION = 3000;
		speed = 3*(targetPosition-currentPosition); //P = 2
	}

//    float rate = (speed - lastSpeed)/0.01;
//
//    if(rate > MAX_ACCELERATION) {
//    	speed = lastSpeed + MAX_ACCELERATION*0.01;
//    } else if(rate < -MAX_ACCELERATION) {
//    	speed = lastSpeed - MAX_ACCELERATION*0.01;
//    }

	//printf("Ticks: %d pos: %d speed: %f rate %f\r\n", encoder.getTick(), currentPosition, speed, rate);

	lastSpeed = speed;

    // calculat
    motor.setSpeed(MOTORCONSTANT*speed);
}

void ScaraLift::initialize() {
    encoder.reset();
    motor.enable();
    motor.stop();
}

void ScaraLift::stop(){
	motor.stop();
}

void ScaraLift::moveTo(float mm) {
    if (!initialized) {
        return;
    }
    targetPosition = clamp<int16_t>(static_cast<int16_t>(mm), MIN_POSITION, MAX_POSITION);
}

float ScaraLift::getPosition() {
    return encoder.getTick();
}

bool ScaraLift::tickInit() {
    if(endStop.isOn()) {
        encoder.reset();
        motor.stop();
        initialized = true;
        return true;
    } else {
        motor.setSpeed(-5000);
        return false;
    }
}
/** @} */
