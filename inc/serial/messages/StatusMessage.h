/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGES_STATUSMESSAGE_H_
#define SERIAL_MESSAGES_STATUSMESSAGE_H_

#include <cstdint>

class StatusMessage {
public:
    static StatusMessage deserialize(uint8_t* msg, int size);
    static int getMessageType() {
        return 0x7;
    }
public:
    StatusMessage();

    int serialize(uint8_t* buffer, int buffersize);
    void print();
};

#endif /* SERIAL_MESSAGES_STATUSMESSAGE_H_ */
/** @} */
