/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGEDISPATCHERFACTORY_H_
#define SERIAL_MESSAGEDISPATCHERFACTORY_H_

#include "serial/messages/MessageDispatcher.h"
#include "serial/HumanSender.h"
#include "serial/HumanCommandHandler.h"
#include "serial/ODROIDCommandHandler.h"

/**
 * Base class for creating the message dispatcher and associated system.
 */
class MessageDispatcherFactory {
protected:
    MessageDispatcher dispatcher; ///< the message dispatcher
public:
    MessageDispatcherFactory(MessageSender& sender);
    virtual ~MessageDispatcherFactory() = default;
    MessageDispatcher& getMessageDispatcher();
};

/**
 * Specialized MessageDispatcherFactory that sets the system up for interacting with a human on a terminal.
 */
class HumanMessageDispatcherFactory : public MessageDispatcherFactory {
private:
    HumanSender sender; ///< the MessageSender used to send out messages
    HumanCommandHandler commandHandler; ///< the CommandHandler used to handle incomming messages
public:
    HumanMessageDispatcherFactory();
};

/**
 * Specialized MessageDispatcherFactory that sets the system up for interacting with the main program on an ODROID
 */
class ODROIDMessageDispatcherFactory : public MessageDispatcherFactory {
private:
    ODROIDCommandHandler commandHandler; ///< the CommandHandler used to handle incomming messages
public:
    ODROIDMessageDispatcherFactory();
};

#endif /* SERIAL_MESSAGEDISPATCHERFACTORY_H_ */
/** @} */
