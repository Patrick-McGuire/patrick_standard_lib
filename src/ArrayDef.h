#ifndef PATRICK_STANDARD_LIB_ARRAYDEF_H
#define PATRICK_STANDARD_LIB_ARRAYDEF_H

#include "psl.h"
#include <cwchar>
#include "Array.h"

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

#endif //PATRICK_STANDARD_LIB_ARRAYDEF_H
