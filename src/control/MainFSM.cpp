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
#include <serial/messages/StatusMessage.h>
#include <serial/messages/GameStartMessage.h>
#include <serial/MessageDispatcher.h>

MainFSMContext::MainFSMContext(MessageDispatcher& dispatcher) :
        currentState(new Reset(*this)), dispatcher(dispatcher) {
    currentState->entry();

    /*dispatcher.registerMessageHandler<InitializeMessage>([this](InitializeMessage){ // TODO: implement init message
     currentState->initializeMessageReceived();
     });*/
}

MainFSMContext::~MainFSMContext() {
    delete(currentState);
}

bool MainFSMContext::tickInit() {
    return true; // initialization done
}

void MainFSMContext::tickNormal() {
    // TODO: tick everything that needs to be ticked
}

void MainFSMContext::tick() {
    // TODO: tick everything that ALWAYS has to be ticked
    currentState->tick();
}

void MainFSMContext::sendReadyMessage() {
    //dispatcher.sendMessage(StatusMessage(Status::READY)); // TODO: implement ready status
}

void MainFSMContext::sendStartMessage() {
    dispatcher.sendMessage(GameStartMessage());
}

void MainFSMContext::startGameTimer() {
    // TODO: start game timer
}

void MainFSMContext::comEstablished() {
    currentState->comEstablished();
}

void MainFSMContext::comFailed() {
    currentState->comFailed();
}

void MainFSMContext::gameTimerFinished() {
    currentState->gameTimerFinished();
}
/** @} */
