/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Position
 * @{
 ******************************************************************************
 */

#ifndef POSITION_POSITIONPARAMETERCALIBRATION_H_
#define POSITION_POSITIONPARAMETERCALIBRATION_H_

#include "util/enum_to_string.h"
#include "hal/Encoder.h"

#define CALIBRATION_STATE_VALS(m) \
    m(Idle) \
    m(CalibrateWheels) \
    m(Wait) \
    m(CalibrateTrackWidth) \
    m(Done)

SMARTENUM_DEFINE_ENUM(CalibrationState, CALIBRATION_STATE_VALS)


class PositionParameterCalibration {
private:
    CalibrationState state;
    Encoder& left;
    Encoder& right;

    float umPerTickLeft;
    float umPerTickRight;
public:
    PositionParameterCalibration(Encoder& left, Encoder& right);
    void transition();
private:
    void calculateAndDisplayWheelParameters();
    void calculateAndDisplayTrackWidth();
};

#endif /* POSITION_POSITIONPARAMETERCALIBRATION_H_ */
/** @} */
