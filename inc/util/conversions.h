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

constexpr float PI = 3.1415926535;

constexpr float degreesToRadians(float degrees) {
    return degrees * PI / 180;
}

constexpr float radiansToDegrees(float rad) {
    return rad * 180 / PI;
}


#endif /* UTIL_CONVERSIONS_H_ */
/** @} */
