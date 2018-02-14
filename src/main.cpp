/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 ******************************************************************************
 */
#include "hal/util.h"
#include "scheduler/Scheduler.h"
#include "serial/UARTWrapper.h"
#include "serial/ODROIDCommandHandler.h"

#include "serial/TestMessage1.h"
#include "serial/TestMessage2.h"

class DebugUARTReceiveHandler: public UARTReceiveHandler {
public:
    void processByte(uint8_t) override {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    }
};

class EchoUARTReceiveHandler: public UARTReceiveHandler {
public:
    void processByte(uint8_t byte) override {
        schedule_task_from_isr([byte]() {
            UARTWrapper::getInstance().send(&byte, 1);
        });
    }
};

class SmartEchoUARTReceiveHandler: public UARTReceiveHandler {
private:
    uint8_t buffers[2][50];
    int currentBuffer = 0;
    int count = 0;
public:
    void processByte(uint8_t byte) override {
        buffers[currentBuffer][count] = byte;
        count++;

        if (byte == '\n' || count == 50) {

            uint8_t* msg = buffers[currentBuffer];
            int size = count;

            schedule_task_from_isr([msg, size]() {
                UARTWrapper::getInstance().send(msg, size);
            });
            currentBuffer = (currentBuffer + 1) % 2;
            count = 0;
        }

        if (byte == '\r') {
            // add newline after carriage return
            processByte('\n');
        }
    }
};

int main(void) {
    setupHardware();
    __HAL_RCC_GPIOA_CLK_ENABLE()
    ;

    GPIO_InitTypeDef gpioa = getDefaultGPIO();
    gpioa.Pin = GPIO_PIN_5;
    gpioa.Mode = GPIO_MODE_OUTPUT_PP;

    HAL_GPIO_Init(GPIOA, &gpioa);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

    schedule_repeating_task([]() {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    }, 500);

    MessageDispatcher dispatcher;
    dispatcher.registerMessageHandler<TestMessage1>([](TestMessage1 msg) {
        msg.printYourself();
    });
    dispatcher.registerMessageHandler<TestMessage2>([](TestMessage2 msg) {
        msg.printYourself();
    });

    UARTWrapper& uartWrapper = UARTWrapper::getInstance();
    //DebugUARTReceiveHandler handler;
    //EchoUARTReceiveHandler handler;
    //SmartEchoUARTReceiveHandler handler;
    ODROIDCommandHandler handler(dispatcher);
    uartWrapper.setReceiveHandler(&handler);

    schedule_repeating_task([&uartWrapper]() {
        //uartWrapper.send("Hello World - From the Nucleo\r\n");
        printf("Well hello there from printf - %d, %.2f, %X.\r\n", 15, 133.456, 255);// @suppress("Float formatting support")
    }, 1000, 250);

    start_scheduler();

    for (;;)
        ;
}
