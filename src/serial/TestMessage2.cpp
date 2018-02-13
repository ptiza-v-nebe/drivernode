/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup <Group Name>
 * @{
 ******************************************************************************
 */

#include <serial/TestMessage2.h>

#include <cstdio>

TestMessage2 TestMessage2::deserialize(uint8_t* msg, int size) {
    TestMessage2 obj;
    if(size == 3){
        obj.payload1 = msg[1];
        obj.payload2 = msg[2];
        obj.number = ((uint16_t)msg[1] << 8) | msg[2];
    }
    return obj;
}

TestMessage2::TestMessage2(uint8_t p1, uint8_t p2, uint16_t number) : payload1(p1), payload2(p2), number(number) {
}

TestMessage2::TestMessage2() : TestMessage2(0,0,0) {
}

void TestMessage2::printYourself() {
    printf("I am a Message 2 with payload %c%c and number %d\r\n", payload1, payload2, number);
}
/** @} */
