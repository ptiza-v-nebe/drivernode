/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#ifndef HAL_SCARAHARDWARE_H_
#define HAL_SCARAHARDWARE_H_

#include "PWM.h"
#include "StepperMotor.h"
#include "Encoder.h"
#include "InputPin.h"
#include "OutputPin.h"
#include "DynamixelAX12A.h"

constexpr int STORAGE_PUMP_COUNT = 3;
constexpr int DYNAMIXEL_COUNT = 4;

class ScaraHardware {
private:
    PWM motorPWM; ///< the pwm for the scara lift motor
    StepperMotor motor; ///< the motor for the scara lift
    Encoder encoder; ///< the encoder on the scara
    InputPin endSwitch; ///< the end switch
    OutputPin pump; ///< the pump on the scara
    OutputPin valve; ///< the valve on the scara sucker
    DynamixelAX12A armServos[DYNAMIXEL_COUNT]; ///< the serovs in the scara

    OutputPin storagePumps[STORAGE_PUMP_COUNT]; ///< the storage pumps

public:
    ScaraHardware(DynamixelCOM& dynamixelCOM);

    void initialize();
    void disable();

    Motor& getLiftMotor();
    Encoder& getLiftEncoder();
    InputPin& getEndSwitch();
    OutputPin& getPump();
    OutputPin& getValve();
    OutputPin* getStoragePumps();

    DynamixelAX12A* getArmServos();

private:
    void initializePWM();
    void initializeEncoder();
};

// ///////////////////////////////////////////////////////////////////////////////
// Shared Scara Config
// ///////////////////////////////////////////////////////////////////////////////
/**
 * GPIO Bank for left encoder.
 * @attention when changing this, also change RCC clock enable in
 *            ScarHardware::initializeEncoder!
 */
#define SCARA_ENCODER_GPIO GPIOB

/**
 * channel a pin for scara encoder.
 * @attention changing this might require using another IRQ/ISR in HALManager
 */
static constexpr uint16_t SCARA_ENCODER_A = GPIO_PIN_2;

/**
 * channel b pin for scara encoder.
 * @attention changing this might require using another IRQ/ISR in HALManager
 */
static constexpr uint16_t SCARA_ENCODER_B = GPIO_PIN_1;

/**
 * should the scara encoder be inverted?
 */
static constexpr bool SCARA_ENCODER_INVERT = false;

#endif /* HAL_SCARAHARDWARE_H_ */
/** @} */
