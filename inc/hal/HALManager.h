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

constexpr int SRF08_COUNT = 4;
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
};

#endif /* HAL_HALMANAGER_H_ */
/** @} */
