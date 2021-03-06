/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include <util/util.h>
#include <cstdio>

/**
 * Helper function to print raw bytes in hex.
 * Will print each byte using tow hex digits and seperates the bytes by a space
 *
 * @param bytes  buffer to print
 * @param length size of the buffer
 */
void printBytes(const uint8_t* bytes, const int length) {
    for(int i = 0; i < length; i++){
        printf("%02X ", bytes[i]);
    }
}

float smoothstep(float edge0, float edge1, float x) {
	x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
	return x * x * (3 - 2 * x);
}

int signum(float x) {
	if(x > 0) return 1;
	if(x < 0) return -1;
	return 0;
}
/** @} */
