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
        inline t_size length() const;
        inline t_size maxLength() const;
        inline t_size availableLength() const;
        inline bool validIndex(t_size i) const;
        inline bool full() const;
        inline bool empty() const;

        // Access
        T &operator[](t_size i);
        const T &operator[](t_size i) const;
        T *data() const;

        // Modifiers
        bool append(const T &value);
        bool append(const ArrayBase<T> &value);
        template<t_size num>
        bool append(const T (&startData)[num]);
        bool insert(t_size i, const T &value);
        bool remove(t_size i);
        bool pop();
        void clear();

        // Assigment
        ArrayBase &operator=(const ArrayBase &other);     // Must define the copy assigment operator explicitly
        template<typename T2>
        ArrayBase &operator=(const T2 &other);
        template<t_size num>
        ArrayBase &operator=(const T (&other)[num]);
    };

    template<typename T>
    class ArrayRef;

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

        template<t_size num>
        Array(const T (&input)[num]) : ArrayBase<T>(m_storage, m_lengthMemory, N) {
            ArrayBase<T>::append(input);
        }

        using ArrayBase<T>::operator=;
    };

    template<typename T>
    class ArrayRef : public ArrayBase<T> {
    public:
        template<unsigned N>
        ArrayRef(Array<T, N> &other) : ArrayBase<T>(other.m_buff, other.m_length, other.m_maxLength) {}

        ArrayRef(const ArrayRef &other) = default;

        using ArrayBase<T>::operator=;
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
//        return validIndex(i) ? m_buff[i] : defaultVal;   @todo implement
    }

    template<typename T>
    T *ArrayBase<T>::data() const {
        return m_buff;
    }

    template<typename T>
    bool ArrayBase<T>::append(const T &value) {
        if (!full()) {
            m_buff[m_length++] = value;
            return true;
        }
        return false;
    }

    template<typename T>
    bool ArrayBase<T>::append(const ArrayBase<T> &value) {
        for (int i = 0; i < value.m_length && !full(); i++) {
            m_buff[m_length++] = value[i];
        }
        return true;
    }

    template<typename T>
    template<t_size num>
    bool ArrayBase<T>::append(const T (&startData)[num]) {
        for (const T &el: startData) {
            if (full())
                return false;
            m_buff[m_length++] = el;
        }
        return true;
    }

    template<typename T>
    bool ArrayBase<T>::insert(t_size i, const T &value) {
        if (!full() && validIndex(i)) {
            memmove(&m_buff[i + 1], &m_buff[i], sizeof(T) * (m_length - i));
            m_buff[i] = value;
            m_length++;
            return true;
        }
        return false;
    }

    template<typename T>
    bool ArrayBase<T>::remove(t_size i) {
        if (!empty() && validIndex(i)) {
            memmove(&m_buff[i], &m_buff[i + 1], sizeof(T) * (m_length - i - 1));
            m_length--;
            return true;
        }
        return false;
    }

    template<typename T>
    bool ArrayBase<T>::pop() {
        if (!empty()) {
            m_length--;
            return true;
        }
        return false;
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

    template<typename T>
    template<t_size num>
    ArrayBase<T> &ArrayBase<T>::operator=(const T (&other)[num]) {
        clear();
        append(other);
        return *this;
    }
}

#endif //PATRICK_STANDARD_LIB_ARRAY_H
