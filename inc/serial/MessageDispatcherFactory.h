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

#include "serial/MessageDispatcher.h"
#include "serial/HumanSender.h"
#include "serial/HumanCommandHandler.h"
#include "serial/ODROIDCommandHandler.h"

class MessageDispatcherFactory {
protected:
    MessageDispatcher dispatcher;
public:
    MessageDispatcherFactory(MessageSender& sender);
    virtual ~MessageDispatcherFactory() = default;
    MessageDispatcher& getMessageDispatcher();
};

class HumanMessageDispatcherFactory : public MessageDispatcherFactory {
private:
    HumanSender sender;
    HumanCommandHandler commandHandler;
public:
    HumanMessageDispatcherFactory();
};

class ODROIDMessageDispatcherFactory : public MessageDispatcherFactory {
private:
    ODROIDCommandHandler commandHandler;
public:
    ODROIDMessageDispatcherFactory();
};

#endif /* SERIAL_MESSAGEDISPATCHERFACTORY_H_ */
/** @} */
