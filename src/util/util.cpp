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

float clamp(float x, float min, float max) {
	if(x < min) {
		x = min;
	} else if(x > max) {
		x = max;
	}
	return x;
}

/** @} */
