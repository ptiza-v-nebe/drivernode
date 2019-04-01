/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include <hal/HALManager.h>
#include "hal/util.h"
#include "error.h"

// ///////////////////////////////////////////////////////////////////////////////
// Hardware configuration
// ///////////////////////////////////////////////////////////////////////////////

// ///////////////////////////////////////////////////////////////////////////////
// Current sensors
// ///////////////////////////////////////////////////////////////////////////////

/**
 * which I²C controller to use.
 * @attention when changing this also change RCC clock enable and GPIO bank and pins in
 *            HALManager::initializeI2C
 */
#define INA226_I2C I2C3

/**
 * which GPIO bank to use for INA226.
 * @attention this depends on INA226_I2C, when changing this also change RCC clock enable
 *            in HALManager::initializeI2C
 */
#define INA226_GPIO GPIOB

/**
 * which pin to use for SCL signal.
 * @attention this depends on INA226_I2C
 */
static constexpr uint16_t INA226_SCL = GPIO_PIN_13;

/**
 * wich pn to use for SDA signal.
 * @attention depends on INA226_I2C
 */
static constexpr uint16_t INA226_SDA = GPIO_PIN_14;

/**
 * ID of the first INA226
 */
static constexpr uint8_t INA226_1_ID = 0xE2;

/**
 * ID of the second INA226
 */
static constexpr uint8_t INA226_2_ID = 0xEE;

// ///////////////////////////////////////////////////////////////////////////////
// Driving Motors
// ///////////////////////////////////////////////////////////////////////////////

#define IN2B_Pin GPIO_PIN_15
#define IN2B_GPIO_Port GPIOB
#define IN2A_Pin GPIO_PIN_13
#define IN2A_GPIO_Port GPIOB

/**
 * the timer used for left motor
 * @attention when changing this, also change RCC clock enable as well as GPIO
 *            and pin in HALManager::initializeMotors
 */
#define LEFT_TIMER TIM1

/**
 * the timer channel used for left motor
 */
static constexpr auto LEFT_CHANNEL = TIM_CHANNEL_1;

/**
 * the timer used for right motor
 * @attention when changing this, also change RCC clock enable as well as GPIO
 *            and pin in HALManager::initializeMotors
 */
#define RIGHT_TIMER TIM1

/**
 * timer channel used for right motor
 */
static constexpr auto RIGHT_CHANNEL = TIM_CHANNEL_3;

/**
 * should the left motor be inverted
 */
static constexpr bool LEFT_MOTOR_INVERT = true;

/**
 * should the right motor be inverted
 */
static constexpr bool RIGHT_MOTOR_INVERT = false;

// ///////////////////////////////////////////////////////////////////////////////
// Constructor
// ///////////////////////////////////////////////////////////////////////////////
/**
 * Constructs and initializes the HALManager.
 */
HALManager::HALManager() :
		statusLED(GPIOB, GPIO_PIN_6, GPIO_PIN_SET), //
		errorLED(GPIOB, GPIO_PIN_5, GPIO_PIN_SET),
		leftMotorPWM(LEFT_TIMER, LEFT_CHANNEL), //
		rightMotorPWM(RIGHT_TIMER, RIGHT_CHANNEL), //
		leftMotor(leftMotorPWM, { GPIOB, GPIO_PIN_12, GPIO_PIN_SET },
				{ GPIOA, GPIO_PIN_2 , GPIO_PIN_SET}, false), //
		rightMotor(rightMotorPWM, { GPIOB, GPIO_PIN_14, GPIO_PIN_SET },
				{ GPIOC, GPIO_PIN_7 , GPIO_PIN_SET}, false)
{
	initializeHal();
	//statusLED.enable();
	//errorLED.enable();
	//statusLED.setOn();
	//disableAllActors();
}

// ///////////////////////////////////////////////////////////////////////////////
// Common Code
// ///////////////////////////////////////////////////////////////////////////////
/**
 * @return the Singleton instance of this class.
 */
HALManager& HALManager::getInstance() {
	static HALManager instance;
	return instance;
}

/**
 * initializes the necessary low level components
 */
void HALManager::initializeHal() {
	initializeMotorsPWM();
}

/**
 * @return reference to the encoder manager
 */
EncoderManager& HALManager::getEncoderManager() {
	return encoderManager;
}


 PWM& HALManager::getLeftMotorPWM(){
 return leftMotorPWM;
 }

 PWM& HALManager::getRightMotorPWM(){
 return rightMotorPWM;
 }


/**
 * @return reference to the left driving motor
 */


 Motor& HALManager::getLeftMotor() {
 return leftMotor;
 }

/**
 * @return reference to the right driving motor
 */

 Motor& HALManager::getRightMotor() {
 return rightMotor;
 }


/**
 * @return reference to the status LED
 */
OutputPin& HALManager::getStatusLED() {
	return statusLED;
}

/**
 * @return reference to the error LED
 */
OutputPin& HALManager::getErrorLED() {
	return errorLED;
}

/**
 * @return reference to the left encoder
 */
Encoder& HALManager::getLeftEncoder() {
	return encoderManager.getLeftEncoder();
}

/**
 * @return reference to the right encoder
 */
Encoder& HALManager::getRightEncoder() {
	return encoderManager.getRightEncoder();
}

/**
 * disables all actors
 */
void HALManager::disableAllActors() {

}

/**
 * initializes the timers and gpios for the pwms for the motors
 */

void HALManager::initializeMotorsPWM() {
	__HAL_RCC_TIM1_CLK_ENABLE(); //this should be before MX_TIM1_Init
	MX_TIM1_Init(htim1);


/*	TIM_OC_InitTypeDef sConfigOC = { 0 };

	//Configure Motor A
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 1600; //speed of motors (range 0-3200)
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1);

	//configure Motor B
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 1600; //speed of motors (range 0-3200)
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3);
	*/

	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);

	//leftMotorPWM.disable();
	//rightMotorPWM.disable();
}

void HALManager::MX_TIM1_Init(TIM_HandleTypeDef& htim1) {

	/* USER CODE BEGIN TIM1_Init 0 */

	/* USER CODE END TIM1_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = { 0 };

	/* USER CODE BEGIN TIM1_Init 1 */

	/* USER CODE END TIM1_Init 1 */
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 0;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 3200;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1)
			!= HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3)
			!= HAL_OK) {
		Error_Handler();
	}
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.BreakFilter = 0;
	sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
	sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
	sBreakDeadTimeConfig.Break2Filter = 0;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM1_Init 2 */

	/* USER CODE END TIM1_Init 2 */
	HAL_TIM_MspPostInit(&htim1);
}

void HALManager::HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	if (htim->Instance == TIM1) {
		/* USER CODE BEGIN TIM1_MspPostInit 0 */

		/* USER CODE END TIM1_MspPostInit 0 */

		__HAL_RCC_GPIOB_CLK_ENABLE()
		;
		/**TIM1 GPIO Configuration
		 PB13     ------> TIM1_CH1N
		 PB15     ------> TIM1_CH3N
		 */
		GPIO_InitStruct.Pin = IN2A_Pin | IN2B_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* USER CODE BEGIN TIM1_MspPostInit 1 */

		/* USER CODE END TIM1_MspPostInit 1 */
	}
}

void HALManager::Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

	/* USER CODE END Error_Handler_Debug */
}
