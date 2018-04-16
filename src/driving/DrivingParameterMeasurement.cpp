/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Position
 * @{
 ******************************************************************************
 */

#include "driving/DrivingParameterMeasurement.h"
#include "position/Position.h"
#include "position/Angle.h"
#include "serial/UARTWrapper.h"
#include "scheduler/Scheduler.h"

SMARTENUM_DEFINE_NAMES(MeasurementState, MEASUREMENT_STATE_VALS)

DrivingParameterMeasurement::DrivingParameterMeasurement(PositionManager& pm,
		Motor& leftMotor, Motor& rightMotor, MessageDispatcher& m) :
		HumanCommandHandler(m), state(MeasurementState::Idle),
		pm(pm), leftMotor(leftMotor), rightMotor(rightMotor),
		taskId(-1), dataIndex(0), data() {
	pm.reset(Position(0,0), Angle(0.0));
	leftMotor.enable();
	rightMotor.enable();


	printf("Please prepare robot for measurement of linear driving.\r\n");
	printf("Press [Enter] for start\r\n");
}

void DrivingParameterMeasurement::transition() {
	switch(state) {
	case MeasurementState::Idle:
		pm.reset(Position(0, 0), Angle(0.0));
		leftMotor.setSpeed(1500);
		rightMotor.setSpeed(1500);

		taskId = schedule_repeating_task([this]() {
			if(dataIndex > MEASUREMENTPOINTS) {
				transition();
			} else {
				linearMeasurement();
			}
		}, MEASUREMENTPERIODTIME);

		state = MeasurementState::MeasureLinearDriving;
		break;
	case MeasurementState::MeasureLinearDriving:
		leftMotor.stop();
		rightMotor.stop();
		unschedule_task(taskId);
		dataIndex = 0;

		// print data to console
		printf("Data acquired:\r\n");
		printf("n[100ms/i] x[mm] y[mm]\r\n");
		for(int i = 0; i < MEASUREMENTPOINTS/2; i += 2) {
			printf("%5d %5d %5d\r\n",i , (int)data[i], (int)data[i+1]);
		}

		printf("Please prepare robot for measurement of rotation.\r\n");
		printf("Press[Enter] for start\r\n");
		state = MeasurementState::Wait;
		break;
	case MeasurementState::Wait:
		pm.reset(Position(0, 0), Angle(0.0));
		leftMotor.setSpeed(1500);
		rightMotor.setSpeed(-1500);

		// schedule measurement task
		taskId = schedule_repeating_task([this]() {
			if(dataIndex > MEASUREMENTPOINTS) {
				transition();
			} else {
				rotationMeasurement();
			}
		}, MEASUREMENTPERIODTIME);

		state = MeasurementState::MeasureRotationalDriving;
		break;
	case MeasurementState::MeasureRotationalDriving:
		leftMotor.stop();
		rightMotor.stop();
		unschedule_task(taskId);

		// print data to console
		printf("Data acquired:\r\n");
		printf("n[100ms/i] x[mm] y[mm]\r\n");
		for(int i = 0; i < MEASUREMENTPOINTS; i++) {
			printf("%5d %5d\r\n",i , (int)data[i]);
		}

		state = MeasurementState::Done;
		break;
	default:
		printf("Please reset Nucleo to restart measurement...\r\n");
		break;
	}
}

void DrivingParameterMeasurement::linearMeasurement() {
	data[dataIndex] = pm.getPosition().x;
	dataIndex++;
	data[dataIndex] = pm.getPosition().y;
	dataIndex++;
}

void DrivingParameterMeasurement::rotationMeasurement() {
	data[dataIndex] = pm.getHeading().getAngleInDegrees();
	dataIndex++;
}

void DrivingParameterMeasurement::sendPrompt() {
	return; // do not send prompt
}

void DrivingParameterMeasurement::processMessage(uint8_t*, int) {
	transition(); // transition on every message
}


/**
 * Constructs a MeasurementMessageDispatcherFactory
 */
MeasurementMessageDispatcherFactory::MeasurementMessageDispatcherFactory(
		PositionManager& pm, Motor& leftMotor, Motor& rightMotor) :
		MessageDispatcherFactory(sender), sender(), commandHandler(pm, leftMotor,
				rightMotor, dispatcher) {
		UARTWrapper::getInstance().setReceiveHandler(&commandHandler);
}
/** @} */
