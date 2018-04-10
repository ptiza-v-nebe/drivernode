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

/**
 * custom error flag to indicate an IO error
 */
static constexpr uint8_t COM_FAIL = (1 << 7);

/**
 * ping command
 */
static constexpr uint8_t DYNAMIXEL_PING = 0x01;

/**
 * read register command
 */
static constexpr uint8_t DYNAMIXEL_READ = 0x02;

/**
 * write register command
 */
static constexpr uint8_t DYNAMIXEL_WRITE = 0x03;


/**
 * Constructs a DynamixelCOM
 */
DynamixelCOM::DynamixelCOM() : uart() {
}

/**
 * Ping the dynamixel.
 *
 * @param id the id to ping
 *
 * @return status code
 * @retval 0 everything ok
 */
uint8_t DynamixelCOM::ping(const uint8_t id) {
    sendInstruction(id, DYNAMIXEL_PING);

    uint8_t buffer[4];
    int result = readStatus(buffer, 4);
    if (result < 0) {
        return COM_FAIL;
    }
    return buffer[2]; // return error code
}

/**
 * Writes data to the dynamixel
 *
 * @param id         the id of the dynamixel
 * @param address    the address to write to
 * @param data       pointer to the data to write
 * @param dataLength size of data to write
 *
 * @return status code
 * @retval 0 everything ok
 */
uint8_t DynamixelCOM::write(const uint8_t id, const uint8_t address,
        const uint8_t* data, const int dataLength) {
    // create params
    int paramCount = dataLength + 1;
    uint8_t *params = new uint8_t[paramCount];
    params[0] = address;
    memcpy(params + 1, data, dataLength);

    sendInstruction(id, DYNAMIXEL_WRITE, params, paramCount);

    uint8_t buffer[4];
    int result = readStatus(buffer, 4);
    if (result < 0) {
        return COM_FAIL;
    }

    delete[] params;
    return buffer[2]; // return error code
}

/**
 * Read data from the dynamixel
 *
 * @param id      the id of the dynamixel
 * @param address the address to read from
 * @param length  the amount of data to read (MUST be less than or equal to the size of the buffer)
 * @param buffer  the buffer to store the read data in
 *
 * @return status code
 * @retval 0 everything ok
 */
uint8_t DynamixelCOM::read(const uint8_t id, const uint8_t address,
        const uint8_t length, uint8_t* buffer) {
    uint8_t params[] = { address, length };
    sendInstruction(id, DYNAMIXEL_READ, params, 2);

    int recvLength = length + 4;
    uint8_t *recvBuf = new uint8_t[recvLength];
    int result = readStatus(recvBuf, recvLength);
    if (result < 0) {
        return COM_FAIL;
    }

    memcpy(buffer, recvBuf + 3, length);
    uint8_t errorCode = recvBuf[2];

    delete[] recvBuf;
    return errorCode;
}

/**
 * Send an instruction packet to the dynamixel
 *
 * @param id          the id of the dynamixel
 * @param instruction the instruction to send
 * @param parameters  pointer to the parameter bytes
 * @param paramCount  number of parameter bytes
 */
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

/**
 * Read the status packet
 *
 * @param buffer the buffer to read into
 * @param size   the size of the buffer
 *
 * @return number of bytes read
 * @retval -1 there was an error
 */
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

/**
 * Calculate the checksum of a packet
 *
 * @param msg  pointer to the message
 * @param size size of the message
 *
 * @return the checksum
 */
uint8_t DynamixelCOM::calculateChecksum(const uint8_t* msg, const int size) {
    uint8_t sum = 0;
    for (int i = 0; i < size; i++) {
        sum += msg[i];
    }
    return ~sum;
}

/**
 * Helper function to write one byte.
 *
 * @param id      the id of the dynamixel
 * @param address the address to write to
 * @param data    the byte to write
 *
 * @return status code
 * @retval 0 everything ok
 */
uint8_t DynamixelCOM::writeByte(const uint8_t id, const uint8_t address,
        const uint8_t data) {
    return write(id, address, &data, 1);
}

/**
 * Helper function to write 16 bit (word).
 *
 * @param id      the id of the dynamixel
 * @param address the address to write to
 * @param data    the word to write
 *
 *
 * @return status code
 * @retval 0 everything ok
 */
uint8_t DynamixelCOM::writeWord(const uint8_t id, const uint8_t address,
        const uint16_t data) {
    uint8_t dataArr[] = { static_cast<uint8_t>(data & 0xFF),
            static_cast<uint8_t>((data >> 8) & 0xFF) };
    return write(id, address, dataArr, 2);
}

/**
 * Helper function to read one byte.
 *
 * @param id      the id of the dynamixel
 * @param address the address to read from
 * @param data    buffer for the result
 *
 * @return status code
 * @retval 0 everything ok
 */
uint8_t DynamixelCOM::readByte(const uint8_t id, const uint8_t address,
        uint8_t& value) {
    return read(id, address, 1, &value);
}

/**
 * Helper function to read 16 bit (word).
 *
 * @param id      the id of the dynamixel
 * @param address the address to read from
 * @param data    buffer for the result
 *
 * @return status code
 * @retval 0 everything ok
 */
uint8_t DynamixelCOM::readWord(const uint8_t id, const uint8_t address,
        uint16_t& value) {
    uint8_t data[2] = { 0 };
    uint8_t retVal = read(id, address, 2, data);
    value = data[0] | (static_cast<uint16_t>(data[1]) << 8);
    return retVal;
}
/** @} */
