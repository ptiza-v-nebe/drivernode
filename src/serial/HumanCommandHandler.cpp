/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#include <serial/HumanCommandHandler.h>
#include "scheduler/Scheduler.h"
#include "serial/UARTWrapper.h"
#include "serial/MessageParser.h"
#include <cstdio>

/**
 * Constructs a HumanCommanHandler.
 *
 * @param dispatcher reference to the dispatcher that processes the message
 */
HumanCommandHandler::HumanCommandHandler(MessageDispatcher& dispatcher) :
        dispatcher(dispatcher) {
    sendPrompt();
}

/*
 * @see - UARTReceiveHandler::processByte()
 */
void HumanCommandHandler::processByte(uint8_t byte) {
    if (byte == 0x7F || byte == '\b') {
        // delete or backspace
        if (count > 0) {
            count--;

            schedule_task_from_isr([]() {
                uint8_t buffer[] = {'\b', ' ', '\b'};
                UARTWrapper::getInstance().send(buffer, 3);
            });
        }
    } else {
        if (byte == '\r' || count == MSG_BUFFER_SIZE - 1) {
            uint8_t* msg = buffers[currentBuffer];
            msg[count] = 0; // insert \0 terminator
            int size = count;

            schedule_task_from_isr([this, msg, size]() {
                sendLineBreak();
                processMessage(msg, size);
            });
            currentBuffer = (currentBuffer + 1) % MSG_BUFFER_COUNT;
            count = 0;

        } else if (isValidCharacter(byte)) {
            buffers[currentBuffer][count] = byte;
            count++;

            schedule_task_from_isr([byte]() {
                UARTWrapper::getInstance().send(&byte, 1);
            });
        }
    }
}

/**
 * Checks if the enterred character is valid.
 * Valid characters are a-z, 0-9, SPACE, . and -.
 * @attention Uppercase letters will be converted to lower case
 *
 * @param byte the character to check
 * @return true if the character is valid
 */
bool HumanCommandHandler::isValidCharacter(uint8_t& byte) {
    bool uppercase = (byte >= 'A' && byte <= 'Z');
    if (uppercase) {
        byte = byte + ('a' - 'A'); // convert to lowercase
        return true;
    }

    bool lowercase = (byte >= 'a' && byte <= 'z');
    bool number = (byte >= '0' && byte <= '9');
    bool space = (byte == ' ');
    bool point = (byte == '.');
    bool minus = (byte == '-');

    return lowercase || number || space || point || minus;
}

/**
 * Send a linebreak (CRLF) using printf
 */
void HumanCommandHandler::sendLineBreak() {
    printf("\r\n");
}

/**
 * Send a prompt using printf.
 */
void HumanCommandHandler::sendPrompt() {
    printf("RCHH-Nucleo $ ");
    fflush(stdout);
}

/**
 * Process a message that was received.
 *
 * @param msg  the message
 * @param size the size of the message
 */
void HumanCommandHandler::processMessage(uint8_t* msg, int size) {
    static uint8_t buffer[MAX_PAYLOAD + 1];
    if(size > 0) {
        int result = MessageParser::parseMessage(msg, size, buffer,
                MAX_PAYLOAD + 1);
        if (result < 0) {
            printf("That is not a valid command...\r\n");
        } else {
            dispatcher.processMessage(buffer, result);
        }
    }
    sendPrompt();
}
/** @} */
