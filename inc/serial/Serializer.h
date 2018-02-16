/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_SERIALIZER_H_
#define SERIAL_SERIALIZER_H_

#include "serial/messages/Message.h"

class Serializer {
public:
    static int serialize(const Message& message, uint8_t* buffer, const int buffersize);
};

#endif /* SERIAL_SERIALIZER_H_ */
/** @} */
