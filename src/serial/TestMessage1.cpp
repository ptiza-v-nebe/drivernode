/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#include <serial/TestMessage1.h>
#include <cstdio>

TestMessage1 TestMessage1::deserialize(uint8_t*, int) {
    return TestMessage1();
}

void TestMessage1::printYourself() {
    printf("I am Message 1\r\n");
}
/** @} */
