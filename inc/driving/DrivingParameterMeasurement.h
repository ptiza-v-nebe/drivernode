/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Driving
 * @{
 ******************************************************************************
 */

#ifndef DRIVING_DRIVINGPARAMETERMEASUREMENT_H_
#define DRIVING_DRIVINGPARAMETERMEASUREMENT_H_

#include "util/enum_to_string.h"
#include "position/PositionManager.h"
#include "hal/Motor.h"
#include "serial/HumanCommandHandler.h"
#include "serial/MessageDispatcherFactory.h"

#define MEASUREMENT_STATE_VALS(m) \
    m(Idle) \
    m(MeasureLinearDriving) \
    m(Wait) \
    m(MeasureRotationalDriving) \
    m(Done)

SMARTENUM_DEFINE_ENUM(MeasurementState, MEASUREMENT_STATE_VALS)

#define MEASUREMENTPOINTS 200
#define MEASUREMENTPERIODTIME 20 // in ms

class DrivingParameterMeasurement : public HumanCommandHandler {
private:
	MeasurementState state;
	PositionManager& pm;
	Motor& leftMotor;
	Motor& rightMotor;

	int taskId;
	int dataIndex;
	float data[MEASUREMENTPOINTS];
public:
	DrivingParameterMeasurement(PositionManager& pm, Motor& leftMotor,
			Motor& rightMotor, MessageDispatcher& m);
	void transition();
private:
	void linearMeasurement();
	void rotationMeasurement();
protected:
	virtual void sendPrompt() override;
	virtual void processMessage(uint8_t*, int) override;
};

/**
 * Specialized MessageDispatcherFactory that sets the system up for calibration
 */
class MeasurementMessageDispatcherFactory: public MessageDispatcherFactory {
private:
    HumanSender sender; ///< the MessageSender used to send out messages
    DrivingParameterMeasurement commandHandler; ///< the CommandHandler used to handle incoming messages
public:
    MeasurementMessageDispatcherFactory(PositionManager& pm, Motor& leftMotor, Motor& rightMotor);
};

#endif /* DRIVING_DRIVINGPARAMETERMEASUREMENT_H_ */
/** @} */
