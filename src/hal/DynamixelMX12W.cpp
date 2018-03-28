/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include <hal/DynamixelMX12W.h>
#include <cmath>

static constexpr uint8_t ENABLE_ADDR = 24;
static constexpr uint8_t MOVING_SPEED_ADDR = 32;

static constexpr uint16_t MAX_SPEED = 500; //1023 according to specification, but physical max speed is around 500
static constexpr uint16_t CW_OFFSET = 1024;
static constexpr float SPEED_TO_RPM = 0.916;

DynamixelMX12W::DynamixelMX12W(uint8_t id, DynamixelCOM& com) :
        id(id), enabled(false), com(com) {
    enable();
}

void DynamixelMX12W::enable() {
    com.writeByte(id, ENABLE_ADDR, 1); // TODO: error handling
    enabled = true;
}

void DynamixelMX12W::disableAndStop() {
    com.writeByte(id, ENABLE_ADDR, 0); // TODO: error handling
    enabled = false;
}

void DynamixelMX12W::setRPM(float rpm) {
    if(!enabled) {
        return;
    }

    uint16_t speed = static_cast<uint16_t>(std::fabs(rpm) / SPEED_TO_RPM);
    if (speed > MAX_SPEED) {
        speed = MAX_SPEED;
    }

    if(rpm < 0){
        speed += CW_OFFSET;
    }

    com.writeWord(id, MOVING_SPEED_ADDR, speed); // TODO: error handling
}
/** @} */
