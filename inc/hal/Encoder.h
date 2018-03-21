/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#ifndef HAL_ENCODER_H_
#define HAL_ENCODER_H_

#include "stm32l4xx.h"
#include "stm32l4xx_hal_gpio.h"
#include <cstdint>

constexpr int TICK_BUFFER_COUNT = 2;

enum class Phase {
    A, B, C, D, _Count
};

class Encoder {
private:
    int tickBuffers[TICK_BUFFER_COUNT];
    int currentBuffer;

    GPIO_TypeDef *gpio;
    uint32_t channelAPin;
    uint32_t channelBPin;
    Phase currentPhase;
    const int sign;
public:
    Encoder(GPIO_TypeDef *gpio, uint32_t channelAPin, uint32_t channelBPin, int sign = 1);

    // prevent move and copy
    Encoder(const Encoder&) = delete;
    Encoder(Encoder&&) = delete;
    Encoder& operator=(const Encoder&) = delete;
    Encoder& operator=(Encoder&&) = delete;

    void update();
    int getTick() const;
    void reset();
    int getTickAndReset();
private:
    Phase getPhase();
};

#endif /* HAL_ENCODER_H_ */
/** @} */
