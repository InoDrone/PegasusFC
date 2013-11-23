/*
 * STM32GPIO.cpp
 *
 *  Created on: 6 nov. 2013
 *      Author: alienx
 */

#include "STM32GPIO.h"

namespace Pegasus {
  namespace arch {
    namespace STM32 {

      STM32GPIO::STM32GPIO() {}

      void STM32GPIO::init()
      {
          //RCC->AHB1ENR |= RCC_AHB1Periph_GPIOG
         // RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
          RCC->AHB1ENR |= (RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOGEN);
          RCC->DCKCFGR |= RCC_DCKCFGR_TIMPRE;
      }

      void STM32GPIO::pinMode(const char port, uint8_t bit, uint8_t mode)
      {
              uint8_t pin = Port2Pin(port, bit);
              pinMode(pin, mode);

      }

      void STM32GPIO::pinMode(uint8_t pin, uint8_t mode)
      {
           const Pin io = PINS[pin];


              /*
               * GPIO_Mode_IN   = 0x00 !< GPIO Input Mode
           * GPIO_Mode_OUT  = 0x01 !< GPIO Output Mode
           * GPIO_Mode_AF   = 0x02 !< GPIO Alternate function Mode
           * GPIO_Mode_AN   = 0x03 !< GPIO Analog Mode
           *
           * GPIO_OType_PP !< Push Pull
           * GPIO_OType_OD !< Open drain
           *
           * GPIO_Speed_2Mhz
           * GPIO_Speed_25Mhz
           * GPIO_Speed_50Mhz
           * GPIO_Speed_100Mhz (30pF)
           *
           * GPIO_PuPd_NOPULL
           * GPIO_PuPd_UP
           * GPIO_PuPd_DOWN
               */

              /*io.device->MODER  &=  ~(GPIO_MODER_MODER0 << (io.pin*2));
              io.device->MODER  |=  ( ((uint32_t)mode) << (io.pin*2) );*/

              GPIO_InitTypeDef def;
              def.GPIO_Pin = _BIT(io.bit);

              switch(mode) {
              case OUTPUT:
                      def.GPIO_Mode  = GPIO_Mode_OUT;
                      def.GPIO_OType = GPIO_OType_PP;
                      def.GPIO_PuPd  = GPIO_PuPd_NOPULL;
                      def.GPIO_Speed = GPIO_Speed_50MHz;
                      break;
              case OUTPUT_OD:
                      def.GPIO_Mode  = GPIO_Mode_OUT;
                      def.GPIO_OType = GPIO_OType_OD;
                      def.GPIO_PuPd  = GPIO_PuPd_NOPULL;
                      def.GPIO_Speed = GPIO_Speed_50MHz;
                      break;
              case INPUT:
              case INPUT_FLOATING:
                      def.GPIO_Mode  = GPIO_Mode_IN;
                      def.GPIO_OType = GPIO_OType_PP;
                      def.GPIO_PuPd  = GPIO_PuPd_NOPULL;
                      def.GPIO_Speed = GPIO_Speed_50MHz;
                      break;
              case INPUT_PU:
                      def.GPIO_Mode  = GPIO_Mode_IN;
                      def.GPIO_OType = GPIO_OType_PP;
                      def.GPIO_PuPd  = GPIO_PuPd_UP;
                      def.GPIO_Speed = GPIO_Speed_50MHz;
                      break;
              case INPUT_PD:
                      def.GPIO_Mode  = GPIO_Mode_IN;
                      def.GPIO_OType = GPIO_OType_PP;
                      def.GPIO_PuPd  = GPIO_PuPd_DOWN;
                      def.GPIO_Speed = GPIO_Speed_50MHz;
                      break;
              case PWM:
                      def.GPIO_Mode = GPIO_Mode_AF;
                      def.GPIO_OType = GPIO_OType_PP;
                      def.GPIO_PuPd = GPIO_PuPd_UP;
                      def.GPIO_Speed = GPIO_Speed_50MHz;
                      break;
              }


              GPIO_Init(io.device, &def);
      }

      /**
       * read pin
       *
       * @param char          port
       * @param uint8_t       bit
       *
       * @return uint8_t
       */
      uint8_t STM32GPIO::read(const char port, uint8_t bit)
      {
              uint8_t pin = Port2Pin(port, bit);
              return read(pin);
      }

      /**
       * read pin
       *
       * @param uint8_t       port
       *
       * @return uint8_t
       */
      uint8_t STM32GPIO::read(uint8_t pin)
      {
              return PINS[pin].device->IDR & _BIT(PINS[pin].bit);
      }

      void STM32GPIO::write(const char port, uint8_t bit, uint8_t value)
      {
              uint8_t pin = Port2Pin(port, bit);
              write(pin, value);
      }

      void STM32GPIO::write(uint8_t pin, uint8_t value)
      {
              if (value == HIGH) {
                      PINS[pin].device->BSRRL = _BIT(PINS[pin].bit);
              } else {
                      PINS[pin].device->BSRRH = _BIT(PINS[pin].bit);
              }
      }

      /**
       *
       */
      void STM32GPIO::toogle(const char port, uint8_t bit)
      {
              uint8_t pin = Port2Pin(port, bit);
              toogle(pin);
      }

      /**
       *
       */
      void STM32GPIO::toogle(uint8_t pin)
      {
              PINS[pin].device->ODR ^= _BIT(PINS[pin].bit);
      }

      void STM32GPIO::pinAFMode(const Pin pin, uint8_t mode)
      {
          pin.device->AFR[ pin.bit >> 0x03 ] &= ~((uint32_t)0xF << ((uint32_t)((uint32_t)pin.bit & (uint32_t)0x07) * 4)); // reset pin AF
          pin.device->AFR[ pin.bit >> 0x03 ] |= ((uint32_t)mode << ((uint32_t)((uint32_t)pin.bit & (uint32_t)0x07) * 4));  // Set AF

          /*
           *   temp = ((uint32_t)(GPIO_AF) << ((uint32_t)((uint32_t)GPIO_PinSource & (uint32_t)0x07) * 4)) ;
    GPIOx->AFR[GPIO_PinSource >> 0x03] &= ~((uint32_t)0xF << ((uint32_t)((uint32_t)GPIO_PinSource & (uint32_t)0x07) * 4)) ;
    temp_2 = GPIOx->AFR[GPIO_PinSource >> 0x03] | temp;
    GPIOx->AFR[GPIO_PinSource >> 0x03] = temp_2;
           */
      }


      #if defined(STM32F429_439xx)
      const STM32GPIO::Pin PINS[STM32_GPIO_COUNT] = {
                      {GPIOA,  &TIMER5,     ADC1,  0, 1, ADC_Channel_0}, /* PA0 */
                      {GPIOA,  &TIMER5,     ADC1,  1, 2, ADC_Channel_1}, /* PA1 */
                      {GPIOA,  &TIMER5,     ADC1,  2, 3, ADC_Channel_2}, /* PA2 */
                      {GPIOA,  &TIMER5,     ADC1,  3, 4, ADC_Channel_3}, /* PA3 */
                      {GPIOA, TIM_NONE,     ADC1,  4,      TIM_NONE, ADC_Channel_4}, /* PA4 */
                      {GPIOA, TIM_NONE,     ADC1,  5,      TIM_NONE, ADC_Channel_5}, /* PA5 */
                      {GPIOA, TIM_NONE,     ADC1,  6,      TIM_NONE, ADC_Channel_6}, /* PA6 */
                      {GPIOA, TIM_NONE,     ADC1,  7,      TIM_NONE, ADC_Channel_7}, /* PA7 */
                      {GPIOA, TIM_NONE, ADC_NONE,  8,      TIM_NONE,        NO_ADC}, /* PA8 */
                      {GPIOA, TIM_NONE, ADC_NONE,  9,      TIM_NONE,        NO_ADC}, /* PA9 */
                      {GPIOA, TIM_NONE, ADC_NONE, 10,      TIM_NONE,        NO_ADC}, /* PA10 */
                      {GPIOA, TIM_NONE, ADC_NONE, 11,      TIM_NONE,        NO_ADC}, /* PA11 */
                      {GPIOA, TIM_NONE, ADC_NONE, 12,      TIM_NONE,        NO_ADC}, /* PA12 */
                      {GPIOA, TIM_NONE, ADC_NONE, 13,      TIM_NONE,        NO_ADC}, /* PA12 */
                      {GPIOA, TIM_NONE, ADC_NONE, 14,      TIM_NONE,        NO_ADC}, /* PA12 */
                      {GPIOA,  &TIMER2, ADC_NONE, 15, 1,        NO_ADC}, /* PA12 */


                      {GPIOB,  &TIMER3,     ADC1,  0, 3, ADC_Channel_8}, /* PB0 */
                      {GPIOB,  &TIMER3,     ADC1,  1, 4, ADC_Channel_9}, /* PB1 */
                      {GPIOB, TIM_NONE, ADC_NONE,  2,      NO_TIMER, NO_ADC}, /* PB2 */
                      {GPIOB,  &TIMER2, ADC_NONE,  3, 2, NO_ADC}, /* PB3 */
                      {GPIOB,  &TIMER3, ADC_NONE,  4, 1, NO_ADC}, /* PB4 */
                      {GPIOB,  &TIMER3, ADC_NONE,  5, 2, NO_ADC}, /* PB5 */
                      {GPIOB, TIM_NONE, ADC_NONE,  6,      NO_TIMER, NO_ADC}, /* PB6 */
                      {GPIOB, TIM_NONE, ADC_NONE,  7,      NO_TIMER, NO_ADC}, /* PB7 */
                      {GPIOB, TIM_NONE, ADC_NONE,  8,      NO_TIMER, NO_ADC}, /* PB8 */
                      {GPIOB, TIM_NONE, ADC_NONE,  9,      NO_TIMER, NO_ADC}, /* PB9 */
                      {GPIOB, TIM_NONE, ADC_NONE, 10,      NO_TIMER, NO_ADC}, /* PB10 */
                      {GPIOB, TIM_NONE, ADC_NONE, 11,      NO_TIMER, NO_ADC}, /* PB11 */
                      {GPIOB, TIM_NONE, ADC_NONE, 12,      NO_TIMER, NO_ADC}, /* PB12 */
                      {GPIOB, TIM_NONE, ADC_NONE, 13,      NO_TIMER, NO_ADC}, /* PB12 */
                      {GPIOB, TIM_NONE, ADC_NONE, 14,      NO_TIMER, NO_ADC}, /* PB12 */
                      {GPIOB, TIM_NONE, ADC_NONE, 15,      NO_TIMER, NO_ADC}, /* PB12 */

                      {GPIOC, NULL, NULL,  0, NO_TIMER, NO_ADC}, /* PC0 */
                      {GPIOC, NULL, NULL,  1, NO_TIMER, NO_ADC}, /* PC1 */
                      {GPIOC, NULL, NULL,  2, NO_TIMER, NO_ADC}, /* PC2 */
                      {GPIOC, NULL, NULL,  3, NO_TIMER, NO_ADC}, /* PC3 */
                      {GPIOC, NULL, NULL,  4, NO_TIMER, NO_ADC}, /* PC4 */
                      {GPIOC, NULL, NULL,  5, NO_TIMER, NO_ADC}, /* PC5 */
                      {GPIOC, NULL, NULL,  6, NO_TIMER, NO_ADC}, /* PC6 */
                      {GPIOC, NULL, NULL,  7, NO_TIMER, NO_ADC}, /* PC7 */
                      {GPIOC, NULL, NULL,  8, NO_TIMER, NO_ADC}, /* PC8 */
                      {GPIOC, NULL, NULL,  9, NO_TIMER, NO_ADC}, /* PC9 */
                      {GPIOC, NULL, NULL, 10, NO_TIMER, NO_ADC}, /* PC10 */
                      {GPIOC, NULL, NULL, 11, NO_TIMER, NO_ADC}, /* PC11 */
                      {GPIOC, NULL, NULL, 12, NO_TIMER, NO_ADC}, /* PC12 */
                      {GPIOC, NULL, NULL, 13, NO_TIMER, NO_ADC}, /* PC12 */
                      {GPIOC, NULL, NULL, 14, NO_TIMER, NO_ADC}, /* PC12 */
                      {GPIOC, NULL, NULL, 15, NO_TIMER, NO_ADC}, /* PC12 */

                      {GPIOD, NULL, NULL,  0, NO_TIMER, NO_ADC}, /* PD0 */
                      {GPIOD, NULL, NULL,  1, NO_TIMER, NO_ADC}, /* PD1 */
                      {GPIOD, NULL, NULL,  2, NO_TIMER, NO_ADC}, /* PD2 */
                      {GPIOD, NULL, NULL,  3, NO_TIMER, NO_ADC}, /* PD3 */
                      {GPIOD, NULL, NULL,  4, NO_TIMER, NO_ADC}, /* PD4 */
                      {GPIOD, NULL, NULL,  5, NO_TIMER, NO_ADC}, /* PD5 */
                      {GPIOD, NULL, NULL,  6, NO_TIMER, NO_ADC}, /* PD6 */
                      {GPIOD, NULL, NULL,  7, NO_TIMER, NO_ADC}, /* PD7 */
                      {GPIOD, NULL, NULL,  8, NO_TIMER, NO_ADC}, /* PD8 */
                      {GPIOD, NULL, NULL,  9, NO_TIMER, NO_ADC}, /* PD9 */
                      {GPIOD, NULL, NULL, 10, NO_TIMER, NO_ADC}, /* PD10 */
                      {GPIOD, NULL, NULL, 11, NO_TIMER, NO_ADC}, /* PD11 */
                      {GPIOD, &TIMER4, NULL, 12, 1, NO_ADC}, /* PD12 */
                      {GPIOD, NULL, NULL, 13, NO_TIMER, NO_ADC}, /* PD12 */
                      {GPIOD, NULL, NULL, 14, NO_TIMER, NO_ADC}, /* PD12 */
                      {GPIOD, NULL, NULL, 15, NO_TIMER, NO_ADC}, /* PD12 */

                      {GPIOE, NULL, NULL,  0, NO_TIMER, NO_ADC}, /* PE0 */
                      {GPIOE, NULL, NULL,  1, NO_TIMER, NO_ADC}, /* PE1 */
                      {GPIOE, NULL, NULL,  2, NO_TIMER, NO_ADC}, /* PE2 */
                      {GPIOE, NULL, NULL,  3, NO_TIMER, NO_ADC}, /* PE3 */
                      {GPIOE, NULL, NULL,  4, NO_TIMER, NO_ADC}, /* PE4 */
                      {GPIOE, NULL, NULL,  5, NO_TIMER, NO_ADC}, /* PE5 */
                      {GPIOE, NULL, NULL,  6, NO_TIMER, NO_ADC}, /* PE6 */
                      {GPIOE, NULL, NULL,  7, NO_TIMER, NO_ADC}, /* PE7 */
                      {GPIOE, NULL, NULL,  8, NO_TIMER, NO_ADC}, /* PE8 */
                      {GPIOE, NULL, NULL,  9, NO_TIMER, NO_ADC}, /* PE9 */
                      {GPIOE, NULL, NULL, 10, NO_TIMER, NO_ADC}, /* PE10 */
                      {GPIOE, NULL, NULL, 11, NO_TIMER, NO_ADC}, /* PE11 */
                      {GPIOE, NULL, NULL, 12, NO_TIMER, NO_ADC}, /* PE12 */
                      {GPIOE, NULL, NULL, 13, NO_TIMER, NO_ADC}, /* PE12 */
                      {GPIOE, NULL, NULL, 14, NO_TIMER, NO_ADC}, /* PE12 */
                      {GPIOE, NULL, NULL, 15, NO_TIMER, NO_ADC}, /* PE12 */

                      {GPIOF, NULL, NULL,  0, NO_TIMER, NO_ADC}, /* PA0 */
                      {GPIOF, NULL, NULL,  1, NO_TIMER, NO_ADC}, /* PA1 */
                      {GPIOF, NULL, NULL,  2, NO_TIMER, NO_ADC}, /* PA2 */
                      {GPIOF, NULL, NULL,  3, NO_TIMER, NO_ADC}, /* PA3 */
                      {GPIOF, NULL, NULL,  4, NO_TIMER, NO_ADC}, /* PA4 */
                      {GPIOF, NULL, NULL,  5, NO_TIMER, NO_ADC}, /* PA5 */
                      {GPIOF, NULL, NULL,  6, NO_TIMER, NO_ADC}, /* PA6 */
                      {GPIOF, NULL, NULL,  7, NO_TIMER, NO_ADC}, /* PA7 */
                      {GPIOF, NULL, NULL,  8, NO_TIMER, NO_ADC}, /* PA8 */
                      {GPIOF, NULL, NULL,  9, NO_TIMER, NO_ADC}, /* PA9 */
                      {GPIOF, NULL, NULL, 10, NO_TIMER, NO_ADC}, /* PA10 */
                      {GPIOF, NULL, NULL, 11, NO_TIMER, NO_ADC}, /* PA11 */
                      {GPIOF, NULL, NULL, 12, NO_TIMER, NO_ADC}, /* PA12 */
                      {GPIOF, NULL, NULL, 13, NO_TIMER, NO_ADC}, /* PA12 */
                      {GPIOF, NULL, NULL, 14, NO_TIMER, NO_ADC}, /* PA12 */
                      {GPIOF, NULL, NULL, 15, NO_TIMER, NO_ADC}, /* PA12 */

                      {GPIOG, NULL, NULL,  0, NO_TIMER, NO_ADC}, /* PA0 */
                      {GPIOG, NULL, NULL,  1, NO_TIMER, NO_ADC}, /* PA1 */
                      {GPIOG, NULL, NULL,  2, NO_TIMER, NO_ADC}, /* PA2 */
                      {GPIOG, NULL, NULL,  3, NO_TIMER, NO_ADC}, /* PA3 */
                      {GPIOG, NULL, NULL,  4, NO_TIMER, NO_ADC}, /* PA4 */
                      {GPIOG, NULL, NULL,  5, NO_TIMER, NO_ADC}, /* PA5 */
                      {GPIOG, NULL, NULL,  6, NO_TIMER, NO_ADC}, /* PA6 */
                      {GPIOG, NULL, NULL,  7, NO_TIMER, NO_ADC}, /* PA7 */
                      {GPIOG, NULL, NULL,  8, NO_TIMER, NO_ADC}, /* PA8 */
                      {GPIOG, NULL, NULL,  9, NO_TIMER, NO_ADC}, /* PA9 */
                      {GPIOG, NULL, NULL, 10, NO_TIMER, NO_ADC}, /* PA10 */
                      {GPIOG, NULL, NULL, 11, NO_TIMER, NO_ADC}, /* PA11 */
                      {GPIOG, NULL, NULL, 12, NO_TIMER, NO_ADC}, /* PA12 */
                      {GPIOG, NULL, NULL, 13, NO_TIMER, NO_ADC}, /* PA12 */
                      {GPIOG, NULL, NULL, 14, NO_TIMER, NO_ADC}, /* PA12 */
                      {GPIOG, NULL, NULL, 15, NO_TIMER, NO_ADC} /* PA12 */
      };
      #elif defined(STM32F4XX)
      extern const STM32GPIO::Pin PINS[STM32_GPIO_COUNT] = {
                      {GPIOA, TIM5, ADC1,  0, 1,    0}, /* PA0 */
                      {GPIOA, TIM5, ADC1,  1, 2,    1}, /* PA1 */
                      {GPIOA, TIM5, ADC1,  2, 3,    2}, /* PA2 */
                      {GPIOA, TIM5, ADC1,  3, 4,    3}, /* PA3 */
                      {GPIOA, NULL, ADC1,  4, 0,    4}, /* PA4 */
                      {GPIOA, NULL, ADC1,  5, 0,    5}, /* PA5 */
                      {GPIOA, NULL, ADC1,  6, 0,    6}, /* PA6 */
                      {GPIOA, NULL, ADC1,  7, 0,    7}, /* PA7 */
                      {GPIOA, NULL, NULL,  8, 0, 0xFF}, /* PA8 */
                      {GPIOA, NULL, NULL,  9, 0, 0xFF}, /* PA9 */
                      {GPIOA, NULL, NULL, 10, 0, 0xFF}, /* PA10 */
                      {GPIOA, NULL, NULL, 11, 0, 0xFF}, /* PA11 */
                      {GPIOA, NULL, NULL, 12, 0, 0xFF}, /* PA12 */
                      {GPIOA, NULL, NULL, 13, 0, 0xFF}, /* PA12 */
                      {GPIOA, NULL, NULL, 14, 0, 0xFF}, /* PA12 */
                      {GPIOA, TIMER1, NULL, 15, 0, 0xFF}, /* PA12 */



                      //


      };
      #endif

      STM32GPIO io;
    }
  }
}
