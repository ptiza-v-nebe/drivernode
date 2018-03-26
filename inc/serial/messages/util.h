/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGES_UTIL_H_
#define SERIAL_MESSAGES_UTIL_H_

#include <cstdint>

void serialToSystem(const uint8_t* bytes, uint16_t& out);
int systemToSerial(const uint16_t number, uint8_t* bytes, int size);

void serialToSystem(const uint8_t* bytes, int16_t& out);
int systemToSerial(const int16_t number, uint8_t* bytes, int size);

void serialToSystem(const uint8_t* bytes, float& out);
int systemToSerial(const float number, uint8_t* bytes, int size);

#endif /* SERIAL_MESSAGES_UTIL_H_ */
/** @} */
