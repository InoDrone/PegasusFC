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

#ifndef PWMLISTENER_H_
#define PWMLISTENER_H_


namespace pegasus {
    namespace hal {

        class PWMListener {
            public:
                virtual void pwmUpdate(uint32_t pulseTime) = 0;
                static void trampoline(PWMListener* listener, uint32_t pulseTime) {
                    listener->pwmUpdate(pulseTime);
                }

        };
    }
}


#endif /* PWMLISTENER_H_ */
