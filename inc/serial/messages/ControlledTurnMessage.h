/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGES_CONTROLLEDTURNMESSAGE_H_
#define SERIAL_MESSAGES_CONTROLLEDTURNMESSAGE_H_

#include <cstdint>

class ControlledTurnMessage {
public:
    static ControlledTurnMessage deserialize(uint8_t* msg, int size);
    static int getMessageType() {
        return 0x5;
    }
public:
    ControlledTurnMessage();

    int serialize(uint8_t* buffer, int buffersize);
    void print();
};

#endif /* SERIAL_MESSAGES_CONTROLLEDTURNMESSAGE_H_ */
/** @} */
