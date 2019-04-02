/*
 * CurrentSensor.h
 *
 *  Created on: 2 Apr 2019
 *      Author: ivan-zarevich
 */

// ///////////////////////////////////////////////////////////////////////////////
// Current sensors
// ///////////////////////////////////////////////////////////////////////////////
#ifndef HAL_CURRENTSENSORMANAGER_H_
#define HAL_CURRENTSENSORMANAGER_H_

#include "hal/CurrentSensorManager.h"
#include "hal/CurrentSensor.h"
#include "error.h"

#include "stm32l4xx.h"

/**
 * which I²C controller to use.
 * @attention when changing this also change RCC clock enable and GPIO bank and pins in
 *
 */
#define INA226_I2C I2C3

/**
 * which GPIO bank to use for INA226.
 * @attention this depends on INA226_I2C, when changing this also change RCC clock enable
 *
 */
#define INA226_GPIO GPIOC

/**
 * which pin to use for SCL signal.
 * @attention this depends on INA226_I2C
 */
#define INA226_SCL GPIO_PIN_0

/**
 * wich pn to use for SDA signal.
 * @attention depends on INA226_I2C
 */
#define INA226_SDA GPIO_PIN_1

/**
 * ID of the first INA226
 */
#define INA226_LEFT_ID 0x40<<1

/**
 * ID of the second INA226
 */
#define INA226_RIGHT_ID 0x45<<1


class CurrentSensorManager {
private:
	I2C_HandleTypeDef hi2c3;
	CurrentSensor leftCurrentSensor;
	CurrentSensor rightCurrentSensor;
	void Error_Handler(void);
	void MX_I2C3_Init(void);
	void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c);
	void initializeCurrentSensors();
	void initializeI2C();
public:
	CurrentSensorManager();
	static CurrentSensorManager& getInstance();
	CurrentSensor& getLeftCurrentSensor();
	CurrentSensor& getRightCurrentSensor();
};

#endif /* HAL_CURRENTSENSORMANAGER_H_ */
