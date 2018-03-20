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

static constexpr int16_t MAX_SPEED = 13000;
static constexpr int16_t MIN_SPEED = -13000;

static constexpr int ID_MAXLENGTH = 3;
static constexpr int SPEED_MAXLENGTH = 6;

FaulhaberBLDC::FaulhaberBLDC(UART_HandleTypeDef* uart, uint8_t id, bool reverseDirection) :
        uart(uart), id(id), reverseDirection(reverseDirection) {
    disableAndStop();
}

/*
 * @see - Actor::enable()
 */
void FaulhaberBLDC::enable() {
    sendCommand("en", 2);
    enabled = true;
}

/*
 * @see - Actor::disableAndStop()
 */
void FaulhaberBLDC::disableAndStop() {
    stop();
    sendCommand("di", 2);
    enabled = false;
}

/*
 * @see - Motor::setSpeed(uint16_t)
 */
void FaulhaberBLDC::setSpeed(uint16_t speed) {
    if (!enabled) {
        return;
    }

    if (speed > MAX_SPEED) {
        setSpeed(MAX_SPEED);
    } else {
        HAL_DAC_SetValue(&dac, dac_channel, DAC_ALIGN_12B_R, speed);
    }
}

/*
 * @see - Motor:setDirection(DriveDirection)
 */
void FaulhaberBLDC::setDirection(DriveDirection direction) {
    GPIO_PinState pinState;
    if (reverseDirection) {
        switch (direction) {
            case DriveDirection::FORWARD:
                pinState = GPIO_PIN_SET;
                break;
            default:
                pinState = GPIO_PIN_RESET;
        }
    } else {
        switch (direction) {
            case DriveDirection::BACKWARD:
                pinState = GPIO_PIN_SET;
                break;
            default:
                pinState = GPIO_PIN_RESET;
        }
    }

    HAL_GPIO_WritePin(direction_gpio, direction_pin, pinState);
}

/*
 * @see - Motor::stop()
 */
void FaulhaberBLDC::stop() {
    setSpeed(0);
}

void FaulhaberBLDC::sendCommand(const char* data, const int size) {
}
/** @} */
