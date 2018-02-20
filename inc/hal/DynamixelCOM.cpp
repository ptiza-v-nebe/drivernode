/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include <hal/DynamixelCOM.h>
#include <cstring>
#include "util/util.h"
#include "config.h"

void DynamixelCOM::sendInstruction(const uint8_t id, const uint8_t instruction,
        const uint8_t* parameters, const int paramCount) {
    uint8_t length = paramCount + 2; // param count + instruction + checksum
    int msgLength = length + 1 + 1 + 2; //length + id + header (2 byte)
    uint8_t* msg = new uint8_t[msgLength];
    msg[0] = 0xFF;
    msg[1] = 0xFF;
    msg[2] = id;
    msg[3] = length;
    msg[4] = instruction;
    memcpy(msg + 5, parameters, paramCount);

    msg[msgLength - 1] = calculateChecksum(msg + 2, length + 1);

#ifdef DEBUG_DYNAMIXEL_COM
    printf("Sending ");
    printBytes(msg, msgLength);
    printf("to the Dynamixel(s).\r\n");
#endif
    uart.send(msg, msgLength);
    delete[] msg;
}

int DynamixelCOM::readStatus(uint8_t* buffer, int size) {
    uint8_t initialBuffer[4] = { 0 };
    int result = uart.receive(initialBuffer, 4);

    if ((result != 0)
            || (initialBuffer[0] != 0xFF || initialBuffer[1] != 0xFF)) {
#ifdef DEBUG_DYNAMIXEL_COM
        printf("Result is %d, got ", result);
        printBytes(initialBuffer, 4);
        printf("from the Dynamixel(s).\r\n");
#endif
        return -1;
    }

    int remainingSize = initialBuffer[3];
    int requiredBuffersize = remainingSize + 2;
    if (size < requiredBuffersize) {
        // insufficient space
#ifdef DEBUG_DYNAMIXEL_COM
        printf("Not enough space.\r\n");
#endif
        return -1;
    }

    result = uart.receive(buffer + 2, remainingSize); // receive rest of message
    buffer[0] = initialBuffer[2]; // copy id
    buffer[1] = initialBuffer[3]; // copy length

    if (result != 0) {
#ifdef DEBUG_DYNAMIXEL_COM
        printf("Receiving rest of message failed!\r\n");
#endif
        return -1;
    }

    // check for errors
    if (calculateChecksum(buffer, requiredBuffersize - 1)
            != buffer[requiredBuffersize - 1]) {
#ifdef DEBUG_DYNAMIXEL_COM
        printf("Checksum error!\r\n");
        printf("Got ");
        printBytes(buffer, requiredBuffersize);
        printf("from the Dynamixel(s).\r\n");
#endif
        return -1;
    }

    return requiredBuffersize;
}

uint8_t DynamixelCOM::calculateChecksum(const uint8_t* msg, const int size) {
    uint8_t sum = 0;
    for (int i = 0; i < size; i++) {
        sum += msg[i];
    }
    return ~sum;
}
/** @} */
