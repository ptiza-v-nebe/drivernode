/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_TESTMESSAGE1_H_
#define SERIAL_TESTMESSAGE1_H_

#include <cstdint>

class TestMessage1 {
public:
    static TestMessage1 deserialize(uint8_t* msg, int size);
    static int getMessageType(){
        return 0x7;
    }

    void printYourself();
};

#endif /* SERIAL_TESTMESSAGE1_H_ */
/** @} */
