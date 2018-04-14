/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_NUCLEOMESSAGEDISPATCHER_H_
#define SERIAL_NUCLEOMESSAGEDISPATCHER_H_

#include "serial/MessageDispatcher.h"

class NucleoMessageDispatcher: public MessageDispatcher {
public:
    NucleoMessageDispatcher(MessageSender& sender);

protected:
    virtual void handleMessageProcessed() const override;
    virtual void handleInvalidMessage() const override;
    virtual void handleUnknownMessage() const override;
};

#endif /* SERIAL_NUCLEOMESSAGEDISPATCHER_H_ */
/** @} */
