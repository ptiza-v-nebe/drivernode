/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Control
 * @{
 ******************************************************************************
 */

#include <control/MainFSMStates.h>
#include <control/MainFSM.h>
#include <hal/HALManager.h>

void MainFSMBaseState::comFailed() {
    CHANGE_STATE(Com_Error);
}
void MainFSMBaseState::comReset() {
    CHANGE_STATE(Reset);
}

void Reset::comEstablished() {
    CHANGE_STATE(UART_Connected);
}

void Reset::comFailed() {
    // do nothing (overrides default behavior)
}

void UART_Connected::entry() {
    HALManager::getInstance().getErrorLED().setOff();
}

void UART_Connected::comFailed() {
    CHANGE_STATE(Reset);
}

void UART_Connected::initializeMessageReceived() {
    CHANGE_STATE(Initialize);
}

void Initialize::entry() {
    context.startInitializing();
}

void Initialize::tick() {
    if(context.tickInit()) {
        CHANGE_STATE(Ready);
    }
}

void Ready::entry() {
    context.sendReadyMessage();
}

void Ready::tick() {
    //static InputPin& starterPin = HALManager::getInstance().getStarterPin();
    //if(starterPin.isOff()) {
	if(true){
        CHANGE_STATE(Normal_Operation);
    }
}

void Normal_Operation::entry() {
    context.sendStartMessage();
    context.startGameTimer();
}

void Normal_Operation::gameTimerFinished() {
    CHANGE_STATE(Game_Finished);
}

void Normal_Operation::tick() {
    context.tickNormal();
}

void EmergencyStop::entry() {
    HALManager::getInstance().disableAllActors();
}

void Com_Error::entry() {
    EmergencyStop::entry();
    HALManager::getInstance().getErrorLED().setOn();
}
/** @} */
