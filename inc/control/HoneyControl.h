/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Control
 * @{
 ******************************************************************************
 */

#ifndef CONTROL_HONEYCONTROL_H_
#define CONTROL_HONEYCONTROL_H_

#include "ClockedInitializable.h"

#include <hal/DynamixelAX12A.h>
#include <serial/messages/ServoControlMessage.h>

class HoneyControl : public ClockedInitializable {
private:
    DynamixelAX12A& leftServo;
    DynamixelAX12A& rightServo;
public:
    HoneyControl(DynamixelAX12A& leftServo, DynamixelAX12A& rightServo);

    void startInitializing() override;
    bool tickInit() override;

    void processCommand(const ServoControlMessage& scm);
};

#endif /* CONTROL_HONEYCONTROL_H_ */
/** @} */
