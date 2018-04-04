/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * @author Cagri Erdogan
 * @author Moritz Höwer
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include "hal/SRF08.h"
#include "constants.h"
#include "error.h"

/**
 * Constructs an abstraction for a SRF08 Ultrasonic sensor
 *
 * @param i2c     the handle for controlling the I²C
 * @param address the address of the sensor
 */
SRF08::SRF08(I2C_HandleTypeDef *i2c, uint8_t address) :
        i2c(i2c), address(address) {

}

/**
 * Send the command to start measuring
 */
void SRF08::startRanging() {
    uint8_t command[] = { 0x00, 0x51 }; // Write command 0x51 to command register (0)
    TRY(HAL_I2C_Master_Transmit(i2c, address, command, 2, DEFAULT_TIMEOUT_MS));
}

/**
 * Checks if the ranging has finished.
 * @attention Currently this function is broken!!
 *
 * @return true if ranging has finished and the sensor
 */
bool SRF08::rangingFinished() {
    uint8_t command[] = { 0x00 };
    uint8_t response = 0xFF;

    // read version id (register 0)
    TRY(HAL_I2C_Master_Transmit(i2c, address, command, 1, DEFAULT_TIMEOUT_MS));
    TRY(HAL_I2C_Master_Receive(i2c, address, &response, 1, DEFAULT_TIMEOUT_MS));

    // if sensor is busy, result should be 0xFF
    // so result != 0xFF --> sensor is ready
    return response != 0xFF;
}

/**
 * Read the measured range from the sensor.
 *
 * @return the measured range in cm
 */
uint16_t SRF08::getRange() {
    uint8_t command[] = { 0x02 };
    uint8_t response[] = { 0xFF, 0xFF };

    // read range (register 2 + 3)
    TRY(HAL_I2C_Master_Transmit(i2c, address, command, 1, DEFAULT_TIMEOUT_MS)); //Send register
    TRY(HAL_I2C_Master_Receive(i2c, address, response, 2, DEFAULT_TIMEOUT_MS)); //Read 16bits result

    //Shift two bytes into 16 bit
    uint16_t range = (static_cast<uint16_t>(response[0]) << 8) | response[1];
    return range;
}

/**
 * Reads the measured light intensity from the sensor.
 * @return the measured light intensity
 */
uint8_t SRF08::getLightIntensity() {
    uint8_t command[] = { 0x01 };
    uint8_t lightValue = 0;

    // read light value (register 1)
    TRY(HAL_I2C_Master_Transmit(i2c, address, command, 1, DEFAULT_TIMEOUT_MS)); //Send command
    TRY(HAL_I2C_Master_Receive(i2c, address, &lightValue, 1, DEFAULT_TIMEOUT_MS)); //Read response

    return lightValue;
}

/**
 * Set the max range of the sensor.
 *
 * @param range the maximum range that can be measured in mm
 */
void SRF08::setMaxRange(uint16_t range) {
    uint8_t rohrange = (range - 43) / 43; // formula from datasheet
    uint8_t command[] = { 0x02, rohrange };

    // write range (register 2)
    TRY(HAL_I2C_Master_Transmit(i2c, address, command, 2, DEFAULT_TIMEOUT_MS)); //Send command
}

/**
 * Set the value of the max gain register.
 *
 * @param gainVal the value to set (refer to datasheet for explanation)
 */
void SRF08::setMaxGainRegister(uint8_t gainVal) {
    uint8_t command[] = { 0x01, gainVal };

    // write gain register (register 1)
    TRY(HAL_I2C_Master_Transmit(i2c, address, command, 2, DEFAULT_TIMEOUT_MS)); //Send command
}

/**
 * Change the address of the SRF08
 *
 * @param newAddress the new address
 */
void SRF08::setAddress(uint8_t newAddress) {
    //Send address change sequence
    uint8_t command[] = { 0x00, 0xA0 };
    TRY(HAL_I2C_Master_Transmit(i2c, address, command, 2, DEFAULT_TIMEOUT_MS));
    command[1] = 0xAA;
    TRY(HAL_I2C_Master_Transmit(i2c, address, command, 2, DEFAULT_TIMEOUT_MS));
    command[1] = 0xA5;
    TRY(HAL_I2C_Master_Transmit(i2c, address, command, 2, DEFAULT_TIMEOUT_MS));
    command[1] = newAddress;
    TRY(HAL_I2C_Master_Transmit(i2c, address, command, 2, DEFAULT_TIMEOUT_MS));
    //Save the updated address
    address = newAddress;
}
/** @} */
