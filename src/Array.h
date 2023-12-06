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

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
////        Implementation
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
namespace psl {
    template<typename T>
    ArrayBase<T>::ArrayBase(T *buff, t_size &length, t_size maxLength): m_buff(buff), m_length(length), m_maxLength(maxLength) {

    }

    template<typename T>
    t_size ArrayBase<T>::length() const {
        return m_length;
    }

    template<typename T>
    t_size ArrayBase<T>::maxLength() const {
        return m_maxLength;
    }

    template<typename T>
    t_size ArrayBase<T>::availableLength() const {
        return m_maxLength - m_length;
    }

    template<typename T>
    bool ArrayBase<T>::validIndex(t_size i) const {
        return i >= 0 && i < m_length;
    }

    template<typename T>
    bool ArrayBase<T>::full() const {
        return m_length == m_maxLength;
    }

    template<typename T>
    bool ArrayBase<T>::empty() const {
        return m_length == 0;
    }

    template<typename T>
    T &ArrayBase<T>::operator[](t_size i) {
        return m_buff[i];
    }

    template<typename T>
    const T &ArrayBase<T>::operator[](t_size i) const {
        return m_buff[i];
    }

    template<typename T>
    T &ArrayBase<T>::getSafe(t_index i, T &defaultVal) {
        return validIndex(i) ? m_buff[i] : defaultVal;
    }

    template<typename T>
    const T &ArrayBase<T>::getSafe(t_index i, T &defaultVal) const {
        return validIndex(i) ? m_buff[i] : defaultVal;
    }

    template<typename T>
    T *ArrayBase<T>::data() const {
        return m_buff;
    }

    template<typename T>
    bool ArrayBase<T>::append(const T &value) {
        m_buff[m_length++] = value;
        return true;
    }

    template<typename T>
    bool ArrayBase<T>::appendSafe(const T &value) {
        return !full() && append(value);
    }

    template<typename T>
    bool ArrayBase<T>::append(const ArrayBase<T> &value) {
        for(int i = 0; i < value.m_length; i++) {
            m_buff[m_length++] = value[i];
        }
        return true;
    }

    template<typename T>
    bool ArrayBase<T>::appendSafe(const ArrayBase<T> &value) {
        for(int i = 0; i < value.m_length; i++) {
            if (!appendSafe(value[i])) {
                return false;
            }
        }
        return true;
    }

    template<typename T>
    template<t_size num>
    bool ArrayBase<T>::append(const T (&startData)[num]) {
        for (const T &el: startData) append(el);
        return true;
    }

    template<typename T>
    template<t_size num>
    bool ArrayBase<T>::appendSafe(const T (&startData)[num]) {
        for (const T &el: startData) {
            if (!appendSafe(el)) {
                return false;
            }
        }
        return true;
    }

    template<typename T>
    bool ArrayBase<T>::insert(t_size i, const T &value) {
        memmove(&m_buff[i + 1], &m_buff[i], sizeof(T) * (m_length - i));
        m_buff[i] = value;
        m_length++;
        return true;
    }

    template<typename T>
    bool ArrayBase<T>::insertSafe(t_size i, const T &value) {
        return (!full() && validIndex(i)) && insert(i, value);
    }

    template<typename T>
    bool ArrayBase<T>::remove(t_size i) {
        memmove(&m_buff[i], &m_buff[i + 1], sizeof(T) * (m_length - i - 1));
        m_length--;
        return true;
    }

    template<typename T>
    bool ArrayBase<T>::removeSafe(t_size i) {
        return (!empty() && validIndex(i)) && remove(i);
    }

    template<typename T>
    bool ArrayBase<T>::pop() {
        m_length--;
        return true;
    }

    template<typename T>
    bool ArrayBase<T>::popSafe() {
        return !empty() && pop();
    }

    template<typename T>
    void ArrayBase<T>::clear() {
        m_length = 0;
    }

    template<typename T>
    ArrayBase<T> &ArrayBase<T>::operator=(const ArrayBase &other) {
        if (&other != this) {
            clear();
            append(other);
        }
        return *this;
    }

    template<typename T>
    template<typename T2>
    ArrayBase<T> &ArrayBase<T>::operator=(const T2 &other) {
        clear();
        append(other);
        return *this;
    }
}

#endif //PATRICK_STANDARD_LIB_ARRAY_H
