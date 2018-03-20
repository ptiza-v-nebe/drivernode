/*
 * srf08.h
 *
 *  Created on: 17.03.2018
 *      Author: chef
 */

#ifndef SRF08_H_
#define SRF08_H_

#include "stm32l4xx.h"
#include <cstdint>

class SRF08 {
public:
	/**
	 * Create a SRF08 object connected to the specified I2C pins and address
	 *
	 * @param SDA I2C SDA pin to connect to
	 * @param SCL I2C SCL pin to connect to
	 * @param i2cAddress Address of WSRF08 on I2C bus
	 *///Konstruktor
	SRF08(I2C_HandleTypeDef *i2c, uint8_t address);

	/**
	 * Send the "Start ranging in cm" command via I2C
	 */
	void startRanging();

	/**
	 * Checks if the module has finished ranging
	 *
	 * @param returns Boolean stating module is finished or not
	 */
	bool rangingFinished();
	/**
	 * Gets the measured range from the module
	 *
	 * @param returns Integer range in centimetre
	 */
	uint16_t getRange();

	/**
	 * Gets the measured light intensity from the module
	 *
	 * @param returns A normalised number 0-255 representing dark to light
	 */
	uint8_t getLightIntensity();

	/**
	 * Sets the range register of the SRF08 for faster ranging.
	 *
	 * The max range is ((rangeVal x 43mm) + 43mm). The sensors maximum range
	 *  is about six metres
	 *
	 * @param rangeVal The value written to the range register of the SRF08
	 */
	void setRange(uint16_t range);

	/**
	 * Sets the max gain register of the SRF08.
	 *
	 * @param gainVal The value written to the max gain register of the SRF08
	 */
	void setMaxGainRegister(uint8_t gain);

	/**
	 * Changes the I2C address of the SRF08.
	 *
	 * The factory default address is 0x0E (224)
	 *  The address can have the following values:
	 *   E0 | E2 | E4 | E6 ... FC | FE
	 *
	 * @param i2cAddress The new I2C address for the SRF08.
	 */
	void setAddress(uint8_t address);

private:
	I2C_HandleTypeDef *i2c;
	uint8_t address;
};


#endif /* SRF08_H_ */
