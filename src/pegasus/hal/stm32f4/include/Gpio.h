/*
 * STM32GPIO.h
 *
 *  Created on: 6 nov. 2013
 *      Author: alienx
 */

#ifndef PEGASUSHALSTM32GPIO_H_
#define PEGASUSHALSTM32GPIO_H_

#include "pegasus/hal/stm32f4/include/Defs.h"
#include "pegasus/hal/include/GpioBase.h"

#define Port2Pin(port, bit) ((uint8_t)((port-'A')*16+bit))

namespace pegasus {

    namespace gpio {
        enum class AlternateFunction : uint32_t {
            AF0 = 0,
            AF1,
            AF2,
            AF3,
            AF4,
            AF5,
            AF6,
            AF7,
            AF8,
            AF9,
            AF10,
            AF11,
            AF12,
            AF13,
            AF14,
            AF15

        };

        enum Speed {
            LOW_2MHz   = 0x00,
            MEDIUM_25MHz  = 0x01,
            FAST_50MHz  = 0x02,
            HIGH_100MHz = 0x03
        };

        enum PuPd {
            NO_PULL    = 0x00,
            PULL_UP    = 0x01,
            PULL_DOWN  = 0x02
        };

        enum Port {
            A = 0,
            B,
            C,
            D,
            E,
            F,
            G,
            H,
            I,
            J,
            K
        };

        enum Pin {
            PIN0,
            PIN1,
            PIN2,
            PIN3,
            PIN4,
            PIN5,
            PIN6,
            PIN7,
            PIN8,
            PIN9,
            PIN10,
            PIN11,
            PIN12,
            PIN13,
            PIN14,
            PIN15

        };
    }


  namespace hal {
    namespace stm32f4 {

      class Gpio: public pegasus::GpioBase {
          public:

              Gpio (const gpio::portNumber_t, const gpio::bitNumber_t);
              void mode(const pegasus::gpio::Mode mode);
              void low(void);
              void high(void);
              void toggle(void);
              bool read(void);

              void setAlternateFunction(const pegasus::gpio::AlternateFunction);
          private:
              GPIO_TypeDef* _mReg;
      };


      inline Gpio::Gpio(gpio::portNumber_t portNumber, gpio::bitNumber_t bitNumber) :
              GpioBase(portNumber, bitNumber),
              _mReg(reinterpret_cast<GPIO_TypeDef*>(GPIOA_BASE + (portNumber * (GPIOB_BASE - GPIOA_BASE))))
      {
         // Enable RCC Clock for port if necessary
          if ( (RCC->AHB1ENR & _BIT(portNumber)) == 0) {
              RCC->AHB1ENR |= _BIT(portNumber);
          }
      }

      INLINE void Gpio::mode(pegasus::gpio::Mode mode)
      {
          uint32_t moder   = _mReg->MODER;
          uint32_t ospeedr = _mReg->OSPEEDR;
          uint32_t otyper  = _mReg->OTYPER;
          uint32_t pupdr   = _mReg->PUPDR;

          moder    &= ~(GPIO_MODER_MODER0 << (_mBitNumber * 2));
          ospeedr  &= ~(GPIO_OSPEEDER_OSPEEDR0  << (_mBitNumber * 2));
          otyper   &= ~(_mBitMask);
          pupdr    &= ~(GPIO_PUPDR_PUPDR0  << (_mBitNumber * 2));


          switch (mode) {
              case gpio::Mode::OUTPUT: // Classic Ouput
                  moder   |= (0x01 << (_mBitNumber * 2));
                  ospeedr |= (gpio::Speed::FAST_50MHz << (_mBitNumber * 2));
                  // Output Type PP
                  // PuPd NOPULL
                  break;
              case gpio::Mode::OUTPUT_OD:
                  moder   |= (0x01 << (_mBitNumber * 2));
                  ospeedr |= (gpio::Speed::FAST_50MHz << (_mBitNumber * 2));
                  otyper  |= _mBitMask;
                  // PuPd NOPULL
                  break;
              case gpio::Mode::INPUT:
              case gpio::Mode::INPUT_FLOATING:
                  moder |= (0x00 << (_mBitNumber * 2));
                  ospeedr |= (gpio::Speed::FAST_50MHz << (_mBitNumber * 2));
                  // Input Type PP
                  // PuPd NOPULL
                  break;
              case gpio::Mode::INPUT_PU:
                  moder   |= (0x00 << (_mBitNumber * 2));
                  ospeedr |= (gpio::Speed::FAST_50MHz << (_mBitNumber * 2));
                  // Input Type PP
                  pupdr   |= (gpio::PuPd::PULL_UP  << (_mBitNumber * 2));
                  break;
              case gpio::Mode::INPUT_PD:
                  moder |= (0x00 << (_mBitNumber * 2));
                  ospeedr |= (gpio::Speed::FAST_50MHz << (_mBitNumber * 2));
                  // Input Type PD
                  pupdr   |= (gpio::PuPd::PULL_DOWN  << (_mBitNumber * 2));
                  break;
              case gpio::Mode::PWM:
                  moder |= (0x02 << (_mBitNumber * 2));
                  ospeedr |= (gpio::Speed::FAST_50MHz << (_mBitNumber * 2));
                  // Input Type PP
                  pupdr   |= (gpio::PuPd::PULL_UP  << (_mBitNumber * 2));
                  break;
              case gpio::Mode::INPUT_ANALOG:
                  break;
          }

          _mReg->MODER   = moder;
          _mReg->OSPEEDR = ospeedr;
          _mReg->OTYPER  = otyper;
          _mReg->PUPDR   = pupdr;
      }

      INLINE void Gpio::high(void)
      {
          _mReg->BSRRL = _mBitMask;
      }

      INLINE void Gpio::low(void)
      {
          _mReg->BSRRH = _mBitMask;
      }

      INLINE void Gpio::toggle(void)
      {
          _mReg->ODR ^= _mBitMask;
      }

      INLINE bool Gpio::read(void)
      {
          return _mReg->IDR & _mBitMask;
      }

      INLINE void Gpio::setAlternateFunction(pegasus::gpio::AlternateFunction af)
      {
          uint32_t current = _mReg->AFR[_mBitNumber >> 0x03];
          current &= ~((uint32_t)0xF << ((uint32_t)((uint32_t)_mBitNumber & (uint32_t)0x07) * 4)); // reset pin AF
          current |=  ((uint32_t)af << ((uint32_t)((uint32_t)_mBitNumber & (uint32_t)0x07) * 4)); // Set AF mode

          _mReg->AFR[_mBitNumber >> 0x03] = current; // Save to register
      }


//      /extern STM32GPIO io;

      //extern const STM32GPIO::Pin PINS[];
    }
  }
}



#endif /* PEGASUSHALSTM32GPIO_H_ */
