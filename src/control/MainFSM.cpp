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
        std::function<bool()>&& init, std::function<void()>&& normal,
        std::function<void()>&& always) :
        currentState(new Reset(*this)), dispatcher(dispatcher), //
        tickAlways(std::move(always)), tickInit(std::move(init)), //
        tickNormal(std::move(normal)) {
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

void MainFSMContext::tick() {
    tickAlways();
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
