/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include <hal/FaulhaberBLDC.h>
#include "hal/util.h"
#include "config.h"
#include "constants.h"
#include "error.h"
#include "util/util.h"

/**
 * the maximum speed of the motor
 */
static constexpr int16_t MAX_SPEED = 13000;

/**
 * the minimum speed of the motor
 */
static constexpr int16_t MIN_SPEED = -13000;

/**
 * timeout for the UART transmission
 */
static constexpr int UART_TIMEOUT_MS = 5;

/**
 * maximum number of characters for ID (max ID is 254)
 */
static constexpr int ID_MAXLENGTH = 3;

/**
 * maximum number of characters for speed (max speed is -13.000)
 */
static constexpr int SPEED_MAXLENGTH = 6;

/**
 * length of the command (command is v)
 */
static constexpr int SPEED_COMMAND_LENGTH = 1;

/**
 * required buffer size for the speed command.
 * command + speed + \0
 */
static constexpr int SPEED_COMMAND_BUFFERSIZE = SPEED_COMMAND_LENGTH + SPEED_MAXLENGTH + 1;

/**
 * required buffer size for entire command
 * id + speed command + carriage return
 */
static constexpr int COMMAND_BUFFERSIZE = ID_MAXLENGTH + SPEED_COMMAND_BUFFERSIZE + 1;

/**
 * Constructs an abstraction for a FaulhaberBLDC
 * @attention FaulhaberBLDC is DISABLED by default
 *
 * @param uart             reference to the UART to be used
 * @param id               the ID of the motor
 * @param reverseDirection should the direction be reversed
 */
FaulhaberBLDC::FaulhaberBLDC(UART_HandleTypeDef* uart, uint8_t id, bool reverseDirection) :
        enabled(false), uart(uart), id(id), reverseDirection(reverseDirection) {
}

/*
 * @see - Actor::enable()
 */
void FaulhaberBLDC::enable() {
    sendCommand("en");
    enabled = true;
}

/*
 * @see - Actor::disableAndStop()
 */
void FaulhaberBLDC::disableAndStop() {
    stop();
    sendCommand("di");
    enabled = false;
}

/*
 * @see - Motor::setSpeed(uint16_t)
 */
void FaulhaberBLDC::setSpeed(int16_t speed) {
    if (!enabled) {
        return;
    }

    if(reverseDirection) {
        speed = -speed;
    }

    if (speed > MAX_SPEED) {
        speed = MAX_SPEED;
    }
    if(speed < MIN_SPEED) {
        speed = MIN_SPEED;
    }

    char buffer[SPEED_COMMAND_BUFFERSIZE] = {0};
    int chars = snprintf(buffer, SPEED_COMMAND_BUFFERSIZE, "v%d", speed);
    if(chars >= SPEED_COMMAND_BUFFERSIZE) {
        // too many characters
        ERROR("Speed comand to long!");
        return;
    }
    sendCommand(buffer);
}

/*
 * @see - Motor::stop()
 */
void FaulhaberBLDC::stop() {
    setSpeed(0);
}

/**
 * Sends a command to the motor.
 * Will prefix the id and append \r
 *
 * @param command the command to be sent
 */
void FaulhaberBLDC::sendCommand(const char* command) {
    char buffer[COMMAND_BUFFERSIZE] = {0};
    int chars = snprintf(buffer, COMMAND_BUFFERSIZE, "%d%s\r", id, command);
    if(chars >= COMMAND_BUFFERSIZE) {
        // too long
        ERROR("Message too long!");
        return;
    }

    uint8_t *data = reinterpret_cast<uint8_t*>(buffer);
#ifdef DEBUG_MOTOR_UART
    printf("Sending command (%d) ", chars);
    printBytes(data, chars);
    printf(" = %s\n", buffer);
#endif

    TRY(HAL_UART_Transmit(uart, data, chars, UART_TIMEOUT_MS));
}
/** @} */
