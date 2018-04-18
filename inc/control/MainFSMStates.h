/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Control
 * @{
 ******************************************************************************
 */

#ifndef CONTROL_MAINFSMSTATES_H_
#define CONTROL_MAINFSMSTATES_H_

#include <new>

class MainFSMContext;

#define SUBSTATE(name, base) \
    class name : public base { \
    public: \
        name(MainFSMContext& context) : base(context) {}

#define STATE(name) SUBSTATE(name, MainFSMBaseState)

#define CHANGE_STATE(newState) \
    new(this) newState(context); entry()

// /////////////////////////////////////////////////////////////
// Base State
// /////////////////////////////////////////////////////////////
class MainFSMBaseState {
protected:
    MainFSMContext& context;
public:
    MainFSMBaseState(MainFSMContext& context) :
            context(context) {
    }
    virtual ~MainFSMBaseState() = default;

    virtual void comEstablished() {

    }
    virtual void comFailed();
    virtual void initializeMessageReceived() {
    }
    virtual void gameTimerFinished() {
    }

    virtual void entry() {
    }

    virtual void tick() {
    }
};

// /////////////////////////////////////////////////////////////
// Concrete State
// /////////////////////////////////////////////////////////////
STATE(Reset)
    void comEstablished() override;
    void comFailed() override;
};

STATE(UART_Connected)
    void comFailed() override;
    void initializeMessageReceived() override;
};

STATE(Initialize)
    void tick() override;
};

STATE(Ready)
    void tick() override;
    void entry() override;
};

STATE(Normal_Operation)
    void gameTimerFinished() override;
    void entry() override;
    void tick() override;
};

STATE(EmergencyStop)
    void entry() override;
};

SUBSTATE(Game_Finished, EmergencyStop)
};

SUBSTATE(Com_Error, EmergencyStop)
};

#endif /* CONTROL_MAINFSMSTATES_H_ */
/** @} */
