/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_HUMANCOMMANDHANDLER_H_
#define SERIAL_HUMANCOMMANDHANDLER_H_

#include <serial/UARTReceiveHandler.h>

class HumanCommandHandler: public UARTReceiveHandler {
public:
    HumanCommandHandler();
    virtual ~HumanCommandHandler();
};

#endif /* SERIAL_HUMANCOMMANDHANDLER_H_ */
/** @} */
