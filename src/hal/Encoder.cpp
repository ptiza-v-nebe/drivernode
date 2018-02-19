/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#include <hal/Encoder.h>

static Phase phaseAfter(const Phase& p) {
    return static_cast<Phase>((static_cast<int>(p) + 1) % static_cast<int>(Phase::_Count));
}

static Phase phaseBefore(const Phase& p) {
    return static_cast<Phase>((static_cast<int>(p) + static_cast<int>(Phase::_Count) - 1) % static_cast<int>(Phase::_Count));
}

Encoder::Encoder(GPIO_TypeDef* gpio, uint32_t channelAPin, uint32_t channelBPin, int sign) :
        currentBuffer(0), gpio(gpio), channelAPin(channelAPin), channelBPin(
                channelBPin), sign(sign) {
    currentPhase = getPhase();
}

void Encoder::update() {
    Phase nextPhase = getPhase();
    if (currentPhase != nextPhase) {
        if(nextPhase == phaseAfter(currentPhase)) {
            // forward
            tickBuffers[currentBuffer] += sign;
        } else if(nextPhase == phaseBefore(currentPhase)) {
            // backward
            tickBuffers[currentBuffer] -= sign;
        } else {
            // TODO: detect invalid
        }
    }
    currentPhase = nextPhase;
}

int Encoder::getTick() const {
    return tickBuffers[currentBuffer];
}

void Encoder::reset() {
    tickBuffers[currentBuffer] = 0;
}

int Encoder::getTickAndReset() {
    // save current buffer
    int oldBuffer = currentBuffer;

    // calculate new buffer
    int newBuffer = (currentBuffer + 1) % TICK_BUFFER_COUNT;

    // reset new buffer and change current buffer so updates get written to the new buffer
    tickBuffers[newBuffer] = 0;
    currentBuffer = newBuffer;

    return tickBuffers[oldBuffer];
}

Phase Encoder::getPhase() {
    bool a = HAL_GPIO_ReadPin(gpio, channelAPin) == GPIO_PIN_SET;
    bool b = HAL_GPIO_ReadPin(gpio, channelBPin) == GPIO_PIN_SET;

    // Explanation of Phase:
    //             ________          ________
    // A:     ____|        |________|        |________
    //                ________          ________
    // B:     _______|        |________|        |______
    //
    // Phase:   A   B   C   D

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
