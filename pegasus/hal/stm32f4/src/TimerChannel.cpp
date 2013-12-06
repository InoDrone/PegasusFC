/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Author: Marc Jacquier <marc@inodrone.com>
 *  Project: InoDronePegasus
 */

#include "hal/stm32f4/include/TimerChannel.h"

namespace pegasus {
    namespace hal {
        namespace stm32f4 {

            uint8_t TimerChannel::getUniqId()
            {
                uint8_t pos = 0;
                __IO TIM_TypeDef* reg = _mTimer->getReg();
                if (reg == TIM2) {
                    pos = 1;
                } else if (reg == TIM3) {
                    pos = 2;
                } else if (reg == TIM4) {
                    pos = 3;
                } else if (reg == TIM5) {
                    pos = 4;
                } else if (reg == TIM6) {
                    pos = 5;
                } else if (reg == TIM7) {
                    pos = 6;
                } else if (reg == TIM8) {
                    pos = 7;
                } else if (reg == TIM9) {
                    pos = 8;
                } else if (reg == TIM10) {
                    pos = 9;
                } else {
                    pos = 10;
                }// TODO finish

                return (uint8_t)((pos * 4) + _mChannel);
            }

        }
    }
}
