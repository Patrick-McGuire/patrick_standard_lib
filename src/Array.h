#ifndef PATRICK_STANDARD_LIB_ARRAY_H
#define PATRICK_STANDARD_LIB_ARRAY_H

#include "psl.h"
#include <cwchar>

namespace psl {
    template<typename T>
    class ArrayBase {
    protected:
        template<typename T2, unsigned N> friend
        class Array;

        template<typename T2> friend
        class ArrayRef;

        const t_size m_maxLength;
        t_size &m_length;
        T *m_buff = nullptr;

        // Constructors
        ArrayBase(const ArrayBase &other) = default;
        ArrayBase(T *buff, t_size &length, t_size maxLength);

    public:
        // Capacity
        t_size length() const;
        t_size maxLength() const;
        t_size availableLength() const;
        bool validIndex(t_size i) const;
        bool full() const;
        bool empty() const;

        // Access
        T &operator[](t_size i);
        const T &operator[](t_size i) const;
        T &getSafe(t_index i, T &defaultVal);
        const T &getSafe(t_index i, T &defaultVal) const;
        T *data() const;

        // Modifiers
        bool append(const T &value);
        bool appendSafe(const T &value);
        bool append(const ArrayBase<T> &value);
        bool appendSafe(const ArrayBase<T> &value);
        template<t_size num>
        bool append(const T (&startData)[num]);
        template<t_size num>
        bool appendSafe(const T (&startData)[num]);
        bool insert(t_size i, const T &value);
        bool insertSafe(t_size i, const T &value);
        bool remove(t_size i);
        bool removeSafe(t_size i);
        bool pop();
        bool popSafe();
        void clear();

        // Assigment
        ArrayBase &operator=(const ArrayBase &other);     // Must define the copy assigment operator explicitly
        template<typename T2>
        ArrayBase &operator=(const T2 &other);
    };


    template<typename T, unsigned N>
    class Array : public ArrayBase<T> {
    private:
        t_size m_lengthMemory = 0;
        T m_storage[N];

    public:
        Array() : ArrayBase<T>(m_storage, m_lengthMemory, N) {}

        template<typename T2>
        Array(const T2 &input) : ArrayBase<T>(m_storage, m_lengthMemory, N) {
            append(input);
        }
    };

    template<typename T>
    class ArrayRef : public ArrayBase<T> {
    public:
        template<unsigned N>
        ArrayRef(Array<T, N> &other) : ArrayBase<T>(other.m_buff, other.m_length, other.m_maxLength) {}

        ArrayRef(const ArrayRef &other) = default;
    };
}


#include "ArrayDef.h"

#endif //PATRICK_STANDARD_LIB_ARRAY_H
