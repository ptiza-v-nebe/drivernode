/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Control
 * @{
 ******************************************************************************
 */

#include <control/StartPinInitializer.h>

StartPinInitializer::StartPinInitializer(const InputPin& startPin) :
        startPin(startPin) {
}

void StartPinInitializer::startInitializing() {
}

bool StartPinInitializer::tickInit() {
    return startPin.isOn();
}
/** @} */
