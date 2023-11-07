#ifndef PMCGUIRESTANDARD_ARRAY_H
#define PMCGUIRESTANDARD_ARRAY_H

#include "pmcguire.h"

namespace pmcguire {
    template<typename T>
    class ArrayBase {
    protected:
        T *m_elements = nullptr;
        pmcguire::t_size m_length = 0;
        pmcguire::t_size m_maxLength = 0;

    public:
        ArrayBase() = default;

        pmcguire::t_size length() const {
            return m_length;
        }

        pmcguire::t_size maxLength() const {
            return m_maxLength;
        }

        bool full() const {
            return length() >= maxLength();
        }

        bool empty() const {
            return length() <= 0;
        }


        virtual bool append(const T &value) {
            m_elements[m_length] = value;
            m_length++;
            return true;
        }

        virtual bool pop() {
            m_length--;
            return true;
        }

        virtual bool insert(pmcguire::t_size i, const T &value) {
            memmove(&m_elements[i + 1], &m_elements[i], sizeof(T) * (m_length - i));
            m_elements[i] = value;
            m_length++;
            return true;
        }

        virtual bool remove(pmcguire::t_size i) {
            memmove(&m_elements[i], &m_elements[i + 1], sizeof(T) * (m_length - i - 1));
            m_length--;
            return true;
        }

        virtual const T &operator[](pmcguire::t_index i) const {
            return m_elements[i];
        }

        virtual T &operator[](pmcguire::t_index i) {
            return m_elements[i];
        }
    };

    template<typename T>
    class SafeArray : public ArrayBase<T> {
    public:
        SafeArray() : ArrayBase<T>() {}

        bool append(const T &value) final {
            if (!ArrayBase<T>::full()) {
                return ArrayBase<T>::append(value);
            }
            return false;
        }

        bool pop() final {
            if (!ArrayBase<T>::empty()) {
                return ArrayBase<T>::pop();
            }
            return false;
        }

        bool insert(pmcguire::t_size i, const T &value) final {
            if (!ArrayBase<T>::full() && i < ArrayBase<T>::length() + 1 && i > 0) {
                return ArrayBase<T>::insert(i, value);
            }
            return false;
        }

        bool remove(pmcguire::t_size i) final {
            if (!ArrayBase<T>::empty() && i < ArrayBase<T>::length() && i > 0) {
                return ArrayBase<T>::remove(i);
            }
            return false;
        }

        const T &operator[](pmcguire::t_index i) const final {
            if (i < ArrayBase<T>::length() && i > 0) {
                return ArrayBase<T>::operator[](i);
            }
            return ArrayBase<T>::operator[](0);
        }

        T &operator[](pmcguire::t_index i) final {
            if (i < ArrayBase<T>::length() && i > 0) {
                return ArrayBase<T>::operator[](i);
            }
            return ArrayBase<T>::operator[](0);
        }


    };

    template<typename T>
    class Array : public ArrayBase<T> {
    public:
        Array() : ArrayBase<T>() {}

        bool append(const T &value) final {
            return ArrayBase<T>::append(value);
        }

        bool pop() final {
            return ArrayBase<T>::pop();
        }

        bool insert(pmcguire::t_size i, const T &value) final {
            return ArrayBase<T>::insert(i, value);
        }

        bool remove(pmcguire::t_size i) final {
            return ArrayBase<T>::remove(i);
        }

        const T &operator[](pmcguire::t_index i) const final {
            return ArrayBase<T>::operator[](i);
        }

        T &operator[](pmcguire::t_index i) final {
            return ArrayBase<T>::operator[](i);
        }
    };

    template<typename T, unsigned length>
    class ArrayStorage : public Array<T> {
    public:
        T buff[length];

        ArrayStorage() : Array<T>() {
            ArrayBase<T>::m_elements = &buff[0];
            ArrayBase<T>::m_maxLength = length;
        }
    };

    template<typename T, unsigned length>
    class SafeArrayStorage : public SafeArray<T> {
    public:
        T buff[length];

        SafeArrayStorage() : SafeArray<T>() {
            ArrayBase<T>::m_elements = &buff[0];
            ArrayBase<T>::m_maxLength = length;
        }
    };
}

#endif //PMCGUIRESTANDARD_ARRAY_H
