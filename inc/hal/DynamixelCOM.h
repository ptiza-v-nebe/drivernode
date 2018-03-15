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

class DynamixelCOM {
private:
    DynamixelUART uart;
public:
    uint8_t ping(const uint8_t id);
    uint8_t writeByte(const uint8_t id, const uint8_t address, const uint8_t data);
    uint8_t write16Bit(const uint8_t id, const uint8_t address, const uint16_t data);
    uint8_t write(const uint8_t id, const uint8_t address, const uint8_t* data, const int dataLength);
    uint8_t read(const uint8_t id, const uint8_t address, const uint8_t length, uint8_t* buffer);
private:
    int readStatus(uint8_t* buffer = nullptr, int size = 0);
    void sendInstruction(const uint8_t id, const uint8_t instruction, const uint8_t* parameters = nullptr, const int paramCount = 0);
    uint8_t calculateChecksum(const uint8_t* msg, const int size);
};

#endif /* HAL_DYNAMIXELCOM_H_ */
/** @} */
