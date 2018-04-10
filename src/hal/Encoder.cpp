/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include <hal/Encoder.h>
#include "error.h"

/**
 * Calculate the next phase, assuming forward movement.
 *
 * @param p the current phase
 * @return the expected next phase for forward movement
 */
static Phase phaseAfter(const Phase& p) {
    return static_cast<Phase>((static_cast<int>(p) + 1)
            % static_cast<int>(Phase::_Count));
}

/**
 * Calculate the next phase, assuming backward movement.
 *
 * @param p the current phase
 * @return the expected next phase for backward movement
 */
static Phase phaseBefore(const Phase& p) {
    return static_cast<Phase>((static_cast<int>(p)
            + static_cast<int>(Phase::_Count) - 1)
            % static_cast<int>(Phase::_Count));
}

/**
 * Constructs an Encoder.
 *
 * @param gpio        the gpio bank
 * @param channelAPin the pin connected to channel a
 * @param channelBPin the pin connected to channel b
 * @param invert      should the encoder evaluation be inverted?
 */
Encoder::Encoder(GPIO_TypeDef* gpio, uint16_t channelAPin, uint16_t channelBPin,
        bool invert) :
        tickBuffers { }, currentBuffer(0), gpio(gpio), channelAPin(channelAPin), channelBPin(
                channelBPin), currentPhase(Phase::A), sign(invert ? -1 : 1) {
    currentPhase = getPhase();
}

/**
 * updates the encoder.
 * this will read both channels and add/subtract tick accordingly
 */
void Encoder::update() {
    Phase nextPhase = getPhase();
    if (currentPhase != nextPhase) {
        if (nextPhase == phaseAfter(currentPhase)) {
            // Going forward: Phase A -> B -> C -> D -> A -> B -> ...
            tickBuffers[currentBuffer] += sign;
        } else if (nextPhase == phaseBefore(currentPhase)) {
            // Going backward: Phase A -> D -> C -> B -> A -> D -> C -> ...
            tickBuffers[currentBuffer] -= sign;
        } else {
            ERROR("Skipped a tick!");
        }
    }
    currentPhase = nextPhase;
}

/**
 * @return the current tick count. positive means forward, negative means backward
 */
int32_t Encoder::getTick() const {
    return tickBuffers[currentBuffer];
}

/**
 * resets the tick count to 0
 */
void Encoder::reset() {
    tickBuffers[currentBuffer] = 0;
}

/**
 * resets the tick count to 0 and returns the old value in an interrupt safe way (no ticks are missed in between)
 * @return the tick count
 */
int32_t Encoder::getTickAndReset() {
    // save current buffer
    int oldBuffer = currentBuffer;

    // calculate new buffer
    int newBuffer = (currentBuffer + 1) % TICK_BUFFER_COUNT;

    // reset new buffer and change current buffer so updates get written to the new buffer
    tickBuffers[newBuffer] = 0;
    currentBuffer = newBuffer;

    return tickBuffers[oldBuffer];
}

/**
 * @return the current phase
 */
Phase Encoder::getPhase() {
    bool a = HAL_GPIO_ReadPin(gpio, channelAPin) == GPIO_PIN_SET;
    bool b = HAL_GPIO_ReadPin(gpio, channelBPin) == GPIO_PIN_SET;

    // Explanation of Phase:
    //           |___|___|   |     ________
    // A:     ___|   |   |___|____|        |________
    //           |   |___|___|         ________
    // B:     ___|___|   |   |________|        |______
    //           |   |   |   |
    // Phase:  A | B | C | D |

    if (a) {
        if (b) {
            return Phase::C;
        } else {
            return Phase::B;
        }
    } else {
        if (b) {
            return Phase::D;
        } else {
            return Phase::A;
        }
    }
}
/** @} */
