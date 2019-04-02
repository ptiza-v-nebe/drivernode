/*
 * CurrentSensor.cpp
 *
 *  Created on: 2 Apr 2019
 *      Author: ivan-zarevich
 */

#include "hal/CurrentSensorManager.h"

CurrentSensorManager::CurrentSensorManager() :
		leftCurrentSensor(&hi2c3, INA226_LEFT_ID), rightCurrentSensor(&hi2c3,
				INA226_RIGHT_ID) {
	initializeI2C();
	initializeCurrentSensors();
}

void CurrentSensorManager::initializeCurrentSensors() {
	//configuring INA226, setting averaging and read speed
	leftCurrentSensor.setConfig(
			INA226_MODE_CONT_SHUNT_VOLTAGE | INA226_VSH_140uS | INA226_AVG_1024);
	rightCurrentSensor.setConfig(
			INA226_MODE_CONT_SHUNT_VOLTAGE | INA226_VSH_140uS | INA226_AVG_1024);

	//setting calibration register, max value 32768
	leftCurrentSensor.setCalibrationReg(0xAEC);
	rightCurrentSensor.setCalibrationReg(0xAEC);
}

CurrentSensorManager& CurrentSensorManager::getInstance() {
	static CurrentSensorManager instance;
	return instance;
}

/**
 * @return reference to the left current sensor
 */
CurrentSensor& CurrentSensorManager::getLeftCurrentSensor() {
	return leftCurrentSensor;
}

/**
 * @return reference to the right current sensor
 */
CurrentSensor& CurrentSensorManager::getRightCurrentSensor() {
	return rightCurrentSensor;
}

/**
 * Initializes the I²C and GPIO for the INA226
 */
void CurrentSensorManager::initializeI2C() {
	hi2c3.Instance = I2C3;
	hi2c3.Init.Timing = 0x10909CEC;
	hi2c3.Init.OwnAddress1 = 0;
	hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c3.Init.OwnAddress2 = 0;
	hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    __HAL_RCC_I2C3_CLK_ENABLE()
    ;
    TRY(HAL_I2C_Init(&hi2c3));

    GPIO_InitTypeDef i2cGPIO = {0};
    i2cGPIO.Pull = GPIO_PULLUP;
    i2cGPIO.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    i2cGPIO.Pin = INA226_SCL | INA226_SDA;
    i2cGPIO.Mode = GPIO_MODE_AF_OD;
    i2cGPIO.Alternate = GPIO_AF4_I2C3;

    __HAL_RCC_GPIOC_CLK_ENABLE()
    ;
    HAL_GPIO_Init(INA226_GPIO, &i2cGPIO);
}

