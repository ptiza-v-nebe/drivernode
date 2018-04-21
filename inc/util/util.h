/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Util
 * @{
 ******************************************************************************
 */

#ifndef UTIL_UTIL_H_
#define UTIL_UTIL_H_

#include <cstdint>

void printBytes(const uint8_t* bytes, const int length);
float clamp(float x, float min, float max);
float smoothstep(float edge0, float edge1, float x);

#endif /* UTIL_UTIL_H_ */
/** @} */
