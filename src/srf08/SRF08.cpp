/*
 * SRF08.cpp
 *
 *  Created on: 17.03.2018
 *      Author: chef
 */


#include "srf_08/srf08.h"

//initializierungsliste für i2c und address
SRF08::SRF08(I2C_HandleTypeDef *i2c, uint8_t address) : i2c(i2c), address(address)
{

}

/*
 * Function:    startRanging
 * Args:        void
 * Returns:     void
 * Description: Sends command to module to start ranging.
 */
void SRF08::startRanging() {
	uint8_t command[] = {0x00, 0x51};
	HAL_I2C_Master_Transmit(i2c, address, command, 2, 10);
	}

/*
 * Function:    rangingFinished
 * Args:        void
 * Returns:     Bool: whether ranging is finished
 * Description: Checks if the ranging process on the module is finished
 */
bool SRF08::rangingFinished() {
	uint8_t command[] = {0x00};
	uint8_t response;
	HAL_I2C_Master_Transmit(i2c, address, command, 1, 10);
	HAL_I2C_Master_Receive(i2c, address, &response, 1, 10);
	return response != 0xFF;
}

/*
 * Function:    getRange
 * Args:        voidint
 * Returns:     int range
 * Description: Range in cm. This function should only be called when ranging is finished, otherwise previous value is returned
 */
uint16_t SRF08::getRange() {

	while (!rangingFinished()){
		HAL_Delay(1);
	} //Wait until ranging is finished
	uint8_t command[]  = {0x02};           //Address of range register
	uint8_t response[] = {0x00, 0x00};
	HAL_I2C_Master_Transmit(i2c, address, command, 1, 10); //Send command
	HAL_I2C_Master_Receive(i2c, address, response, 2, 10);   //Read 16bits result
	uint16_t range = (static_cast <uint16_t>(response[0])<<8) | response[1]; //Shift two bytes into int 16 bit
	return range;                             //Return int range
}

/*
 * Function:    readLightIntensity
 * Args:        void
 * Returns:     int lightIntensity
 * Description: Reads the lightIntensity from the module
 *              The light intensity is updated if a range command is sent, so don't use
 *              this function only
 */
uint8_t SRF08::getLightIntensity() {
	uint8_t command[] = {0x01};           //Light intensity register
	uint8_t response[] = {0x00};
	HAL_I2C_Master_Transmit(i2c, address, command, 1, 1); //Send command
	HAL_I2C_Master_Receive(i2c, address, response, 2, 10);    //Read response
	return response[0];
}

/*
 * Function:    setRangeRegister
 * Args:        rangeVal
 * Returns:     void
 * Description: Sets the maximum range for which the module waits for an echo
 *              The range is ((rangeVal x 43mm) + 43mm)
 *              The max range is about six meters
 */
void SRF08::setRange(uint16_t range) {
	uint8_t rohrange = (range - 43)/43;
	while (!rangingFinished()){
			HAL_Delay(1);
		} //Wait until ranging is finished
	uint8_t command[] = {0x02, rohrange};       //Range register
	HAL_I2C_Master_Transmit(i2c, address, command, 2, 1);    //Send command
}


//Function setMaxGainRegister
void SRF08::setMaxGainRegister(uint8_t gainVal) {
	while (!rangingFinished()){
				HAL_Delay(1);
			} //Wait until ranging is finished
	uint8_t command[] = {0x01, gainVal};        //Max gain register
	HAL_I2C_Master_Transmit(i2c, address, command, 2, 1);    //Send command
}


/*
 * Function:    setAddress
 * Args:        address
 * Returns:     void
 * Description: Sets the address of the module on the I2C bus. The factory default address is 0x0E (224)
 *                  The address can have the following values:
 *                      E0 | E2 | E4 | E6 ... FC | FE
 */
void SRF08::setAddress(uint8_t newAddress) {
	//Send address change sequence
	uint8_t command[] = {0x00, 0xA0};
	HAL_I2C_Master_Transmit(i2c,  address, command, 2, 1);
	command[1] = 0xAA;
	HAL_I2C_Master_Transmit(i2c, address, command, 2, 1);
	command[1] = 0xA5;
	HAL_I2C_Master_Transmit(i2c, address, command, 2, 1);
	command[1] = newAddress;
	HAL_I2C_Master_Transmit(i2c, address, command, 2, 1);
	//Save the updated address
	address = newAddress;
}



































