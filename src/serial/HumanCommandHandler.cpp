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
 * Print a helpful message on screen :)
 */
void HumanCommandHandler::printHelpMessage() {
    printf("Available commands: \r\n");
    printf("q                 - Stop \r\n\n");

    printf("Driving: \r\n");
    printf("dp <i> <i>        - ControlledDrive to position x,y (FAST, FORWARD, HIGH accuracy)\r\n\n");

    printf("Turning: \r\n");
    printf("t <f>             - ControlledTurn to angle in degrees\r\n\n");

    printf("Scara: \r\n");
    printf("pc                - Cancel current scara action\r\n");
    printf("pp                - Send scara to park position\r\n");
    printf("pt                - Send scara to tickswitch position\r\n");
    printf("p <i> <i> <f> <i> - Action with the scara - x, y, phi (degreees!), id (of storage location)\r\n\n");

    printf("Other: \r\n");
    printf("s <i>             - SetSpeed same on both wheels\r\n");
    printf("s <i> <i>         - SetSpeed left, right\r\n");
    printf("r <i> <i> <f>     - ResetOdometry to position x,y and angle in degrees\r\n\n");
}

/**
 * Process a message that was received.
 *
 * @param msg  the message
 * @param size the size of the message
 */
void HumanCommandHandler::processMessage(uint8_t* msg, int size) {
    static uint8_t buffer[MAX_PAYLOAD + 1];
    if (size > 0) {
        int result = MessageParser::parseMessage(msg, size, buffer,
                MAX_PAYLOAD + 1);
        if (result < 0) {
            printf("That is not a valid command...\r\n");
            printHelpMessage();
        } else {
            dispatcher.processMessage(buffer, result);
        }
    }
    sendPrompt();
}
/** @} */
