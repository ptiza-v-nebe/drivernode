/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_TESTMESSAGE2_H_
#define SERIAL_TESTMESSAGE2_H_

#include <cstdint>

class TestMessage2 {
private:
    uint8_t payload1;
    uint8_t payload2;
    uint16_t number;
private:
    TestMessage2();
public:
    static TestMessage2 deserialize(uint8_t* msg, int size);
    static int getMessageType() {
        return 0x6;
    }

    TestMessage2(uint8_t p1, uint8_t p2, uint16_t number);

    void printYourself();
};

#endif /* SERIAL_TESTMESSAGE2_H_ */
/** @} */
