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
	printf("Please prepare robot for measurement of linear driving.\r\n");
	printf("Press [Enter] for start");
}

void DrivingParameterMeasurement::transition() {
	switch(state) {
	case MeasurementState::Idle:
		pm.reset(Position(0.0, 0.0), Angle(0.0));
		leftMotor.setSpeed(2000);
		rightMotor.setSpeed(2000);
		taskId = schedule_repeating_task([this]() {
			linearMeasurement();
		}, 100);
		state = MeasurementState::MeasureLinearDriving;
		break;
	case MeasurementState::MeasureLinearDriving:
		if(dataIndex > MEASUREMENTPOINTS) {
			unschedule_task(taskId);
			printf("Data acquired:\r\n");
			printf("n[100ms/i] x[mm] y[mm]\r\n");
			for(int i = 0; i < MEASUREMENTPOINTS/2; i += 2) {
				printf("%5d %5d %5d\r\n",i , (int)data[i], (int)data[i+1]);
			}
			printf("Please prepare robot for measurement of rotation.\r\n");
			printf("Press[Enter] for start");
			state = MeasurementState::Wait;
		}
		break;
	case MeasurementState::Wait:
		pm.reset(Position(0.0, 0.0), Angle(0.0));
		leftMotor.setSpeed(2000);
		rightMotor.setSpeed(-2000);
		taskId = schedule_repeating_task([this]() {
			rotationMeasurement();
		}, 100);
		state = MeasurementState::MeasureRotationalDriving;
		break;
	case MeasurementState::MeasureRotationalDriving:
		if(dataIndex > MEASUREMENTPOINTS) {
			unschedule_task(taskId);
			state = MeasurementState::Done;
		}
		break;
	case MeasurementState::Done:
		break;
	default:
		printf("Please reset Nucleo to restart measurement...\r\n");
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
