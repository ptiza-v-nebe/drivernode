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
#include "hal/Encoder.h"
#include "hal/FaulhaberBLDC.h"
#include "hal/SRF08.h"

constexpr int SRF08_COUNT = 4;

class HALManager  {
public:
    static HALManager& getInstance();
private:
    Encoder leftEncoder;
    Encoder rightEncoder;
    FaulhaberBLDC leftMotor;
    FaulhaberBLDC rightMotor;
    SRF08 srf08[SRF08_COUNT];

    I2C_HandleTypeDef i2c;
    UART_HandleTypeDef motorUART;

private:
    HALManager();
public:
    Encoder& getLeftEncoder();
    Encoder& getRightEncoder();
    Motor& getLeftMotor();
    Motor& getRightMotor();
    SRF08* getSRF08s();
private:
    void initializeHal();
    void initializeEncoders();
    void initializeMotorUART();
    void initializeI2C();
};
#endif

#endif /* HAL_HALMANAGER_H_ */
/** @} */
