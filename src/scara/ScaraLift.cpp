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

ScaraLift::ScaraLift(Motor& motor, Encoder& encoder) :
        motor(motor), encoder(encoder), startPosition(0), targetPosition(0), //
        initialized(false) {
}

void ScaraLift::operator ()() {
    tick();
}

void ScaraLift::tick() {
    // TODO!!
    // printf("ScaraLift::tick()...\r\n");

    int16_t currentPosition = encoder.getTick();

    int16_t speedStart = (currentPosition - startPosition) /* * factor */;
    int16_t speedEnd = (endPosition - currentPosition) /* * factor */;
}

void ScaraLift::initialize() {
    // TODO!!
    //schedule_repeating_task(*this, 100);
}

void ScaraLift::moveTo(float mm) {
    if (!initialized) {
        return;
    }
    startPosition = encoder.getTick();
    // TODO!!
    // targetPosition = CONVERSION * mm;
}

float ScaraLift::getPosition() {
    // TODO!!
    // return CONVERSION * mm
    return 0;
}
/** @} */
