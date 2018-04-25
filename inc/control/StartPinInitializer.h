/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Control
 * @{
 ******************************************************************************
 */

#ifndef CONTROL_STARTPININITIALIZER_H_
#define CONTROL_STARTPININITIALIZER_H_

#include "ClockedInitializable.h"

#include <hal/InputPin.h>

class StartPinInitializer: public ClockedInitializable {
private:
    InputPin& startPin;
public:
    StartPinInitializer(InputPin& startPin);

    virtual void startInitializing() override;
    virtual bool tickInit() override;

};

#endif /* CONTROL_STARTPININITIALIZER_H_ */
/** @} */
