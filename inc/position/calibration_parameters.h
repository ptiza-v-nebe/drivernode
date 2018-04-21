/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Position
 * @{
 ******************************************************************************
 */


#ifndef POSITION_CALIBRATION_PARAMETERS_H_
#define POSITION_CALIBRATION_PARAMETERS_H_

#include "config.h"

// /////////////////////////
// Parameters for big robot
// /////////////////////////
#ifdef BIG_ROBOT
static constexpr float TRACK_WIDTH_UM = 300732.06250; // calibrated 19.04.18
static constexpr float UM_PER_TICK_L = 49.27808; // calibrated 19.04.18
static constexpr float UM_PER_TICK_R = 48.64760; // calibrated 19.04.18
#endif
// /////////////////////////
// Parameters for small robot
// /////////////////////////
#ifdef SMALL_ROBOT
static constexpr float TRACK_WIDTH_UM = 180466.78125; // calibrated 21.04.18
static constexpr float UM_PER_TICK_L = 48.68075; // calibrated 21.04.18
static constexpr float UM_PER_TICK_R = 49.02682; // calibrated 21.04.18
#endif


// /////////////////////////
// General parameters
// /////////////////////////
static constexpr float EPSILON = 1e-5;

#endif /* POSITION_CALIBRATION_PARAMETERS_H_ */
/** @} */
