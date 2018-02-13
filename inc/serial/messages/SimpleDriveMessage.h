/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGES_SIMPLEDRIVEMESSAGE_H_
#define SERIAL_MESSAGES_SIMPLEDRIVEMESSAGE_H_

#include <cstdint>

class SimpleDriveMessage {
public:
    static SimpleDriveMessage deserialize(uint8_t* msg, int size);
    static int getMessageType() {
        return 0x2;
    }
public:
    SimpleDriveMessage();

    int serialize(uint8_t* buffer, int buffersize);
    void print();
};

#endif /* SERIAL_MESSAGES_SIMPLEDRIVEMESSAGE_H_ */
/** @} */
