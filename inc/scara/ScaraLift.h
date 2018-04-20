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

class ScaraLift {
private:
    Motor& motor; ///< reference to the motor
    Encoder& encoder; ///< reference to the encoder
    int32_t startPosition; ///< the starting position for a movement
    int32_t targetPosition; ///< the target position
    bool initialized; ///< is the lift initialized
public:
    ScaraLift(Motor& motor, Encoder& encoder);

    // prevent copy and move
    ScaraLift(const ScaraLift&) = delete;
    ScaraLift(ScaraLift&&) = delete;
    ScaraLift& operator=(const ScaraLift&) = delete;
    ScaraLift& operator=(ScaraLift&&) = delete;

    void initialize();

    void moveTo(float mm);
    float getPosition();

private:
    void tick();
    int16_t clamp(int16_t value);
};

#endif /* SCARA_SCARALIFT_H_ */
/** @} */
