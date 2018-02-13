/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGES_GAMESTARTMESSAGE_H_
#define SERIAL_MESSAGES_GAMESTARTMESSAGE_H_

#include <cstdint>

class GameStartMessage {
public:
    static GameStartMessage deserialize(uint8_t* msg, int size);
    static int getMessageType() {
        return 0xE;
    }
public:
    GameStartMessage();

    int serialize(uint8_t* buffer, int buffersize);
    void print();
};

#endif /* SERIAL_MESSAGES_GAMESTARTMESSAGE_H_ */
/** @} */
