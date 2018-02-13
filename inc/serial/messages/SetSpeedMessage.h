/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGES_SETSPEEDMESSAGE_H_
#define SERIAL_MESSAGES_SETSPEEDMESSAGE_H_

#include <cstdint>

class SetSpeedMessage {
public:
    static SetSpeedMessage deserialize(uint8_t* msg, int size);
    static int getMessageType() {
        return 0xF;
    }
public:
    SetSpeedMessage();

    int serialize(uint8_t* buffer, int buffersize);
    void print();
};

#endif /* SERIAL_MESSAGES_SETSPEEDMESSAGE_H_ */
/** @} */
