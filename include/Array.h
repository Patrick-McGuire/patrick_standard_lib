#ifndef PATRICK_STANDARD_LIB_ARRAY_H
#define PATRICK_STANDARD_LIB_ARRAY_H

#include "psl.h"

namespace psl {
    template<typename T>
    class Array {
    protected:
        struct ArrayData {
            T *buff;
            t_size length;
            t_size maxLength;
        };
        ArrayData &arrayData;

        bool validIndex(t_size i) {
            return i < length() + 1 && i > 0;
        }


    public:
        // Class that has storage, will extend this
        template<unsigned N>
        class size;

        explicit Array(ArrayData &arrayData1) : arrayData(arrayData1) {}

        Array(Array &other) : arrayData(other.arrayData) {}

        template<unsigned N>
        Array(size<N> other);

        t_size length() const {
            return arrayData.length;
        }

        t_size maxLength() const {
            return arrayData.maxLength;
        }

        bool full() const {
            return length() >= maxLength();
        }

        bool empty() const {
            return length() <= 0;
        }


        bool append(const T &value) {
            arrayData.buff[arrayData.length] = value;
            arrayData.length++;
            return true;
        }

        bool appendSafe(const T &value) {
            return !full() && append(value);
        }

        template<t_size num>
        bool append(const T (&startData)[num]) {
            for (const T &el: startData) append(el);
            return true;
        }

        template<t_size num>
        bool appendSafe(const T (&startData)[num]) {
            for (const T &el: startData) {
                if (!appendSafe(el)) {
                    return false;
                }
            }
            return true;
        }

        bool pop() {
            arrayData.length--;
            return true;
        }

        bool popSafe() {
            return !empty() && pop();
        }

        bool insert(t_size i, const T &value) {
            memmove(&arrayData.buff[i + 1], &arrayData.buff[i], sizeof(T) * (arrayData.length - i));
            arrayData.buff[i] = value;
            arrayData.length++;
            return true;
        }

        bool insertSafe(t_size i, const T &value) {
            return (!full() && validIndex(i)) && insert(i, value);
        }

        bool remove(t_size i) {
            memmove(&arrayData.buff[i], &arrayData.buff[i + 1], sizeof(T) * (arrayData.length - i - 1));
            arrayData.length--;
            return true;
        }

        bool removeSafe(t_size i) {
            return (!empty() && validIndex(i)) && remove(i);
        }

        const T &operator[](t_index i) const {
            return arrayData.buff[i];
        }

        T &operator[](t_index i) {
            return arrayData.buff[i];
        }

        T &getSafe(t_index i) {
            return validIndex(i) ? arrayData.buff[i] : arrayData.buff[0];
        }

        const T &getSafe(t_index i) const {
            return getSafe(i);
        }
    };

    /**
     * Array storage class
     * @brief Version of array that has it's own storage
     *
     * @tparam T Type of data to store in array
     * @tparam N Maximum number of elements
     */
    template<typename T>
    template<unsigned N>
    class Array<T>::size final : public Array<T> {
    private:
        T buff[N];
        ArrayData arrayDataNonRef = {buff, 0, N};
    public:
        size() : Array<T>(arrayDataNonRef) {}

        template<t_size num>
        explicit size(const T (&startData)[num]) : Array<T>(arrayDataNonRef) {
            append(startData);
        }

    };

    template<typename T>
    template<unsigned int N>
    Array<T>::Array(Array::size<N> other) : arrayData(other.arrayData) {}
}

#endif //PATRICK_STANDARD_LIB_ARRAY_H
