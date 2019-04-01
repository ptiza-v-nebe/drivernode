/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 ******************************************************************************
 */

#include "config.h"

#include "hal/util.h"
#include "scheduler/Scheduler.h"
#include "serial/MessageDispatcherFactory.h"
#include "position/PositionParameterCalibration.h"
#include "hal/HALManager.h"
#include "position/PositionManager.h"
#include "driving/DriverFSM.h"
#include "driving/DrivingParameterMeasurement.h"
#include "constants.h"

#include "serial/messages/all.h"
#include "util/util.h"
#include "control/MainFSM.h"

#include "hal/PWM.h"
#include "hal/OutputPin.h"
#include "hal/InputPin.h"
#include "error.h"
#include "serial/messages/version.h"
#include "serial/messages/all.h"

#include <functional>

#include <stm32l4xx.h>


int main(void) {
	setupHardware();

    HALManager& hal = HALManager::getInstance();

    hal.getLeftMotor().enable();
    hal.getRightMotor().enable();
    hal.getLeftMotor().setSpeed(6000);
    hal.getRightMotor().setSpeed(-1000);


    for (;;){
    	//printf("test\n\r");
    	hal.getErrorLED().toggle();
    	hal.getStatusLED().toggle();
    	HAL_Delay(500);
    }

}
