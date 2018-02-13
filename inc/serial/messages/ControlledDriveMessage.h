/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGES_CONTROLLEDDRIVEMESSAGE_H_
#define SERIAL_MESSAGES_CONTROLLEDDRIVEMESSAGE_H_

#include <cstdint>

class ControlledDriveMessage {
public:
    static ControlledDriveMessage deserialize(uint8_t* msg, int size);
    static int getMessageType() {
        return 0x3;
    }
public:
    ControlledDriveMessage();

    int serialize(uint8_t* buffer, int buffersize);
    void print();
};

#endif /* SERIAL_MESSAGES_CONTROLLEDDRIVEMESSAGE_H_ */
/** @} */
