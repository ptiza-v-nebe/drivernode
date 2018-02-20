/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#ifndef HAL_DYNAMIXELCOM_H_
#define HAL_DYNAMIXELCOM_H_

#include "hal/DynamixelUART.h"

constexpr uint8_t DYNAMIXEL_PING = 0x01;
constexpr uint8_t DYNAMIXEL_READ = 0x02;
constexpr uint8_t DYNAMIXEL_WRITE = 0x03;

class DynamixelCOM {
private:
    DynamixelUART uart;
public:
    void sendInstruction(const uint8_t id, const uint8_t instruction, const uint8_t* parameters = nullptr, const int paramCount = 0);
    int readStatus(uint8_t* buffer = nullptr, int size = 0);
private:
    uint8_t calculateChecksum(const uint8_t* msg, const int size);
};

#endif /* HAL_DYNAMIXELCOM_H_ */
/** @} */
