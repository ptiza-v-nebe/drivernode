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

// /////////////////////////////////////////////////////////////
// Common Includes
// /////////////////////////////////////////////////////////////
#include "config.h"

#include "Encoder.h"
#include "SRF08.h"
#include "PWM.h"
#include "StepperMotor.h"
#include "DynamixelCOM.h"
#include "InputPin.h"

// /////////////////////////////////////////////////////////////
// Specific Includes and Constants
// /////////////////////////////////////////////////////////////
#ifdef BIG_ROBOT
#include "FaulhaberBLDC.h"
#include "ScaraHardware.h"

constexpr int SRF08_COUNT = 2;
#endif
#ifdef SMALL_ROBOT
#include "StepperMotor.h"
#include "ShootingBLDC.h"
#include "DynamixelAX12A.h"
constexpr int SRF08_COUNT = 2;
#endif

// /////////////////////////////////////////////////////////////
// Common
// /////////////////////////////////////////////////////////////
class HALManager {
public:
    static HALManager& getInstance();
private:
    // /////////////////////////////////////////////////////////////
    // Common Members
    // /////////////////////////////////////////////////////////////
    Encoder leftEncoder; ///< the left encoder used for odometry
    Encoder rightEncoder; ///< the right encoder used for odometry
    SRF08 srf08[SRF08_COUNT]; ///< the ultrasonic sensor used for enemy detection when driving backwards
    DynamixelCOM dynamixelCom; ///< the COM object for the dynamixels
    InputPin starterSwitch; ///< the starter switch
    OutputPin statusLED; ///< the status LED
    OutputPin errorLED; ///< the error LED

    I2C_HandleTypeDef i2c; ///< the I²C to be used for the SRF08 sensors

    // /////////////////////////////////////////////////////////////
    // Specific Members
    // /////////////////////////////////////////////////////////////
#ifdef BIG_ROBOT
    ScaraHardware scaraHardware; ///< the scara hardware

    FaulhaberBLDC leftMotor; ///< the left motor used for driving
    FaulhaberBLDC rightMotor; ///< the right motor used for driving
    UART_HandleTypeDef motorUART; ///< the UART used for the Faulhaber BLDC motors
#endif
#ifdef SMALL_ROBOT
    PWM leftMotorPWM; ///< the PWM for the left motor.
    PWM rightMotorPWM;/// the PWM for the right motor
    StepperMotor leftMotor;///< the left motor used for driving
    StepperMotor rightMotor;///< the right motor used for driving

    PWM shootingBLDCPWM;///<
    ShootingBLDC shootingBLDC;///< the BLDC used to shoot
    DynamixelAX12A servo;///< the dynamixel used for bee and switch
#endif

    // /////////////////////////////////////////////////////////////
    // Common Functions
    // /////////////////////////////////////////////////////////////
private:
    HALManager();
public:
    Encoder& getLeftEncoder();
    Encoder& getRightEncoder();
    Motor& getLeftMotor();
    Motor& getRightMotor();
    SRF08* getSRF08s();
    InputPin& getStarterPin();
    OutputPin& getStatusLED();
    OutputPin& getErrorLED();

    void enableISRs();
    void disableAllActors();

private:
    void initializeHal();
    void initializeEncoders();
    void initializeI2C();

    // /////////////////////////////////////////////////////////////
    // Specific Functions
    // /////////////////////////////////////////////////////////////
#ifdef BIG_ROBOT
public:
    ScaraHardware& getScaraHardware();

private:
    void initializeMotorUART();
#endif
#ifdef SMALL_ROBOT
public:
    ShootingBLDC& getShootingBLDC();
    DynamixelAX12A& getServo();
private:
    void initializeMotors();
    void initializeShootingBLDC();
#endif
};

#endif /* HAL_HALMANAGER_H_ */
/** @} */
