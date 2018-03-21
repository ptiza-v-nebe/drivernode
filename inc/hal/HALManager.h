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

class HALManager  {
public:
    static HALManager& getInstance();
private:
    Encoder leftEncoder;
    Encoder rightEncoder;
    FaulhaberBLDC leftMotor;
    FaulhaberBLDC rightMotor;

    UART_HandleTypeDef motorUART;
private:
    HALManager();
public:
    Encoder& getLeftEncoder();
    Encoder& getRightEncoder();
    Motor& getLeftMotor();
    Motor& getRightMotor();
private:
    void initializeHal();
    void initializeEncoders();
    void initializeMotorUART();
};
#endif

#endif /* HAL_HALMANAGER_H_ */
/** @} */
