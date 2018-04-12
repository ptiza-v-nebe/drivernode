/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Scara
 * @{
 ******************************************************************************
 */

#include <scara/ScaraLift.h>
#include <scheduler/Scheduler.h>

static constexpr int16_t DELTA_MIN = 10;
static constexpr int16_t DELTA_MAX = 100;
static constexpr int16_t ACCURACY = 10;
static constexpr float DELTA_TO_SPEED = 3.0f;

ScaraLift::ScaraLift(Motor& motor, Encoder& encoder) :
        motor(motor), encoder(encoder), startPosition(0), targetPosition(0), //
        initialized(false) {
}

void ScaraLift::operator ()() {
    tick();
}

void ScaraLift::tick() {
    // TODO!!
    // read input
    int16_t currentPosition = encoder.getTick();

    // calculate
    int16_t deltaEnd = std::abs(targetPosition - currentPosition);
    int16_t speed;
    if (deltaEnd < ACCURACY) {
        speed = 0;
    } else {
        int16_t deltaStart = std::abs(currentPosition - startPosition);
        int16_t delta = std::min(
                std::max(std::min(deltaStart, deltaEnd), DELTA_MIN), DELTA_MAX);

        if (currentPosition > targetPosition) {
            delta = -delta;
        }

        speed = static_cast<int16_t>(delta * DELTA_TO_SPEED);
    }

    // set output
    motor.setSpeed(speed);
}

void ScaraLift::initialize() {
    // TODO!!
    initialized = true;
    schedule_repeating_task(*this, 100);
}

void ScaraLift::moveTo(float mm) {
    if (!initialized) {
        return;
    }
    startPosition = encoder.getTick();
    // TODO!!
    targetPosition = /*CONVERSION * mm*/static_cast<int16_t>(mm);
}

float ScaraLift::getPosition() {
    // TODO!!
    // return CONVERSION * mm
    return 0;
}
/** @} */
