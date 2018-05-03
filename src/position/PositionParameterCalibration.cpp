/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Position
 * @{
 ******************************************************************************
 */

#include "config.h"
#include <position/PositionParameterCalibration.h>
#include "position/Angle.h" // for PI
#include "serial/UARTWrapper.h"
#include <cmath>

static constexpr int METERS_TRAVELLED = 1;
static constexpr int ROTATIONS = 5;

SMARTENUM_DEFINE_NAMES(CalibrationState, CALIBRATION_STATE_VALS)

PositionParameterCalibration::PositionParameterCalibration(Encoder& left,
        Encoder& right, MessageDispatcher& m) :
        HumanCommandHandler(m), state(CalibrationState::Idle), left(left), right(
                right), umPerTickLeft(0), umPerTickRight(0) {
    printf("Please prepare robot for calibration of the wheels.\r\n");
    printf("Press [Enter] to start");
}

void PositionParameterCalibration::transition() {
    switch (state) {
        case CalibrationState::Idle:
            left.reset();
            right.reset();
            printf("Please move robot straight forward for %dm.\r\n",
                    METERS_TRAVELLED);
            printf("Press [Enter] when finished");
            state = CalibrationState::CalibrateWheels;
            break;
        case CalibrationState::CalibrateWheels:
            calculateAndDisplayWheelParameters();
            printf(
                    "Please prepare robot for calibration of the track width.\r\n");
            printf("Press [Enter] to start");
            state = CalibrationState::Wait;
            break;
        case CalibrationState::Wait:
            left.reset();
            right.reset();
            printf("Please turn robot %d times.\r\n", ROTATIONS);
            printf("Press [Enter] when finished");
            state = CalibrationState::CalibrateTrackWidth;
            break;
        case CalibrationState::CalibrateTrackWidth:
            calculateAndDisplayTrackWidth();
            state = CalibrationState::Done;
            break;
        default:
            printf("Please reset Nucleo to restart calibration...\r\n");
    }
}

void PositionParameterCalibration::calculateAndDisplayWheelParameters() {
    int ticksLeft = left.getTick();
    int ticksRight = right.getTick();

    const float DISTANCE_UM = METERS_TRAVELLED * 1000 /*mm/m*/* 1000 /*um/mm*/;

    umPerTickLeft = DISTANCE_UM / ticksLeft;
    umPerTickRight = DISTANCE_UM / ticksRight;

    printf("\n");
    printf("UM_PER_TICK_L = %.5f;\r\n", umPerTickLeft);
    printf("UM_PER_TICK_R = %.5f;\r\n", umPerTickRight);
    printf("\n");
}

void PositionParameterCalibration::calculateAndDisplayTrackWidth() {
    int ticksLeft = left.getTick();
    int ticksRight = right.getTick();

    float umLeft = ticksLeft * umPerTickLeft;
    float umRight = ticksRight * umPerTickRight;

    float avgDist = (std::abs(umLeft) + std::abs(umRight)) / 2;
    float avgCircumference = avgDist / ROTATIONS;

    float trackWidth = avgCircumference / PI;

    printf("\n");
    printf("TRACK_WIDTH_UM = %.5f;\r\n", trackWidth);
    printf("\n");
}

void PositionParameterCalibration::sendPrompt() {
    return; // do not send prompt
}

void PositionParameterCalibration::processMessage(uint8_t*, int) {
    transition(); // transition on every message
}

/**
 * Constructs a CalibrationMessageDispatcherFactory.
 */
CalibrationMessageDispatcherFactory::CalibrationMessageDispatcherFactory(
        Encoder& left, Encoder& right) :
        MessageDispatcherFactory(sender), sender(), commandHandler(left, right,
                dispatcher) {
    UARTWrapper::getInstance().setReceiveHandler(&commandHandler);
}
/** @} */
