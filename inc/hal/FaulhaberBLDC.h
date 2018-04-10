/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup HAL
 * @{
 ******************************************************************************
 */

#ifndef HAL_FAULHABERBLDC_H_
#define HAL_FAULHABERBLDC_H_

#include <hal/Motor.h>
#include <stm32l4xx.h>

class FaulhaberBLDC: public Motor {
private:
    bool enabled; ///< is the motor enabled
    UART_HandleTypeDef *uart; ///< handle for accessing the UART
    uint8_t id; ///< the id of the motor
    bool reverseDirection; ///< should the direction be inverted
public:
    FaulhaberBLDC(UART_HandleTypeDef *uart, uint8_t id, bool reverseDirection =
            false);

    // prevent copy and move
    FaulhaberBLDC(const FaulhaberBLDC&) = delete;
    FaulhaberBLDC(FaulhaberBLDC&&) = delete;
    FaulhaberBLDC& operator=(const FaulhaberBLDC&) = delete;
    FaulhaberBLDC& operator=(FaulhaberBLDC&&) = delete;

    virtual void enable() override;
    virtual void disableAndStop() override;
    virtual void setSpeed(int16_t speed) override;
    virtual void stop() override;
private:
    void sendCommand(const char *command);
};

#endif /* HAL_FAULHABERBLDC_H_ */
/** @} */
