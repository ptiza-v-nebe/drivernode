/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * @author Cagri Erdogan
 * @author Moritz Höwer
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#ifndef SRF08_H_
#define SRF08_H_

#include "stm32l4xx.h"
#include <cstdint>

class SRF08 {
private:
    I2C_HandleTypeDef *i2c; ///< handle for the I²C
    uint8_t address; ///< the address of the sensor

public:
    SRF08(I2C_HandleTypeDef *i2c, uint8_t address);

    void startRanging();
    bool rangingFinished();

    uint16_t getRange();
    uint8_t getLightIntensity();

    void setMaxRange(uint16_t range);
    void setMaxGainRegister(uint8_t gainVal);
    void setAddress(uint8_t newAddress);
};

#endif /* SRF08_H_ */
/** @} */
