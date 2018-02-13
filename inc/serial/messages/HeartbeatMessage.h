/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGES_HEARTBEATMESSAGE_H_
#define SERIAL_MESSAGES_HEARTBEATMESSAGE_H_

#include <cstdint>

class HeartbeatMessage {
public:
    static HeartbeatMessage deserialize(uint8_t* msg, int size);
    static int getMessageType() {
        return 0x0;
    }
public:
    HeartbeatMessage();

    int serialize(uint8_t* buffer, int buffersize);
    void print();
};

#endif /* SERIAL_MESSAGES_HEARTBEATMESSAGE_H_ */
/** @} */
