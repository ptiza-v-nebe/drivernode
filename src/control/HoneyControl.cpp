/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Control
 * @{
 ******************************************************************************
 */

#include <control/HoneyControl.h>
#include <map>
#include <cmath>

static const std::map<ServoPosition, Angle> ANGLE_MAP_LEFT //
{ //
{ ServoPosition::STORED, 50_deg }, //
        { ServoPosition::PREPARE, 200_deg }, //
        { ServoPosition::PUSH, 60_deg } //
};

static const std::map<ServoPosition, Angle> ANGLE_MAP_RIGHT //
{ //
{ ServoPosition::STORED, 250_deg }, //
        { ServoPosition::PREPARE, 100_deg }, //
        { ServoPosition::PUSH, 240_deg } //
};

static constexpr double INITIALIZE_MAX_DELTA = 5; //degrees

HoneyControl::HoneyControl(DynamixelAX12A& leftServo,
        DynamixelAX12A& rightServo) :
        leftServo(leftServo), rightServo(rightServo) {
}

void HoneyControl::startInitializing() {
    leftServo.enable();
    leftServo.moveTo(ANGLE_MAP_LEFT.at(ServoPosition::STORED));

    rightServo.enable();
    rightServo.moveTo(ANGLE_MAP_RIGHT.at(ServoPosition::STORED));
}

bool HoneyControl::tickInit() {
    bool leftInitialized = fabs((leftServo.getAngle()
            - ANGLE_MAP_LEFT.at(ServoPosition::STORED)) //
    .getAngleInDegreesAround(0)) < INITIALIZE_MAX_DELTA;

    bool rightInitialized = fabs((rightServo.getAngle()
            - ANGLE_MAP_RIGHT.at(ServoPosition::STORED)) //
    .getAngleInDegreesAround(0)) < INITIALIZE_MAX_DELTA;

    return leftInitialized && rightInitialized;
}

void HoneyControl::processCommand(const ServoControlMessage& scm) {
    switch (scm.getColor()) {
        case Color::GREEN:
            rightServo.moveTo(ANGLE_MAP_RIGHT.at(scm.getServoPosition()));
            break;
        case Color::ORANGE:
            leftServo.moveTo(ANGLE_MAP_LEFT.at(scm.getServoPosition()));
            break;
        default:
            break;
    }
}
/** @} */
