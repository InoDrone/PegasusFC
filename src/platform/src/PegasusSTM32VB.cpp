#include "platform/include/PegasusSTM32VB.h"
#include "hal/stm32f4/include/Core.h"
#include "core/include/ComManager.h"
#include "fc/include/RC.h"

namespace pegasus {

    namespace hal {

        /**************************************************************
         * SPI
         **************************************************************/
        namespace spi {
            using namespace pegasus::hal::gpio;

            const SpiPort spiPort[SPI_PORT_COUNT] = {
                    {{A, PIN7}, {A, PIN6}, {A, PIN5}, AlternateFunction::AF5}, /* SPI1 MOSI, MISO, SCK */
                    {{A, PIN7}, {A, PIN6}, {A, PIN5}, AlternateFunction::AF5}, /* SPI2 MOSI, MISO, SCK */
                    {{F, PIN9}, {F, PIN8}, {F, PIN7}, AlternateFunction::AF5}  /* SPI5 MOSI, MISO, SCK */
            };


            pegasus::hal::SpiDriver spi1(SPI1, spiPort[SPI_1]);
        }


        /**************************************************************
         * UART
         **************************************************************/
        namespace uart {
            using namespace pegasus::hal::gpio;

            const UartConfig uartConfig[UART_PORT_COUNT] = {
                    {{A, PIN10},{A, PIN9}, AlternateFunction::AF7, 115200}, /* UART 1 */
                    {{A, PIN3},{A, PIN2}, AlternateFunction::AF7, 9600},  /* UART 2 9600 */
                    {{D, PIN9},{D, PIN8}, AlternateFunction::AF7, 115200}   /* UART 3 */
            };

            pegasus::hal::UARTDriver uart1(USART1, uartConfig[UART_1]);
            pegasus::hal::UARTDriver uart2(USART2, uartConfig[UART_2]);
        }

        namespace pwm {

        }

        namespace timer {
            pegasus::hal::Timer tim1(TIM1);
            pegasus::hal::Timer tim2(TIM2);
            pegasus::hal::Timer tim3(TIM3);
            pegasus::hal::Timer tim4(TIM4);
            pegasus::hal::Timer tim5(TIM5);
            pegasus::hal::Timer tim6(TIM6);
            pegasus::hal::Timer tim8(TIM8);
            pegasus::hal::Timer tim9(TIM9);
        }

        pegasus::hal::USBSerial usbSerial;


        const PortMapping::PinInfo PORTMAP[PORT_MAPPING_MAX_PINS] = {
                {gpio::C, gpio::PIN6 , gpio::AlternateFunction::AF3, &timer::tim8, timer::CHANNEL_1, ESC_SPEED}, // MOTOR1 C6 (TIM8 CH1)
                {gpio::C, gpio::PIN7 , gpio::AlternateFunction::AF3, &timer::tim8, timer::CHANNEL_2, ESC_SPEED}, // MOTOR2 C7 (TIM8 CH2)
                {gpio::C, gpio::PIN8 , gpio::AlternateFunction::AF3, &timer::tim8, timer::CHANNEL_3, ESC_SPEED}, // MOTOR3 C8 (TIM8 CH3)
                {gpio::C, gpio::PIN9 , gpio::AlternateFunction::AF3, &timer::tim8, timer::CHANNEL_4, ESC_SPEED}, // MOTOR4 C9 (TIM8 CH4)
                {gpio::A, gpio::PIN0 , gpio::AlternateFunction::AF2, &timer::tim5, timer::CHANNEL_1, ESC_SPEED}, // MOTOR5 A0 (TIM5 CH1)
                {gpio::A, gpio::PIN1 , gpio::AlternateFunction::AF2, &timer::tim5, timer::CHANNEL_2, ESC_SPEED}, // MOTOR6 A1 (TIM5 CH2)
                {gpio::A, gpio::PIN2 , gpio::AlternateFunction::AF2, &timer::tim5, timer::CHANNEL_3, ESC_SPEED}, // MOTOR7 A2 (TIM5 CH3)
                {gpio::A, gpio::PIN3 , gpio::AlternateFunction::AF2, &timer::tim5, timer::CHANNEL_4, ESC_SPEED}, // MOTOR8 A3 (TIM5 CH4)

                {gpio::E, gpio::PIN5 , gpio::AlternateFunction::AF3, &timer::tim9, timer::CHANNEL_1, SERVO_SPEED}, // SERVO1 E5 (TIM9 CH1)
                {gpio::E, gpio::PIN6 , gpio::AlternateFunction::AF3, &timer::tim9, timer::CHANNEL_2, SERVO_SPEED}, // SERVO2 E6 (TIM9 CH2)
                {gpio::B, gpio::PIN10, gpio::AlternateFunction::AF1, &timer::tim2, timer::CHANNEL_3, SERVO_SPEED}, // SERVO3 B10 (TIM2 CH3)
                {gpio::B, gpio::PIN11, gpio::AlternateFunction::AF1, &timer::tim2, timer::CHANNEL_4, SERVO_SPEED}, // SERVO4 B11 (TIM2 CH4)

                {gpio::E, gpio::PIN9 , gpio::AlternateFunction::AF1, &timer::tim1, timer::CHANNEL_1, 0xFFFF}, // RC1 E9  (TIM1 CH1)
                {gpio::E, gpio::PIN11, gpio::AlternateFunction::AF1, &timer::tim1, timer::CHANNEL_2, 0xFFFF}, // RC2 E11 (TIM1 CH2)
                {gpio::E, gpio::PIN13, gpio::AlternateFunction::AF1, &timer::tim1, timer::CHANNEL_3, 0xFFFF}, // RC3 E13 (TIM1 CH3)
                {gpio::E, gpio::PIN14, gpio::AlternateFunction::AF1, &timer::tim1, timer::CHANNEL_4, 0xFFFF}, // RC4 E14 (TIM1 CH4)
                {gpio::D, gpio::PIN12, gpio::AlternateFunction::AF2, &timer::tim4, timer::CHANNEL_1, 0xFFFF}, // RC5 D12 (TIM4 CH1)
                {gpio::D, gpio::PIN13, gpio::AlternateFunction::AF2, &timer::tim4, timer::CHANNEL_2, 0xFFFF}, // RC5 D13 (TIM4 CH1)
                {gpio::D, gpio::PIN14, gpio::AlternateFunction::AF2, &timer::tim4, timer::CHANNEL_3, 0xFFFF}, // RC5 D14 (TIM4 CH1)
                {gpio::D, gpio::PIN15, gpio::AlternateFunction::AF2, &timer::tim4, timer::CHANNEL_4, 0xFFFF}, // RC5 D15 (TIM4 CH1)
        };
    }

   /* namespace core {
        shell.init(&pegasus::hal::uart::uart3);
    }*/
}

using namespace pegasus::hal::gpio;
using namespace pegasus::hal::stm32f4;
using namespace pegasus::hal::timer;


/**
 * Gyro
 */
pegasus::hal::Gpio mpu6000CsPin(E, PIN12);
pegasus::hal::SpiDevice spiMPU6000(&pegasus::hal::spi::spi1, &mpu6000CsPin);
pegasus::peripherals::MPU6000 mpu6000(&spiMPU6000);

/**
 * Sonar
 */
//const pegasus::hal::pwm::PWMConfig sonarConfig = {{B, PIN0} , AlternateFunction::AF2, 0xFFFF};

pegasus::hal::Gpio sonarIo(E, PIN6);
pegasus::hal::TimerChannel sonarChannel(&tim9, pegasus::hal::timer::CHANNEL_2); // TIM3_C3
pegasus::hal::PWMInput sonarInput(&sonarIo, AlternateFunction::AF3, &sonarChannel, 0xFFFF);
pegasus::peripherals::MaxSonar sonar(&sonarInput);

/**
 * LEDS
 */
pegasus::peripherals::Led ledYellow(E, PIN0);
pegasus::peripherals::Led ledWhite(E, PIN2);
pegasus::peripherals::Led ledRed(E, PIN1);
pegasus::peripherals::Led ledBlue(E, PIN3);

pegasus::hal::Gpio baroCSPin(E, PIN15);
pegasus::hal::SpiDevice spiBARO(&pegasus::hal::spi::spi1, &baroCSPin);
pegasus::peripherals::MS5611 MS5611(&spiBARO);

void initPlatform()
{

    /* INIT COM */
    pegasus::core::com.addDevice(&pegasus::hal::usbSerial); // Usb Serial
    pegasus::core::com.addDevice(&pegasus::hal::uart::uart2); // Telemetry

    /* Platform information */
    Clocks clocks;
    Core::getSystemClock(&clocks);

    pegasus::core::trace.log("Platform:\tPegasus STM32VB");
    pegasus::core::trace.log("** Timing Information **");
    pegasus::core::trace.log("* SYSCLK : %dHz", clocks.SYSCLK);
    pegasus::core::trace.log("* HCLK   : %dHz", clocks.HCLK);
    pegasus::core::trace.log("* PCLK1  : %dHz", clocks.PCLK1);
    pegasus::core::trace.log("* PCLK2  : %dHz", clocks.PCLK2);
    pegasus::core::trace.log("**");

    /* Add led manager */
    pegasus::fc::engine.addLed(LED_YELLOW, &ledYellow);
    pegasus::fc::engine.addLed(LED_WHITE, &ledWhite);
    pegasus::fc::engine.addLed(LED_RED, &ledRed);
    pegasus::fc::engine.addLed(LED_BLUE, &ledBlue);

    /* INIT PORTMAPIING */
    pegasus::hal::portMapping.add(MOTOR1);
    pegasus::hal::portMapping.add(MOTOR2);
    pegasus::hal::portMapping.add(MOTOR3);
    pegasus::hal::portMapping.add(MOTOR4);

    pegasus::hal::portMapping.add(RC1);
    pegasus::hal::portMapping.add(RC2);
    pegasus::hal::portMapping.add(RC3);
    pegasus::hal::portMapping.add(RC4);
    pegasus::hal::portMapping.add(RC5);
    pegasus::hal::portMapping.add(RC6);

    /* INIT RCChannel*/
    pegasus::fc::rc.throttle.init(RC_THROTTLE);
    pegasus::fc::rc.roll.init(RC_ROLL);
    pegasus::fc::rc.pitch.init(RC_PITCH);
    pegasus::fc::rc.yaw.init(RC_YAW);
    pegasus::fc::rc.aux1.init(RC_ALT_HOLD);
    //pegasus::fc::rc.aux2.init();


    /* INIT Engine */
    pegasus::fc::engine.init(&tim6, &pegasus::fc::rc, &mpu6000, &MS5611, &sonar);
}



