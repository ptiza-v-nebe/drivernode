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
#include "serial/HumanSender.h"

#include "serial/messages/all.h"
#include "util/conversions.h"

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
            printf("%X ", byte);
            fflush(stdout);
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
        if (byte == 0x7F || byte == '\b') {
            // delete or backspace
            if (count > 0) {
                count--;

                schedule_task_from_isr([]() {
                    uint8_t buffer[] = {'\b', ' ', '\b'};
                    UARTWrapper::getInstance().send(buffer, 3);
                });
            }
        } else {
            buffers[currentBuffer][count] = byte;
            count++;

            if (byte == '\n' || count == 50) {

                uint8_t* msg = buffers[currentBuffer];
                int size = count;

                schedule_task_from_isr([msg, size]() {
                    // we need to echo the characters
                        uint8_t buffer[2];
                        buffer[0] = '\r';
                        buffer[1] = '\n';
                        UARTWrapper::getInstance().send(buffer, 2);

                        UARTWrapper::getInstance().send(msg, size);
                    });
                currentBuffer = (currentBuffer + 1) % 2;
                count = 0;
            } else if (byte == '\r') {
                // add newline after carriage return
                processByte('\n');
            } else {
                schedule_task_from_isr([byte]() {
                    UARTWrapper::getInstance().send(&byte, 1);
                });
            }
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

    UARTWrapper& uartWrapper = UARTWrapper::getInstance();
    //MessageDispatcher dispatcher(uartWrapper);

    HumanSender hSender;
    MessageDispatcher dispatcher(hSender);

    dispatcher.registerMessageHandler<StatusMessage>(
            [&dispatcher](StatusMessage sm) {
                dispatcher.sendMessage(sm);
            });
    dispatcher.registerMessageHandler<ControlledDriveMessage>(
            [&dispatcher](ControlledDriveMessage cdm) {
                dispatcher.sendMessage(cdm);
            });

    //DebugUARTReceiveHandler handler;
    //EchoUARTReceiveHandler handler;
    //SmartEchoUARTReceiveHandler handler;
    ODROIDCommandHandler handler(dispatcher);
    uartWrapper.setReceiveHandler(&handler);

    /*schedule_repeating_task([&dispatcher]() {
     StatusMessage message(Status::ABORTED);
     dispatcher.sendMessage(message);
     }, 1000, 250);*/
    /*schedule_repeating_task([&uartWrapper]() {
     //uartWrapper.send("Hello World - From the Nucleo\r\n");
     printf("Well hello there from printf - %d, %.2f, %X.\r\n", 15, 133.456, 255);
     }, 1000, 250);*/

    ResetOdometryMessage rom(0, 256, 0.0);
    dispatcher.sendMessage(rom);

    PositionMessage pm(1500, 0xAFFE, PI);
    dispatcher.sendMessage(pm);

    SimpleDriveMessage sdm1(DriveSpeed::FAST, DriveDirection::BACKWARD);
    SimpleDriveMessage sdm2(DriveSpeed::SLOW, DriveDirection::FORWARD);
    dispatcher.sendMessage(sdm1);
    dispatcher.sendMessage(sdm2);

    start_scheduler();

    for (;;)
        ;
}
