/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGES_STOPMESSAGE_H_
#define SERIAL_MESSAGES_STOPMESSAGE_H_

#include <cstdint>

class StopMessage {
public:
    static StopMessage deserialize(uint8_t* msg, int size);
    static int getMessageType() {
        return 0x1;
    }
public:
    StopMessage();

    int serialize(uint8_t* buffer, int buffersize);
    void print();
};

#endif /* SERIAL_MESSAGES_STOPMESSAGE_H_ */
/** @} */
