/**
 ********************************************************************************
 * @brief   STM32 HAL Library for INA226 Current/Power Monitor
 * @date    Feb 2016
 * @version 1.0
 * @author  George Christidis
 ********************************************************************************
 * @details
 This library contains the necessary functions to initialize, read and
 write data to the TI INA226 Current/Power Monitor using the I2C
 protocol.
 ******************************************************************************
 */

#include <hal/CurrentSensor.h>

CurrentSensor::CurrentSensor(I2C_HandleTypeDef *I2CHandler, uint16_t DevAddress) :
		I2CHandler(I2CHandler), DevAddress(DevAddress) {
}

float CurrentSensor::getBusV() {
	return getBusVReg();
}

float CurrentSensor::getCurrent() {
	return (getCurrentReg() * INA226_CURRENTLSB_INV);
}

float CurrentSensor::getPower() {
	return (getPowerReg() * INA226_POWERLSB_INV);
}

uint8_t CurrentSensor::setConfig(uint16_t ConfigWord) {
	uint8_t SentTable[3];
	SentTable[0] = INA226_CONFIG;
	SentTable[1] = (ConfigWord & 0xFF00) >> 8;
	SentTable[2] = (ConfigWord & 0x00FF);
	return HAL_I2C_Master_Transmit(I2CHandler, DevAddress, SentTable, 3,
			INA226_I2CTIMEOUT);
}

uint16_t CurrentSensor::getConfig() {
	uint8_t SentTable[1] = { INA226_CONFIG };
	uint8_t ReceivedTable[2];
	HAL_I2C_Master_Transmit(I2CHandler, DevAddress, SentTable, 1,
			INA226_I2CTIMEOUT);
	if (HAL_I2C_Master_Receive(I2CHandler, DevAddress, ReceivedTable, 2,
			INA226_I2CTIMEOUT) != HAL_OK)
		return 0xFF;
	else
		return ((uint16_t) ReceivedTable[0] << 8 | ReceivedTable[1]);
}

uint16_t CurrentSensor::getShuntV() {
	uint8_t SentTable[1] = { INA226_SHUNTV };
	uint8_t ReceivedTable[2];
	HAL_I2C_Master_Transmit(I2CHandler, DevAddress, SentTable, 1,
			INA226_I2CTIMEOUT);
	if (HAL_I2C_Master_Receive(I2CHandler, DevAddress, ReceivedTable, 2,
			INA226_I2CTIMEOUT) != HAL_OK)
		return 0xFF;
	else
		return ((uint16_t) ReceivedTable[0] << 8 | ReceivedTable[1]);
}

uint16_t CurrentSensor::getBusVReg() {
	uint8_t SentTable[1] = { INA226_BUSV };
	uint8_t ReceivedTable[2];
	HAL_I2C_Master_Transmit(I2CHandler, DevAddress, SentTable, 1,
			INA226_I2CTIMEOUT);
	if (HAL_I2C_Master_Receive(I2CHandler, DevAddress, ReceivedTable, 2,
			INA226_I2CTIMEOUT) != HAL_OK)
		return 0xFF;
	else
		return ((uint16_t) ReceivedTable[0] << 8 | ReceivedTable[1]);
}

uint8_t CurrentSensor::setCalibrationReg(uint16_t ConfigWord) {
	uint8_t SentTable[3];
	SentTable[0] = INA226_CALIB;
	SentTable[1] = (ConfigWord & 0xFF00) >> 8;
	SentTable[2] = (ConfigWord & 0x00FF);
	return HAL_I2C_Master_Transmit(I2CHandler, DevAddress, SentTable, 3,
			INA226_I2CTIMEOUT);
}

uint16_t CurrentSensor::getCalibrationReg() {
	uint8_t SentTable[1] = { INA226_CALIB };
	uint8_t ReceivedTable[2];
	HAL_I2C_Master_Transmit(I2CHandler, DevAddress, SentTable, 1,
			INA226_I2CTIMEOUT);
	if (HAL_I2C_Master_Receive(I2CHandler, DevAddress, ReceivedTable, 2,
			INA226_I2CTIMEOUT) != HAL_OK)
		return 0xFF;
	else
		return ((uint16_t) ReceivedTable[0] << 8 | ReceivedTable[1]);
}

uint16_t CurrentSensor::getPowerReg() {
	uint8_t SentTable[1] = { INA226_POWER };
	uint8_t ReceivedTable[2];
	HAL_I2C_Master_Transmit(I2CHandler, DevAddress, SentTable, 1,
			INA226_I2CTIMEOUT);
	if (HAL_I2C_Master_Receive(I2CHandler, DevAddress, ReceivedTable, 2,
			INA226_I2CTIMEOUT) != HAL_OK)
		return 0xFF;
	else
		return ((uint16_t) ReceivedTable[0] << 8 | ReceivedTable[1]);
}

uint16_t CurrentSensor::getCurrentReg() {
	uint8_t SentTable[1] = { INA226_CURRENT };
	uint8_t ReceivedTable[2];
	HAL_I2C_Master_Transmit(I2CHandler, DevAddress, SentTable, 1,
			INA226_I2CTIMEOUT);
	if (HAL_I2C_Master_Receive(I2CHandler, DevAddress, ReceivedTable, 2,
			INA226_I2CTIMEOUT) != HAL_OK)
		return 0xFF;
	else
		return ((uint16_t) ReceivedTable[0] << 8 | ReceivedTable[1]);
}

uint16_t CurrentSensor::getManufID() {
	uint8_t SentTable[1] = { INA226_MANUF_ID };
	uint8_t ReceivedTable[2];
	HAL_I2C_Master_Transmit(I2CHandler, DevAddress, SentTable, 1,
			INA226_I2CTIMEOUT);
	if (HAL_I2C_Master_Receive(I2CHandler, DevAddress, ReceivedTable, 2,
			INA226_I2CTIMEOUT) != HAL_OK)
		return 0xFF;
	else
		return ((uint16_t) ReceivedTable[0] << 8 | ReceivedTable[1]);
}

uint16_t CurrentSensor::getDieID() {
	uint8_t SentTable[1] = { INA226_DIE_ID };
	uint8_t ReceivedTable[2];
	HAL_I2C_Master_Transmit(I2CHandler, DevAddress, SentTable, 1,
			INA226_I2CTIMEOUT);
	if (HAL_I2C_Master_Receive(I2CHandler, DevAddress, ReceivedTable, 2,
			INA226_I2CTIMEOUT) != HAL_OK)
		return 0xFF;
	else
		return ((uint16_t) ReceivedTable[0] << 8 | ReceivedTable[1]);
}

uint8_t CurrentSensor::setMaskEnable(uint16_t ConfigWord) {
	uint8_t SentTable[3];
	SentTable[0] = INA226_MASK;
	SentTable[1] = (ConfigWord & 0xFF00) >> 8;
	SentTable[2] = (ConfigWord & 0x00FF);
	return HAL_I2C_Master_Transmit(I2CHandler, DevAddress, SentTable, 3,
			INA226_I2CTIMEOUT);
}

uint16_t CurrentSensor::getMaskEnable() {
	uint8_t SentTable[1] = { INA226_MASK };
	uint8_t ReceivedTable[2];
	HAL_I2C_Master_Transmit(I2CHandler, DevAddress, SentTable, 1,
			INA226_I2CTIMEOUT);
	if (HAL_I2C_Master_Receive(I2CHandler, DevAddress, ReceivedTable, 2,
			INA226_I2CTIMEOUT) != HAL_OK)
		return 0xFF;
	else
		return ((uint16_t) ReceivedTable[0] << 8 | ReceivedTable[1]);
}

uint8_t CurrentSensor::setAlertLimit(uint16_t ConfigWord) {
	uint8_t SentTable[3];
	SentTable[0] = INA226_ALERTL;
	SentTable[1] = (ConfigWord & 0xFF00) >> 8;
	SentTable[2] = (ConfigWord & 0x00FF);
	return HAL_I2C_Master_Transmit(I2CHandler, DevAddress, SentTable, 3,
			INA226_I2CTIMEOUT);
}

uint16_t CurrentSensor::getAlertLimit() {
	uint8_t SentTable[1] = { INA226_ALERTL };
	uint8_t ReceivedTable[2];
	HAL_I2C_Master_Transmit(I2CHandler, DevAddress, SentTable, 1,
			INA226_I2CTIMEOUT);
	if (HAL_I2C_Master_Receive(I2CHandler, DevAddress, ReceivedTable, 2,
			INA226_I2CTIMEOUT) != HAL_OK)
		return 0xFF;
	else
		return ((uint16_t) ReceivedTable[0] << 8 | ReceivedTable[1]);
}


