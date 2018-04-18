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
#include <vector>

class MessageDispatcher;
class MainFSMBaseState;

class Clocked;
class InitializableClocked;

class MainFSM: public ComStatusHandler {
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

    std::vector<Clocked*> alwaysClocked;
    std::vector<Clocked*> clockedInNormalOperation;
    std::vector<InitializableClocked*> needInitializing;
public:
    MainFSMContext(MessageDispatcher& dispatcher,
            std::vector<Clocked*> clockedInNormalOperation = { },
            std::vector<InitializableClocked*> needInitialising = { },
            std::vector<Clocked*> alwaysClocked = { });
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

    void startInitializing();

    bool tickInit();
    void tickNormal();

    // external interface
    void tick() override;

    void handleComEstablished() override;
    void handleComFailed() override;
    void gameTimerFinished() override;
};

#endif /* CONTROL_MAINFSM_H_ */
/** @} */
