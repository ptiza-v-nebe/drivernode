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

/**
 * Constructs a MessageDispatcherFactory.
 *
 * @param sender the MessageSender to be used for sending
 */
MessageDispatcherFactory::MessageDispatcherFactory(MessageSender& sender) :
        dispatcher(sender) {
}

/**
 * @return the MessageDispatcher
 */
MessageDispatcher& MessageDispatcherFactory::getMessageDispatcher() {
    return dispatcher;
}

/**
 * Constructs a HumanMessageDispatcherFactory.
 */
HumanMessageDispatcherFactory::HumanMessageDispatcherFactory() :
        MessageDispatcherFactory(sender), sender(), commandHandler(dispatcher) {
    UARTWrapper::getInstance().setReceiveHandler(&commandHandler);
}

/**
 * Constructs an ODROIDMessageDispatcherFactory.
 */
ODROIDMessageDispatcherFactory::ODROIDMessageDispatcherFactory() :
        MessageDispatcherFactory(UARTWrapper::getInstance()), commandHandler(
                dispatcher) {
    UARTWrapper::getInstance().setReceiveHandler(&commandHandler);
}

/** @} */
