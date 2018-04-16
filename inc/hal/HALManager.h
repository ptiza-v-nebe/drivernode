/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#ifndef HAL_HALMANAGER_H_
#define HAL_HALMANAGER_H_

#include "config.h"

#ifdef BIG_ROBOT
#include "Encoder.h"
#include "FaulhaberBLDC.h"
#include "SRF08.h"
#include "PWM.h"
#include "StepperMotor.h"

constexpr int SRF08_COUNT = 4;

class HALManager {
public:
    static HALManager& getInstance();
private:
    Encoder leftEncoder; ///< the left encoder used for odometry
    Encoder rightEncoder; ///< the right encoder used for odometry
    FaulhaberBLDC leftMotor; ///< the left motor used for driving
    FaulhaberBLDC rightMotor; ///< the right motor used for driving
    SRF08 srf08[SRF08_COUNT]; ///< the ultrasonic sensor used for enemy detection when driving backwards

    PWM scaraLiftMotorPWM;
    StepperMotor scaraLiftMotor;
    Encoder scaraLiftEncoder;

    I2C_HandleTypeDef i2c; ///< the I²C to be used for the SRF08 sensors
    UART_HandleTypeDef motorUART; ///< the UART used for the Faulhaber BLDC motors

private:
    HALManager();
public:
    Encoder& getLeftEncoder();
    Encoder& getRightEncoder();
    Motor& getLeftMotor();
    Motor& getRightMotor();
    SRF08* getSRF08s();

    Motor& getScaraLiftMotor();
    Encoder& getScaraLiftEncoder();

    void enableISRs();

private:
    void initializeHal();
    void initializeEncoders();
    void initializeMotorUART();
    void initializeI2C();
    void initializeScara();
};
#endif

#endif /* HAL_HALMANAGER_H_ */
/** @} */
