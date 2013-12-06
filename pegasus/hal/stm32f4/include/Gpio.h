/*
 * STM32GPIO.h
 *
 *  Created on: 6 nov. 2013
 *      Author: alienx
 */

#ifndef PEGASUSHALSTM32GPIO_H_
#define PEGASUSHALSTM32GPIO_H_

#include "hal/stm32f4/include/Defs.h"
#include "hal/include/GpioBase.h"

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

        namespace gpio = pegasus::gpio;

      class Gpio: public pegasus::hal::GpioBase {
          public:

              Gpio (const gpio::portNumber_t, const gpio::bitNumber_t);
              void mode(const pegasus::gpio::Mode mode);
              void low(void);
              void high(void);
              void toggle(void);
              bool read(void);

              gpio::portNumber_t getPort() const;
              gpio::bitNumber_t getBit() const;

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

      INLINE gpio::portNumber_t Gpio::getPort() const
      {
          return _mPortNumber;
      }

      INLINE gpio::bitNumber_t Gpio::getBit() const
      {
          return _mBitNumber;
      }


//      /extern STM32GPIO io;

      //extern const STM32GPIO::Pin PINS[];
    }
  }
}



#endif /* PEGASUSHALSTM32GPIO_H_ */
