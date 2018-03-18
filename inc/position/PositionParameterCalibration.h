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
#include "serial/HumanCommandHandler.h"
#include "serial/MessageDispatcherFactory.h"

#define CALIBRATION_STATE_VALS(m) \
    m(Idle) \
    m(CalibrateWheels) \
    m(Wait) \
    m(CalibrateTrackWidth) \
    m(Done)

SMARTENUM_DEFINE_ENUM(CalibrationState, CALIBRATION_STATE_VALS)

class PositionParameterCalibration: public HumanCommandHandler {
private:
    CalibrationState state;
    Encoder& left;
    Encoder& right;

    float umPerTickLeft;
    float umPerTickRight;
public:
    PositionParameterCalibration(Encoder& left, Encoder& right,
            MessageDispatcher& m);
    void transition();
private:
    void calculateAndDisplayWheelParameters();
    void calculateAndDisplayTrackWidth();
protected:
    virtual void sendPrompt() override;
    virtual void processMessage(uint8_t*, int) override;
};

/**
 * Specialized MessageDispatcherFactory that sets the system up for calibration
 */
class CalibrationMessageDispatcherFactory: public MessageDispatcherFactory {
private:
    HumanSender sender; ///< the MessageSender used to send out messages
    PositionParameterCalibration commandHandler; ///< the CommandHandler used to handle incomming messages
public:
    CalibrationMessageDispatcherFactory(Encoder& left, Encoder& right);
};

#endif /* POSITION_POSITIONPARAMETERCALIBRATION_H_ */
/** @} */
