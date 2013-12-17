#include "platform/include/PegasusSTM32VB.h"
#include "core/include/ComManager.h"

namespace pegasus {

    namespace fc {
        Engine engine;
    }

    namespace hal {

        /**************************************************************
         * SPI
         **************************************************************/
        namespace spi {
            using namespace pegasus::hal::gpio;

            const SpiPort spiPort[SPI_PORT_COUNT] = {
                    {{A, PIN7}, {A, PIN6}, {F, PIN5}, AlternateFunction::AF5}, /* SPI1 MOSI, MISO, SCK */
                    {{A, PIN7}, {A, PIN6}, {F, PIN5}, AlternateFunction::AF5}, /* SPI2 MOSI, MISO, SCK */
                    {{F, PIN9}, {F, PIN8}, {F, PIN7}, AlternateFunction::AF5}  /* SPI5 MOSI, MISO, SCK */
            };


            pegasus::hal::SpiDriver spi5(SPI5, spiPort[SPI_3]);
        }


        /**************************************************************
         * UART
         **************************************************************/
        namespace uart {
            using namespace pegasus::hal::gpio;

            const UartConfig uartConfig[UART_PORT_COUNT] = {
                    {{A, PIN10},{A, PIN9}, AlternateFunction::AF7, 115200}, /* UART 1 */
                    {{D, PIN6},{D, PIN5}, AlternateFunction::AF7, 115200},  /* UART 2 */
                    {{D, PIN9},{D, PIN8}, AlternateFunction::AF7, 115200}   /* UART 3 */
            };

            pegasus::hal::UARTDriver uart3(USART3, uartConfig[UART_3]);

        }

        namespace pwm {

        }
    }

   /* namespace core {
        shell.init(&pegasus::hal::uart::uart3);
    }*/
}

using namespace pegasus::hal::gpio;


/**
 * Gyro
 */
pegasus::hal::Gpio l3gd20Cs(C, PIN1);
pegasus::hal::SpiDevice spiL3GD20(&pegasus::hal::spi::spi5, &l3gd20Cs);
pegasus::peripherals::L3GD20 l3gd20(&spiL3GD20);

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
    /* Init engine */
    pegasus::core::com.addDevice(&pegasus::hal::uart::uart3);
    pegasus::fc::engine.init(&l3gd20, &sonar);
}



