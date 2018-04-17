/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Serial
 * @{
 ******************************************************************************
 */

#ifndef INC_SERIAL_UARTSTATES_H_
#define INC_SERIAL_UARTSTATES_H_

#include <new>

class NucleoMessageDispatcher;
class HeartbeatMessage;

#define SUBSTATE(name, base) \
    class name : public base { \
    public: \
        name(NucleoMessageDispatcher& context) : base(context) {}

#define STATE(name) SUBSTATE(name, UARTBaseState)

#define CHANGE_STATE(newState) \
    new(this) newState(context)

// /////////////////////////////////////////////////////////////
// Base State
// /////////////////////////////////////////////////////////////
class UARTBaseState {
protected:
    NucleoMessageDispatcher& context;
public:
    UARTBaseState(NucleoMessageDispatcher& context) : context(context) {}
    virtual ~UARTBaseState() = default;

    virtual void heartbeatTick() {}
    virtual void heartbeatReceived(const HeartbeatMessage& msg);
    virtual void heartbeatTimeout() {}
    virtual void messageReceived() {}
};


// /////////////////////////////////////////////////////////////
// Concrete State
// /////////////////////////////////////////////////////////////
STATE(Init)
    void heartbeatTick() override;
    void heartbeatReceived(const HeartbeatMessage& msg) override;
};

STATE(WaitForConnection)
    void heartbeatTick() override;
    void heartbeatReceived(const HeartbeatMessage& msg) override;
    void heartbeatTimeout() override;
};

STATE(ReadyToSend)
    void heartbeatTick() override;
};

SUBSTATE(Connected, ReadyToSend)
    void heartbeatTimeout() override;
    void messageReceived() override;
};

SUBSTATE(MessageReceived, ReadyToSend)
    void heartbeatTimeout() override;
};


#endif /* INC_SERIAL_UARTSTATES_H_ */
/** @} */
