/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup <Group Name>
 * @{
 ******************************************************************************
 */

#ifndef CONTROL_MAINFSM_H_
#define CONTROL_MAINFSM_H_

class MessageDispatcher;
class MainFSMBaseState;

class MainFSM {
public:
    virtual ~MainFSM() = default;

    virtual void tick() = 0;

    virtual void comEstablished() = 0;
    virtual void comFailed() = 0;
    virtual void gameTimerFinished() = 0;
};

class MainFSMContext: public MainFSM {
private:
    MainFSMBaseState *currentState;
    MessageDispatcher& dispatcher;
public:
    MainFSMContext(MessageDispatcher& dispatcher);
    virtual ~MainFSMContext();

    // prevent copy and move
    MainFSMContext(const MainFSMContext&) = delete;
    MainFSMContext(MainFSMContext&&) = delete;
    MainFSMContext& operator=(const MainFSMContext&) = delete;
    MainFSMContext& operator=(MainFSMContext&&) = delete;

    // internal / FSM
    bool tickInit();
    void tickNormal();

    void sendReadyMessage();
    void sendStartMessage();
    void startGameTimer();

    // external interface
    void tick() override;

    void comEstablished() override;
    void comFailed() override;
    void gameTimerFinished() override;
};

#endif /* CONTROL_MAINFSM_H_ */
/** @} */
