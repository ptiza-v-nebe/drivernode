/**
 ********************************************************************************
 * @brief   STM32 HAL Library for INA226 Current/Power Monitor
 * @date    Feb 2016
 * @version 1.0
 * @author  George Christidis
 * @modified ivan-zarevich
 ********************************************************************************
 * @details
 This library contains the necessary functions to initialize, read and
 write data to the TI INA226 Current/Power Monitor using the I2C
 protocol.
 ******************************************************************************
 */

#ifndef HAL_CURRENTSENSOR_H_
#define HAL_CURRENTSENSOR_H_

#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_i2c.h"

#define INA226_CALIB_VAL		1024
#define INA226_CURRENTLSB		0.5F // mA/bit
#define INA226_CURRENTLSB_INV	1/INA226_CURRENTLSB // bit/mA
#define INA226_POWERLSB_INV		1/(INA226_CURRENTLSB*25) // bit/mW
#define INA226_I2CTIMEOUT		10

#define INA226_CONFIG		0x00 // Configuration Register (R/W)
#define INA226_SHUNTV		0x01 // Shunt Voltage (R)
#define INA226_BUSV			0x02 // Bus Voltage (R)
#define INA226_POWER		0x03 // Power (R)
#define INA226_CURRENT		0x04 // Current (R)
#define INA226_CALIB		0x05 // Calibration (R/W)
#define INA226_MASK			0x06 // Mask/Enable (R/W)
#define INA226_ALERTL		0x07 // Alert Limit (R/W)
#define INA226_MANUF_ID		0xFE // Manufacturer ID (R)
#define INA226_DIE_ID		0xFF // Die ID (R)

#define INA226_MODE_POWER_DOWN			(0<<0) // Power-Down
#define INA226_MODE_TRIG_SHUNT_VOLTAGE	(1<<0) // Shunt Voltage, Triggered
#define INA226_MODE_TRIG_BUS_VOLTAGE	(2<<0) // Bus Voltage, Triggered
#define INA226_MODE_TRIG_SHUNT_AND_BUS	(3<<0) // Shunt and Bus, Triggered
#define INA226_MODE_POWER_DOWN2			(4<<0) // Power-Down
#define INA226_MODE_CONT_SHUNT_VOLTAGE	(5<<0) // Shunt Voltage, Continuous
#define INA226_MODE_CONT_BUS_VOLTAGE	(6<<0) // Bus Voltage, Continuous
#define INA226_MODE_CONT_SHUNT_AND_BUS	(7<<0) // Shunt and Bus, Continuous

// Shunt Voltage Conversion Time
#define INA226_VSH_140uS			(0<<3)
#define INA226_VSH_204uS			(1<<3)
#define INA226_VSH_332uS			(2<<3)
#define INA226_VSH_588uS			(3<<3)
#define INA226_VSH_1100uS			(4<<3)
#define INA226_VSH_2116uS			(5<<3)
#define INA226_VSH_4156uS			(6<<3)
#define INA226_VSH_8244uS			(7<<3)

// Bus Voltage Conversion Time (VBUS CT Bit Settings[6-8])
#define INA226_VBUS_140uS			(0<<6)
#define INA226_VBUS_204uS			(1<<6)
#define INA226_VBUS_332uS			(2<<6)
#define INA226_VBUS_588uS			(3<<6)
#define INA226_VBUS_1100uS			(4<<6)
#define INA226_VBUS_2116uS			(5<<6)
#define INA226_VBUS_4156uS			(6<<6)
#define INA226_VBUS_8244uS			(7<<6)

// Averaging Mode (AVG Bit Settings[9-11])
#define INA226_AVG_1				(0<<9)
#define INA226_AVG_4				(1<<9)
#define INA226_AVG_16				(2<<9)
#define INA226_AVG_64				(3<<9)
#define INA226_AVG_128				(4<<9)
#define INA226_AVG_256				(5<<9)
#define INA226_AVG_512				(6<<9)
#define INA226_AVG_1024				(7<<9) 

// Reset Bit (RST bit [15])
#define INA226_RESET_ACTIVE			(1<<15)
#define INA226_RESET_INACTIVE		(0<<15)

// Mask/Enable Register
#define INA226_MER_SOL				(1<<15) // Shunt Voltage Over-Voltage
#define INA226_MER_SUL				(1<<14) // Shunt Voltage Under-Voltage
#define INA226_MER_BOL				(1<<13) // Bus Voltagee Over-Voltage
#define INA226_MER_BUL				(1<<12) // Bus Voltage Under-Voltage
#define INA226_MER_POL				(1<<11) // Power Over-Limit
#define INA226_MER_CNVR				(1<<10) // Conversion Ready
#define INA226_MER_AFF				(1<<4)  // Alert Function Flag
#define INA226_MER_CVRF				(1<<3)  // Conversion Ready Flag
#define INA226_MER_OVF				(1<<2)  // Math Overflow Flag
#define INA226_MER_APOL				(1<<1)  // Alert Polarity Bit
#define INA226_MER_LEN				(1<<0)  // Alert Latch Enable

#define INA226_MANUF_ID_DEFAULT	0x5449
#define INA226_DIE_ID_DEFAULT		0x2260

class CurrentSensor {
private:
	I2C_HandleTypeDef *I2CHandler;
	uint16_t DevAddress;
public:
	CurrentSensor(I2C_HandleTypeDef *I2CHandler, uint16_t DevAddress);

	float getBusV();
	float getCurrent();
	float getPower();

	uint8_t setConfig(uint16_t ConfigWord);
	uint16_t getConfig();
	uint16_t getShuntV();
	uint16_t getBusVReg();
	uint16_t getPowerReg();
	uint8_t setCalibrationReg(uint16_t ConfigWord);
	uint16_t getCalibrationReg();
	uint16_t getCurrentReg();
	uint16_t getManufID();
	uint16_t getDieID();
	uint8_t setMaskEnable(uint16_t ConfigWord);
	uint16_t getMaskEnable();
	uint8_t setAlertLimit(uint16_t ConfigWord);
	uint16_t getAlertLimit();
};

#endif //INA226_H
