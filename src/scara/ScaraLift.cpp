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
        motor(motor), encoder(encoder), targetPosition(0), initialized(false) {
}

void ScaraLift::operator ()() {
    tick();
}

void ScaraLift::tick() {
    // TODO!!
    // printf("ScaraLift::tick()...\r\n");
}

void ScaraLift::initialize() {
    // TODO!!
    //schedule_repeating_task(*this, 100);
}

void ScaraLift::moveTo(float mm) {
    if(!initialized) {
        return;
    }
    // TODO!!
    // targetPosition = CONVERSION * mm;
}

float ScaraLift::getPosition() {
    // TODO!!
    // return CONVERSION * mm
    return 0;
}
/** @} */
