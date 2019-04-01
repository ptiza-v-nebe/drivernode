/*
 * DCMotor.cpp
 *
 *  Created on: 31 Mar 2019
 *      Author: ivan-zarevich
 */

#include "hal/DCMotor.h"
#include <utility>

/**
 * prescaler for the pwm
 */
static constexpr int PRESCALE = 1; // timer base frequency = 80MHz / 1 = 80 MHz

/**
 * frequency for the pwm
 */
static constexpr int FREQUENCY = 25000; // ESC uses 50 Hz

/**
 * duty cycle percentage for stopping the motor
 */
static constexpr float STOP_PERCENT = 0.05f;

/**
 * duty cycle percentage for running the motor
 */
static constexpr float ON_PERCENT = 0.058f;


/**
 * the maximum speed of the motor
 */
static constexpr int16_t MAX_SPEED = 13000;

/**
 * the minimum speed of the motor
 */
static constexpr int16_t MIN_SPEED = -13000;

DCMotor::DCMotor(PWM& in2_pwm, OutputPin&& in1_gpio, OutputPin&& motor_enable_gpio, bool reverseDirection):
		in2_pwm(in2_pwm), enabled(false),
		reverseDirection(reverseDirection),
		in1_gpio(std::move(in1_gpio)),
		motor_enable_gpio(std::move(motor_enable_gpio)){
	initialize();
}

/*
 * @see - Actor::enable()
 */
void DCMotor::enable() {
	motor_enable_gpio.setOn();
    enabled = true;
}

/*
 * @see - Actor::disableAndStop()
 */
void DCMotor::disableAndStop() {
	motor_enable_gpio.setOff();
    stop();
    enabled = false;
}

/*
 * @see - Motor::stop()
 */
void DCMotor::stop() {
    setSpeed(0);
}

/**
 * initializes the dc motor
 */
void DCMotor::initialize() {
    this->in1_gpio.enable();
    this->motor_enable_gpio.enable();

	in2_pwm.setPrescale(PRESCALE);
	in2_pwm.setFrequency(FREQUENCY);

	in2_pwm.setDutyCycle(0);
	in2_pwm.enable();
}

/*
 * @see - Motor::setSpeed(uint16_t)
 */
void DCMotor::setSpeed(int32_t speed) {
    if (!enabled) {
        return;
    }

    if(reverseDirection) {
        speed = -speed;
    }

    if (speed > MAX_SPEED) {
        speed = MAX_SPEED;
    }
    if(speed < MIN_SPEED) {
        speed = MIN_SPEED;
    }

    setDirection(speed);
    in2_pwm.setDutyCycle(fabs(static_cast<double>(speed)/MAX_SPEED));
}

/**
 * sets the direction.
 *
 * @param direction positive means forward, negative means backward
 */
void DCMotor::setDirection(int32_t direction) {
	if(direction == 0){
		return;
	}

    if (direction > 0) {
    	in2_pwm.setPolarity(TIM_OCNPOLARITY_HIGH);
    	in1_gpio.setOn();
    }
    if (direction < 0){
    	in2_pwm.setPolarity(TIM_OCNPOLARITY_LOW);
    	in1_gpio.setOff();
    }
}

