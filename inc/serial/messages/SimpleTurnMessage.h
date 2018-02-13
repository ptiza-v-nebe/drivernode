/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGES_SIMPLETURNMESSAGE_H_
#define SERIAL_MESSAGES_SIMPLETURNMESSAGE_H_

#include <cstdint>

class SimpleTurnMessage {
public:
    static SimpleTurnMessage deserialize(uint8_t* msg, int size);
    static int getMessageType() {
        return 0x4;
    }
public:
    SimpleTurnMessage();

    int serialize(uint8_t* buffer, int buffersize);
    void print();
};

#endif /* SERIAL_MESSAGES_SIMPLETURNMESSAGE_H_ */
/** @} */
