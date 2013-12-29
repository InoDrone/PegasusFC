#include "platform/include/PegasusSTM32VB.h"
#include "hal/stm32f4/include/Core.h"
#include "core/include/ComManager.h"

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
                    {{A, PIN3},{A, PIN2}, AlternateFunction::AF7, 9600},  /* UART 2 */
                    {{D, PIN9},{D, PIN8}, AlternateFunction::AF7, 115200}   /* UART 3 */
            };

            pegasus::hal::UARTDriver uart1(USART1, uartConfig[UART_1]);
            pegasus::hal::UARTDriver uart2(USART2, uartConfig[UART_2]);
        }

        namespace pwm {

        }

        pegasus::hal::USBSerial usbSerial;
    }

   /* namespace core {
        shell.init(&pegasus::hal::uart::uart3);
    }*/
}

using namespace pegasus::hal::gpio;
using namespace pegasus::hal::stm32f4;

/**
 * Main Timer Engine 200hz
 */
pegasus::hal::Timer tim6(TIM6);

/**
 * Gyro
 */
pegasus::hal::Gpio mpu6000CsPin(E, PIN12);
pegasus::hal::SpiDevice spiMPU6000(&pegasus::hal::spi::spi1, &mpu6000CsPin);
pegasus::peripherals::MPU6000 mpu6000(&spiMPU6000);

/**
 * Sonar
 */
const pegasus::hal::pwm::PWMConfig sonarConfig = {{B, PIN0} , AlternateFunction::AF2, 0xFFFF};

pegasus::hal::Timer tim3(TIM3);
pegasus::hal::TimerChannel sonarChannel(&tim3, pegasus::timer::CHANNEL_3); // TIM3_C3
pegasus::hal::PWMInput sonarInput(sonarConfig, &sonarChannel);
pegasus::peripherals::MaxSonar sonar(&sonarInput);

void initPlatform()
{
    /* INIT COM */
    pegasus::core::com.addDevice(&pegasus::hal::usbSerial); // Usb Serial
    pegasus::core::com.addDevice(&pegasus::hal::uart::uart2); // Telemetry

    /* Platform information */
    Clocks clocks;
    Core::getSystemClock(&clocks);

    pegasus::core::trace.log("Platform:\tPegasus STM32VB");
    pegasus::core::trace.log("** Timing Information :");
    pegasus::core::trace.log("* SYSCLK : %dHz", clocks.SYSCLK);
    pegasus::core::trace.log("* HCLK   : %dHz", clocks.HCLK);
    pegasus::core::trace.log("* PCLK1  : %dHz", clocks.PCLK1);
    pegasus::core::trace.log("* PCLK2  : %dHz", clocks.PCLK2);
    pegasus::core::trace.log("**");

    /* INIT Engine */
    pegasus::fc::engine.init(&tim6, &mpu6000, &sonar);
}



