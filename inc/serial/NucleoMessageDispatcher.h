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

class UARTBaseState;

class NucleoMessageDispatcher: public MessageDispatcher {
private:
    // FSM Stuff
    UARTBaseState *currentState;
    int count;
    mutable bool messageSent;
public:
    NucleoMessageDispatcher(MessageSender& sender);
    ~NucleoMessageDispatcher();

    NucleoMessageDispatcher(const NucleoMessageDispatcher& other) = delete;
    NucleoMessageDispatcher(NucleoMessageDispatcher&& other) = delete;
    NucleoMessageDispatcher& operator=(const NucleoMessageDispatcher& other) = delete;
    NucleoMessageDispatcher& operator=(NucleoMessageDispatcher&& other) = delete;

    // FSM stuff
    void resetCount();
    void increaseCount();
    int getCount();

    void setMessageSent() const;
    void clearMessageSent();
    bool getMessageSent();

    void publishComError();
    void publishComEstablished();
    void publishComReset();

    void sendMessage(const Message& msg) const override;

protected:
    virtual void handleMessageProcessed(uint8_t type) const override;
    virtual void handleInvalidMessage() const override;
    virtual void handleUnknownMessage() const override;
};

#endif /* SERIAL_NUCLEOMESSAGEDISPATCHER_H_ */
/** @} */
