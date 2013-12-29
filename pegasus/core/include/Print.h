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

#ifndef PRINT_H_
#define PRINT_H_

namespace pegasus
{
    namespace core
    {
        namespace io
        {

            /*
              Print.h - Base class that provides print() and println()
              Copyright (c) 2008 David A. Mellis.  All right reserved.

              This library is free software; you can redistribute it and/or
              modify it under the terms of the GNU Lesser General Public
              License as published by the Free Software Foundation; either
              version 2.1 of the License, or (at your option) any later version.

              This library is distributed in the hope that it will be useful,
              but WITHOUT ANY WARRANTY; without even the implied warranty of
              MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
              Lesser General Public License for more details.

              You should have received a copy of the GNU Lesser General Public
              License along with this library; if not, write to the Free Software
              Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
            */

            #ifndef Print_h
            #define Print_h

            #include <inttypes.h>
            #include <stdio.h> // for size_t

            enum {
                BYTE = 0,
                BIN  = 2,
                OCT  = 8,
                DEC  = 10,
                HEX  = 16
            };

            class Print {
            public:
                virtual void write(uint8_t ch) = 0;
                virtual void write(const char *str);
                virtual void write(const void *buf, uint32_t len);
                void print(char);
                void print(const char[]);
                void print(uint8_t, int=DEC);
                void print(int, int=DEC);
                void print(unsigned int, int=DEC);
                void print(long, int=DEC);
                void print(unsigned long, int=DEC);
                void print(long long, int=DEC);
                void print(unsigned long long, int=DEC);
                void print(double, int=2);
                void println(void);
                void println(char);
                void println(const char[]);
                void println(uint8_t, int=DEC);
                void println(int, int=DEC);
                void println(unsigned int, int=DEC);
                void println(long, int=DEC);
                void println(unsigned long, int=DEC);
                void println(long long, int=DEC);
                void println(unsigned long long, int=DEC);
                void println(double, int=2);
            private:
                void printNumber(unsigned long long, uint8_t);
                void printFloat(double, uint8_t);
            };

            #endif

        } /* namespace io */
    } /* namespace core */
} /* namespace pegasus */

#endif /* PRINT_H_ */
