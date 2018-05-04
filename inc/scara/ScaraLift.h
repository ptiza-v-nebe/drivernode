/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Scara
 * @{
 ******************************************************************************
 */

#ifndef SCARA_SCARALIFT_H_
#define SCARA_SCARALIFT_H_

#include <hal/Encoder.h>
#include <hal/Motor.h>
#include <hal/InputPin.h>
#include <control/Clocked.h>

class ScaraLift : public Clocked {
private:
    Motor& motor; ///< reference to the motor
    Encoder& encoder; ///< reference to the encoder
    InputPin& endStop; ///< reference to the end stop
    int32_t startPosition; ///< the starting position for a movement
    int32_t targetPosition; ///< the target position
    bool initialized; ///< is the lift initialized

    float lastSpeed;
    int16_t currentPosition;
    int16_t speed;
   // int16_t lastPosition;
    int16_t lastError;
public:
    ScaraLift(Motor& motor, Encoder& encoder, InputPin& endStop);

    // prevent copy and move
    ScaraLift(const ScaraLift&) = delete;
    ScaraLift(ScaraLift&&) = delete;
    ScaraLift& operator=(const ScaraLift&) = delete;
    ScaraLift& operator=(ScaraLift&&) = delete;

    void initialize();
    bool tickInit();

    void moveTo(float mm);
    float getPosition();
    void tick() override;
    void stop();
    void disable();
};

#endif /* SCARA_SCARALIFT_H_ */
/** @} */
