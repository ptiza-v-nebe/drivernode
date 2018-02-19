/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#ifndef HAL_HALMANAGERBIGROBOT_H_
#define HAL_HALMANAGERBIGROBOT_H_

#include <hal/HALManager.h>
#include "hal/Encoder.h"

class HALManagerBigRobot: public HALManager {
public:
    static HALManagerBigRobot& getInstance();
private:
    Encoder leftEncoder;
    Encoder rightEncoder;
private:
    HALManagerBigRobot();
public:

    virtual Encoder& getLeftEncoder() override;
    virtual Encoder& getRightEncoder() override;
private:
    void initializeHal();
    void initializeEncoders();
};

#endif /* HAL_HALMANAGERBIGROBOT_H_ */
/** @} */
