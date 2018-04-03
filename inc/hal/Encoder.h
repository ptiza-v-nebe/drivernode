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
    int32_t tickBuffers[TICK_BUFFER_COUNT]; ///< buffers for interrupt safe reset
    int currentBuffer; ///< pointer to the buffer that is currently written

    GPIO_TypeDef *gpio; ///< which gpio bank is used
    uint16_t channelAPin; ///< which pin is connected to encoder channel a
    uint16_t channelBPin; ///< which pin is connected to encoder channel b
    Phase currentPhase; ///< the current phase
    const int sign; ///< used to invert the encoder
public:
    Encoder(GPIO_TypeDef *gpio, uint16_t channelAPin, uint16_t channelBPin, bool invert = false);

    // prevent move and copy
    Encoder(const Encoder&) = delete;
    Encoder(Encoder&&) = delete;
    Encoder& operator=(const Encoder&) = delete;
    Encoder& operator=(Encoder&&) = delete;

    void update();
    int32_t getTick() const;
    void reset();
    int32_t getTickAndReset();
private:
    Phase getPhase();
};

#endif /* HAL_ENCODER_H_ */
/** @} */
