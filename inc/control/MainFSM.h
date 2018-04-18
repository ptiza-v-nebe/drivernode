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

#include <serial/ComStatusHandler.h>
#include <functional>

class MessageDispatcher;
class MainFSMBaseState;

class MainFSM : public ComStatusHandler {
public:
    virtual ~MainFSM() = default;

    virtual void tick() = 0;

    //virtual void handleComEstablished() = 0; // from ComStatusHandler
    //virtual void handleComFailed() = 0; // from ComStatusHandler
    virtual void gameTimerFinished() = 0;
};

class MainFSMContext: public MainFSM {
private:
    MainFSMBaseState *currentState;
    MessageDispatcher& dispatcher;
    std::function<void()> tickAlways;
public:
    // internal / FSM
    std::function<bool()> tickInit;
    std::function<void()> tickNormal;
public:
    MainFSMContext(MessageDispatcher& dispatcher, std::function<bool()>&& init = []{return true;},
            std::function<void()>&& normal = []{}, std::function<void()>&& always = []{});
    virtual ~MainFSMContext();

    // prevent copy and move
    MainFSMContext(const MainFSMContext&) = delete;
    MainFSMContext(MainFSMContext&&) = delete;
    MainFSMContext& operator=(const MainFSMContext&) = delete;
    MainFSMContext& operator=(MainFSMContext&&) = delete;

    // internal / FSM
    void sendReadyMessage();
    void sendStartMessage();
    void startGameTimer();

    // external interface
    void tick() override;

    void handleComEstablished() override;
    void handleComFailed() override;
    void gameTimerFinished() override;
};

#endif /* CONTROL_MAINFSM_H_ */
/** @} */
