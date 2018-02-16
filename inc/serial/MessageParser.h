/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGEPARSER_H_
#define SERIAL_MESSAGEPARSER_H_

#include <cstdint>

class Message;

class MessageParser {
public:
    static int parseMessage(const uint8_t *msg, const int size, uint8_t *buffer, const int buffersize);

private:
    static int parseStopMessage(uint8_t *buffer, const int buffersize);
    static int parseDriveMessage(const uint8_t *msg, const int size, uint8_t *buffer, const int buffersize);
    static int parseTurnMessage(const uint8_t *msg, const int size, uint8_t *buffer, const int buffersize);
    static int parseSetSpeedMessage(const uint8_t *msg, const int size, uint8_t *buffer, const int buffersize);
    static int parseResetMessage(const uint8_t *msg, const int size, uint8_t *buffer, const int buffersize);

    static void print(const Message& message);
};

#endif /* SERIAL_MESSAGEPARSER_H_ */
/** @} */
