/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_HUMANCOMMANDHANDLER_H_
#define SERIAL_HUMANCOMMANDHANDLER_H_

#include <serial/UARTReceiveHandler.h>
#include "serial/MessageDispatcher.h"

constexpr int MSG_BUFFER_SIZE = 50;
constexpr int MSG_BUFFER_COUNT = 2;

class HumanCommandHandler: public UARTReceiveHandler {
private:
    uint8_t buffers[MSG_BUFFER_COUNT][MSG_BUFFER_SIZE];
    int currentBuffer = 0;
    int count = 0;
    MessageDispatcher& dispatcher;
public:
    HumanCommandHandler(MessageDispatcher& dispatcher);

    void processByte(uint8_t byte) override;

private:
    bool isValidCharacter(uint8_t& byte);
    void sendLineBreak();
    void sendPrompt();

    void processMessage(uint8_t *msg, int size);
};

#endif /* SERIAL_HUMANCOMMANDHANDLER_H_ */
/** @} */
