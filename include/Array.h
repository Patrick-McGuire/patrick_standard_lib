#ifndef PMCGUIRESTANDARD_ARRAY_H
#define PMCGUIRESTANDARD_ARRAY_H

#include "pmcguire.h"

namespace pmcguire {
    template<typename T>
    class Array {
    private:
        T *elements = nullptr;
        pmcguire::size_t length = 0;

    public:
        Array() = default;

        virtual T &operator[](pmcguire::index_t i) {
            return elements[i];
        }

//        bool insert(const T &element, pmcguire::index_t index) {
//
//        }
    };

    template<typename T, unsigned length>
    class ArraySafe : public Array<T> {
    public:
        T &operator[](pmcguire::index_t i) override {
            if(i < length) {

            }
            return Array<T>::operator[](i);
        }
//        T buff[length];
//
//        ArraySafe() : Array<T>() {
//            Array<T>::elements = &buff;
//            Array<T>::length = length
//        }
    };


    template<typename T, unsigned length>
    class ArrayStorage : public Array<T> {
    public:
        T buff[length];

        ArrayStorage() : Array<T>() {
            Array<T>::elements = &buff;
            Array<T>::length = length
        }
    };

}

#endif //PMCGUIRESTANDARD_ARRAY_H
