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
#include "serial/HumanCommandHandler.h"
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
            if (byte == '\r' || count == 50) {
                uint8_t* msg = buffers[currentBuffer];
                int size = count;

                schedule_task_from_isr([msg, size]() {
                    // we need to echo the linebreak
                        uint8_t buffer[2];
                        buffer[0] = '\r';
                        buffer[1] = '\n';
                        UARTWrapper::getInstance().send(buffer, 2);

                        UARTWrapper::getInstance().send(msg, size);
                        UARTWrapper::getInstance().send(buffer, 2);
                    });
                currentBuffer = (currentBuffer + 1) % 2;
                count = 0;

            } else if (isValidCharacter(byte)) {
                buffers[currentBuffer][count] = byte;
                count++;

                schedule_task_from_isr([byte]() {
                    UARTWrapper::getInstance().send(&byte, 1);
                });
            }
        }
    }
private:
    bool isValidCharacter(uint8_t& byte) {
        bool uppercase = (byte >= 'A' && byte <= 'Z');
        if (uppercase) {
            byte = byte + ('a' - 'A'); // convert to lowercase
            return true;
        }

        bool lowercase = (byte >= 'a' && byte <= 'z');
        bool number = (byte >= '0' && byte <= '9');
        bool space = (byte == ' ');

        return lowercase || number || space;
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

    /*dispatcher.registerMessageHandler<StatusMessage>(
            [&dispatcher](StatusMessage sm) {
                dispatcher.sendMessage(sm);
            });
    dispatcher.registerMessageHandler<ControlledDriveMessage>(
            [&dispatcher](ControlledDriveMessage cdm) {
                dispatcher.sendMessage(cdm);
            });*/

//DebugUARTReceiveHandler handler;
//EchoUARTReceiveHandler handler;
    //SmartEchoUARTReceiveHandler handler;
//ODROIDCommandHandler handler(dispatcher);
    HumanCommandHandler handler(dispatcher);
    uartWrapper.setReceiveHandler(&handler);

    /*schedule_repeating_task([&dispatcher]() {
     StatusMessage message(Status::ABORTED);
     dispatcher.sendMessage(message);
     }, 1000, 250);*/
    /*schedule_repeating_task([&uartWrapper]() {
     //uartWrapper.send("Hello World - From the Nucleo\r\n");
     printf("Well hello there from printf - %d, %.2f, %X.\r\n", 15, 133.456, 255);
     }, 1000, 250);*/

    SimpleDriveMessage sdm1(DriveSpeed::FAST, DriveDirection::BACKWARD);
    SimpleDriveMessage sdm2(DriveSpeed::SLOW, DriveDirection::FORWARD);
    dispatcher.sendMessage(sdm1);
    dispatcher.sendMessage(sdm2);

    SimpleTurnMessage stm(TurnSpeed::FAST, TurnDirection::CCW);
    dispatcher.sendMessage(stm);

    ControlledDriveMessage cdm(DriveSpeed::FAST, 0, 256);
    dispatcher.sendMessage(cdm);

    ControlledTurnMessage ctm(TurnSpeed::SLOW, degreesToRadians(45.28));
    dispatcher.sendMessage(ctm);

    HeartbeatMessage hbm;
    dispatcher.sendMessage(hbm);

    StopMessage sm;
    dispatcher.sendMessage(sm);

    GameStartMessage gsm;
    dispatcher.sendMessage(gsm);

    ResetOdometryMessage rom(0, 256, 0.0);
    dispatcher.sendMessage(rom);

    PositionMessage pm(0xDEAD, 0xBEEF, PI);
    dispatcher.sendMessage(pm);

    SetSpeedMessage ssm1(100);
    SetSpeedMessage ssm2(25, 3000);
    dispatcher.sendMessage(ssm1);
    dispatcher.sendMessage(ssm2);

    start_scheduler();

    for (;;)
        ;
}
