/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#include <serial/MessageDispatcherFactory.h>
#include "serial/UARTWrapper.h"

MessageDispatcherFactory::MessageDispatcherFactory(MessageSender& sender) : dispatcher(sender){
}

MessageDispatcher& MessageDispatcherFactory::getMessageDispatcher() {
    return dispatcher;
}

HumanMessageDispatcherFactory::HumanMessageDispatcherFactory() :
        MessageDispatcherFactory(sender), commandHandler(dispatcher) {
    UARTWrapper::getInstance().setReceiveHandler(&commandHandler);
}

ODROIDMessageDispatcherFactory::ODROIDMessageDispatcherFactory() :
        MessageDispatcherFactory(UARTWrapper::getInstance()), commandHandler(dispatcher) {
    UARTWrapper::getInstance().setReceiveHandler(&commandHandler);
}

/** @} */
