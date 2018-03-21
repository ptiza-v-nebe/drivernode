/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#include <serial/messages/util.h>

/**
 * Converts serial bytes to a 16 bit unsigned.
 *
 * @param bytes the **2**(!) bytes that were received
 * @param out   the corresponding 16bit unsigned integer
 */
void serialToSystem(const uint8_t* bytes, uint16_t& out) {
    out = (((uint16_t) bytes[0]) << 8) | bytes[1];
}

/**
 * Converts 16bit unsigned to serial representation
 *
 * @param number the number to convert
 * @param bytes  buffer for the bytes (2 required)
 * @param size   available size
 *
 * @return the number of bytes used
 * @retval -1 insufficient size
 */
int systemToSerial(const uint16_t number, uint8_t* bytes, int size) {
    if (size < 2) {
        return -1;
    }

    bytes[0] = (uint8_t) (number >> 8);
    bytes[1] = (uint8_t) number;
    return 2;
}

/**
 * Converts serial bytes to a 16 bit unsigned.
 *
 * @param bytes the **2**(!) bytes that were received
 * @param out   the corresponding 16bit signed integer
 */
void serialToSystem(const uint8_t* bytes, int16_t& out) {
    out = (((uint16_t) bytes[0]) << 8) | bytes[1];
}

/**
 * Converts 16bit signed to serial representation
 *
 * @param number the number to convert
 * @param bytes  buffer for the bytes (2 required)
 * @param size   available size
 *
 * @return the number of bytes used
 * @retval -1 insufficient size
 */
int systemToSerial(const int16_t number, uint8_t* bytes, int size) {
    if (size < 2) {
        return -1;
    }

    bytes[0] = (uint8_t) (number >> 8);
    bytes[1] = (uint8_t) number;
    return 2;
}

/**
 * Converts serial bytes to a 32 bit floating point number.
 *
 * @param bytes the **4**(!) bytes that were received
 * @param out   the corresponding 32 bit floating point number
 */
void serialToSystem(const uint8_t* bytes, float& out) {
    uint8_t* temp = reinterpret_cast<uint8_t*>(&out);
    temp[0] = bytes[3];
    temp[1] = bytes[2];
    temp[2] = bytes[1];
    temp[3] = bytes[0];
}

/**
 * Converts 32 bit floating point to serial representation
 *
 * @param number the number to convert
 * @param bytes  buffer for the bytes (4 required)
 * @param size   available size
 *
 * @return the number of bytes used
 * @retval -1 insufficient size
 */
int systemToSerial(const float number, uint8_t* bytes, int size) {
    if (size < 4) {
        return -1;
    }

    const uint8_t* temp = reinterpret_cast<const uint8_t*>(&number);
    bytes[0] = temp[3];
    bytes[1] = temp[2];
    bytes[2] = temp[1];
    bytes[3] = temp[0];
    return 4;
}

/** @} */
