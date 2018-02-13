/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGES_RESETODOMETRYMESSAGE_H_
#define SERIAL_MESSAGES_RESETODOMETRYMESSAGE_H_

#include <cstdint>

class ResetOdometryMessage {
public:
    static ResetOdometryMessage deserialize(uint8_t* msg, int size);
    static int getMessageType() {
        return 0x8;
    }
public:
    ResetOdometryMessage();

    int serialize(uint8_t* buffer, int buffersize);
    void print();
};

#endif /* SERIAL_MESSAGES_RESETODOMETRYMESSAGE_H_ */
/** @} */
