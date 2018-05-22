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
int signum(float x);


template <typename T>
T clamp(T x, T min, T max) {
	if(x < min) {
		x = min;
	} else if(x > max) {
		x = max;
	}
	return x;
}

#endif /* UTIL_UTIL_H_ */
/** @} */
