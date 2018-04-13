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

static constexpr int16_t DELTA_MIN = 50;
static constexpr int16_t DELTA_MAX = 500;
static constexpr int16_t ACCURACY = 20;
static constexpr float DELTA_TO_SPEED = 8.0f;

static constexpr int16_t MAX_POSITION = 7100;
static constexpr int16_t MIN_POSITION = 0;

ScaraLift::ScaraLift(Motor& motor, Encoder& encoder) :
        motor(motor), encoder(encoder), startPosition(0), targetPosition(0), //
        initialized(false) {
}

void ScaraLift::tick() {
    // TODO!!
    // read input
    int16_t currentPosition = encoder.getTick(); //between 0 and 7100

    // calculate
    int16_t deltaEnd = std::abs(targetPosition - currentPosition);
    int16_t speed;
    if (deltaEnd < ACCURACY) {
        speed = 0;
    } else {
        int16_t deltaStart = std::abs(currentPosition - startPosition);
        int16_t delta = std::min( std::max(std::min(deltaStart, deltaEnd), DELTA_MIN), DELTA_MAX);

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
    encoder.reset();
    motor.enable();
    motor.stop();
    initialized = true;
    schedule_repeating_task([this](){
       tick();
    }, 100);
}

void ScaraLift::moveTo(float mm) {
    if (!initialized) {
        return;
    }
    startPosition = encoder.getTick();
    // TODO!!
    targetPosition = /*CONVERSION * mm*/clamp(static_cast<int16_t>(mm));
}

float ScaraLift::getPosition() {
    // TODO!!
    // return CONVERSION * mm
    return 0;
}

int16_t ScaraLift::clamp(int16_t value) {
    return std::min(std::max(value, MIN_POSITION), MAX_POSITION);
}
/** @} */
