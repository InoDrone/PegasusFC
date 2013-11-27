/*
 * Pipe.h
 *
 *  Created on: 25 nov. 2013
 *      Author: alienx
 */

#ifndef PIPE_H_
#define PIPE_H_

#include <stdint.h>

namespace pegasus {
    namespace core {

        template<class Obj_t, uint8_t Size_t = 128>
        class Pipe {
            public:
                Pipe();

                bool push(Obj_t* obj);
                bool remove(Obj_t* obj);

                uint16_t count() const;
                uint16_t size() const;

                uint16_t find(Obj_t* obj);

                Obj_t* volatile *begin();
                Obj_t* volatile *end();

                Obj_t* pop();

                Obj_t* operator[](uint16_t index);


            private:
                Obj_t* volatile _mObjs[Size_t];
                uint16_t volatile _mCount;

        };

        template<class Obj_t, uint8_t Size_t>
        Pipe<Obj_t,Size_t>::Pipe()
        {
            _mCount = 0;
            for (uint16_t i=0;i<size();++i) {
                _mObjs[i] = nullptr;
            }
        }

        template<class Obj_t, uint8_t Size_t>
        uint16_t Pipe<Obj_t,Size_t>::find(Obj_t* search)
        {
            for (uint16_t i=0;i<size();++i) {
                if (_mObjs[i] == search ) {
                    return i;
                }
            }

            return -1;
        }

        template<class Obj_t, uint8_t Size_t>
        bool Pipe<Obj_t,Size_t>::push(Obj_t* obj)
        {
                if (find(obj) != -1) {
                    return false;
                }

                if (_mCount >= size()) {
                    return false;
                }

                _mObjs[_mCount++] = obj;
        }

        template<class Obj_t, uint8_t Size_t>
        bool Pipe<Obj_t,Size_t>::remove(Obj_t* obj)
        {
                uint16_t i = find(obj);
                if (i == -1) {
                    return false;
                }

                if (_mCount >= size()) {
                    return false;
                }

                // Move elements
                for(; i < _mCount; ++i) {
                    _mObjs[i] = _mObjs[i+1];
                }
                _mCount--;
               // _mObjs[_mCount] = nullptr;
        }


        template<class Obj_t, uint8_t Size_t>
        inline uint16_t Pipe<Obj_t,Size_t>::count() const
        {
            return _mCount;
        }

        template<class Obj_t, uint8_t Size_t>
        inline uint16_t Pipe<Obj_t,Size_t>::size() const
        {
            return sizeof(_mObjs) / sizeof(_mObjs[0]);
        }

        template<class Obj_t, uint8_t Size_t>
        inline Obj_t* Pipe<Obj_t,Size_t>::operator[](uint16_t index)
        {
            return _mObjs[index];
        }

        template<class Obj_t, uint8_t Size_t>
        inline Obj_t* volatile *
        Pipe<Obj_t,Size_t>::begin()
        {
            return &_mObjs[0];
        }

        template<class Obj_t, uint8_t Size_t>
        inline Obj_t* volatile *
        Pipe<Obj_t,Size_t>::end()
        {
            return &_mObjs[_mCount];
        }

        template<class Obj_t, uint8_t Size_t>
        inline Obj_t*
        Pipe<Obj_t,Size_t>::pop()
        {
            return _mObjs[0];
        }

    }
}


#endif /* PIPE_H_ */
