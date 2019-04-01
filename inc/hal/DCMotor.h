/*
 * DCMotor.h
 *
 *  Created on: 31 Mar 2019
 *      Author: ivan-zarevich
 */

#ifndef HAL_DCMOTOR_H_
#define HAL_DCMOTOR_H_

#include <hal/Motor.h>
#include "PWM.h"
#include "OutputPin.h"
#include <tgmath.h>

class DCMotor: public Motor  {
private:
	PWM& in2_pwm;
	OutputPin in1_gpio;
	OutputPin motor_enable_gpio;

	bool enabled; ///< is the motor enabled
	bool reverseDirection; ///< should the direction be inverted
public:
	DCMotor(PWM& in2_pwm, OutputPin&& in1_gpio, OutputPin&& motor_enable_gpio, bool reverseDirection);

    // prevent copy and move
	DCMotor(const DCMotor&) = delete;
	DCMotor(DCMotor&&) = delete;
	DCMotor& operator=(const DCMotor&) = delete;
	DCMotor& operator=(DCMotor&&) = delete;

    virtual void enable() override;
    virtual void disableAndStop() override;
    virtual void setSpeed(int32_t speed) override;
    virtual void stop() override;

    void setDirection(int32_t direction);
    void start();
    void initialize();
};

#endif /* HAL_DCMOTOR_H_ */
