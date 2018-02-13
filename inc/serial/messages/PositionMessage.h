/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGES_POSITIONMESSAGE_H_
#define SERIAL_MESSAGES_POSITIONMESSAGE_H_

#include <cstdint>

class PositionMessage {
public:
    static PositionMessage deserialize(uint8_t* msg, int size);
    static int getMessageType() {
        return 0x6;
    }
public:
    PositionMessage();

    int serialize(uint8_t* buffer, int buffersize);
    void print();
};

#endif /* SERIAL_MESSAGES_POSITIONMESSAGE_H_ */
/** @} */
