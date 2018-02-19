/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Util
 * @{
 ******************************************************************************
 */

#ifndef UTIL_CONVERSIONS_H_
#define UTIL_CONVERSIONS_H_

/**
 * The number PI
 */
constexpr float PI = 3.1415926535;

/**
 * Convert angle from degrees to radians.
 *
 * @param degrees the angle in degrees
 * @return the angle in radians
 */
constexpr float degreesToRadians(float degrees) {
    return degrees * PI / 180;
}

/**
 * Convert angel fro mradiant to degrees.
 *
 * @param rad the angle in radians
 * @return the angle in degrees
 */
constexpr float radiansToDegrees(float rad) {
    return rad * 180 / PI;
}


#endif /* UTIL_CONVERSIONS_H_ */
/** @} */
