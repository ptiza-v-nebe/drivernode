/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Control
 * @{
 ******************************************************************************
 */

#include <control/MainFSM.h>
#include <control/MainFSMStates.h>
#include <serial/messages/InitializeMessage.h>
#include <serial/messages/GameStartMessage.h>
#include <serial/messages/StatusMessage.h>
#include <serial/MessageDispatcher.h>
#include <algorithm>

MainFSMContext::MainFSMContext(MessageDispatcher& dispatcher,
        std::vector<Clocked*> clockedInNormalOperation,
        std::vector<ClockedInitializable*> needInitialising,
        std::vector<Clocked*> alwaysClocked) :
        currentState(new Reset(*this)), dispatcher(dispatcher), alwaysClocked(
                alwaysClocked), clockedInNormalOperation(
                clockedInNormalOperation), needInitializing(needInitialising) {
    currentState->entry();

    dispatcher.registerMessageHandler<InitializeMessage>(
            [this](InitializeMessage) {
                currentState->initializeMessageReceived();
            });

    dispatcher.setStatusHandler(this);
}

MainFSMContext::~MainFSMContext() {
    delete (currentState);
}

void MainFSMContext::startInitializing() {
    std::for_each(needInitializing.begin(), needInitializing.end(),
            [](ClockedInitializable *element) {
                element->startInitializing();
            });
}

bool MainFSMContext::tickInit() {
    bool result = true;
    std::for_each(needInitializing.begin(), needInitializing.end(),
            [&result](ClockedInitializable *element) {
                result = result && element->tickInit();
            });
    return result;
}

void MainFSMContext::tickNormal() {
    std::for_each(clockedInNormalOperation.begin(),
            clockedInNormalOperation.end(), [](Clocked *element) {
                element->tick();
            });
}

void MainFSMContext::tick() {
    std::for_each(alwaysClocked.begin(), alwaysClocked.end(),
            [](Clocked *element) {
                element->tick();
            });
    currentState->tick();
}

void MainFSMContext::sendReadyMessage() {
    dispatcher.sendMessage(StatusMessage(Status::READY));
}

void MainFSMContext::sendStartMessage() {
    dispatcher.sendMessage(GameStartMessage());
}

void MainFSMContext::startGameTimer() {
    // TODO: start game timer
}

void MainFSMContext::handleComEstablished() {
    currentState->comEstablished();
}

void MainFSMContext::handleComFailed() {
    currentState->comFailed();
}

void MainFSMContext::gameTimerFinished() {
    currentState->gameTimerFinished();
}
/** @} */
