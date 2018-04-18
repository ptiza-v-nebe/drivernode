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

MainFSMContext::MainFSMContext(MessageDispatcher& dispatcher,
        std::vector<Clocked*> clockedInNormalOperation,
        std::vector<InitializableClocked*> needInitialising,
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

bool MainFSMContext::tickInit() {
    // TODO: tick init
    return true;
}

void MainFSMContext::tickNormal() {
    // TODO: tick normal
}

void MainFSMContext::tick() {
    // TODO: tick always
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
