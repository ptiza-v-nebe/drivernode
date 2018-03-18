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
static constexpr float TRACK_WIDTH_UM = 22*10*1000;
static constexpr float UM_PER_TICK_L = 94;
static constexpr float UM_PER_TICK_R = 94;
#endif

// /////////////////////////
// Parameters for small robot
// /////////////////////////
#ifdef SMALL_ROBOT
#endif


// /////////////////////////
// General parameters
// /////////////////////////
static constexpr float EPSILON = 1e-5;

#endif /* POSITION_CALIBRATION_PARAMETERS_H_ */
/** @} */
