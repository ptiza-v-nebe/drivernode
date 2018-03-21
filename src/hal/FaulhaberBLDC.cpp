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

static constexpr int16_t MAX_SPEED = 13000;
static constexpr int16_t MIN_SPEED = -13000;

static constexpr int ID_MAXLENGTH = 3;
static constexpr int SPEED_MAXLENGTH = 6;
static constexpr int SPEED_COMMAND_LENGTH = 1;

static constexpr int SPEED_COMMAND_BUFFERSIZE = SPEED_COMMAND_LENGTH + SPEED_MAXLENGTH + 1;
static constexpr int COMMAND_BUFFERSIZE = ID_MAXLENGTH + SPEED_COMMAND_BUFFERSIZE + 1;

FaulhaberBLDC::FaulhaberBLDC(UART_HandleTypeDef* uart, uint8_t id, bool reverseDirection) :
        uart(uart), id(id), reverseDirection(reverseDirection) {
    disableAndStop();
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
#ifdef HUMAN_MODE
        printf("WARNING, speed command too long!\r\n");
#endif
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

void FaulhaberBLDC::sendCommand(const char* command) {
    char buffer[COMMAND_BUFFERSIZE] = {0};
    int chars = snprintf(buffer, COMMAND_BUFFERSIZE, "%d%s\r", id, command);
    if(chars >= COMMAND_BUFFERSIZE) {
        // too long
#ifdef HUMAN_MODE
        printf("WARNING, message too long!\r\n");
#endif
        return;
    }

#ifdef DEBUG_MOTOR_UART
    printf("Sending command %s\n", buffer);
#endif

    uint8_t *data = reinterpret_cast<uint8_t*>(buffer);
    HAL_UART_Transmit(uart, data, chars, DEFAULT_TIMEOUT_MS);
}
/** @} */
