/*
 * STM32GPIO.h
 *
 *  Created on: 6 nov. 2013
 *      Author: alienx
 */

#ifndef STM32GPIO_H_
#define STM32GPIO_H_

#include "STM32Defs.h"
#include "STM32Timer.h"
#include "GPIO.h"

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_tim.h"

#define Port2Pin(port, bit) ((uint8_t)((port-'A')*16+bit))
#define STM32_GPIO_COUNT 112

#define NO_TIMER 0xFF
#define NO_ADC	 0xFF

#define TIM_NONE (uint16_t)0
#define ADC_NONE (uint16_t)0

namespace Pegasus {
  namespace arch {
    namespace STM32 {

      class STM32GPIO : public Pegasus::GPIO {
      public:


              struct Pin {
                      GPIO_TypeDef*       device;
                      const STM32Timer::Timer*  timer;
                      ADC_TypeDef*        adc;
                      uint16_t            bit;
                      uint16_t            timer_channel;
                      uint8_t             adc_channel;
              };

              STM32GPIO();

              void init();

              void pinMode(const char port, uint8_t bit, uint8_t output);
              uint8_t read(const char port, uint8_t bit);
              void write(const char port, uint8_t bit, uint8_t value);
              void toogle(const char port, uint8_t bit);

              void pinMode(uint8_t pin, uint8_t output);
              uint8_t read(uint8_t pin);
              void write(uint8_t pin, uint8_t value);
              void toogle(uint8_t pin);

              void pinAFMode(const Pin pin, uint8_t mode);
      };

      extern STM32GPIO io;

      extern const STM32GPIO::Pin PINS[];
    }
  }
}



#endif /* STM32GPIO_H_ */
