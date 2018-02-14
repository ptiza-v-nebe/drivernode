/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Util
 * @{
 ******************************************************************************
 */

#include <util/conversions.h>

constexpr float degreesToRadians(float degrees) {
    return degrees * PI / 180;
}

constexpr float radiansToDegrees(float rad) {
    return rad * 180 / PI;
}
/** @} */
