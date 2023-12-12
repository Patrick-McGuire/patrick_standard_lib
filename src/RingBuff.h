#ifndef PATRICK_STANDARD_LIB_RINGBUFF_H
#define PATRICK_STANDARD_LIB_RINGBUFF_H

#include "psl.h"

namespace psl {
    template<typename T>
    class RingBuffBase {
    protected:
        template<typename T2, unsigned N> friend
        class RingBuff;

        template<typename T2> friend
        class RingBuffRef;

        const t_size m_maxLength;
        t_size &m_start;
        t_size &m_length;
        T *m_buff = nullptr;

        // Constructors
        RingBuffBase(const RingBuffBase &other) = default;
        RingBuffBase(T *buff, t_size &length, t_size &m_start, t_size maxLength);

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
        T &back();
        const T &back() const;
        T &front();
        const T &front() const;

        // Modifiers
        bool popFirst();
        bool popLast();
        bool pushFirst(const T &val);
        bool pushLast(const T &val);

        // Assigment    @todo decide how/what to implement
//        RingBuffBase &operator=(const RingBuffBase &other);     // Must define the copy assigment operator explicitly
//        template<typename T2>
//        ArrayBase &operator=(const T2 &other);
    };

    template<typename T, unsigned N>
    class RingBuff : public RingBuffBase<T> {
    private:
        t_size m_lengthMemory = 0;
        t_size m_startMemory = 0;
        T m_storage[N];

    public:
        RingBuff() : RingBuffBase<T>(m_storage, m_lengthMemory, m_startMemory, N) {}

        // @todo decide how to implement
//        template<typename T2>
//        RingBuff(const T2 &input) : RingBuffBase<T>(m_storage, m_lengthMemory, N) {
//            append(input);
//        }
    };

    template<typename T>
    class RingBuffRef : public RingBuffBase<T> {
    public:
        template<unsigned N>
        RingBuffRef(RingBuff<T, N> &other) : RingBuffBase<T>(other.m_buff, other.m_length, other.m_start, other.m_maxLength) {}

        RingBuffRef(const RingBuffRef &other) = default;
    };
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
////        Implementation
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
namespace psl {
    template<typename T>
    RingBuffBase<T>::RingBuffBase(T *buff, t_size &length, t_size &start, t_size maxLength)
            : m_buff(buff), m_length(length), m_start(start), m_maxLength(maxLength) {}

    template<typename T>
    t_size RingBuffBase<T>::length() const {
        return m_length;
    }

    template<typename T>
    t_size RingBuffBase<T>::maxLength() const {
        return m_maxLength;
    }

    template<typename T>
    t_size RingBuffBase<T>::availableLength() const {
        return m_maxLength - m_length;
    }

    template<typename T>
    bool RingBuffBase<T>::validIndex(t_size i) const {
        return i >= 0 && i < m_length;
    }

    template<typename T>
    bool RingBuffBase<T>::full() const {
        return m_length == m_maxLength;
    }

    template<typename T>
    bool RingBuffBase<T>::empty() const {
        return m_length == 0;
    }

    template<typename T>
    T &RingBuffBase<T>::operator[](t_size i) {
        return <#initializer#>;
    }

    template<typename T>
    const T &RingBuffBase<T>::operator[](t_size i) const {
        return <#initializer#>;
    }

    template<typename T>
    T &RingBuffBase<T>::back() {
        return <#initializer#>;
    }

    template<typename T>
    const T &RingBuffBase<T>::back() const {
        return <#initializer#>;
    }

    template<typename T>
    T &RingBuffBase<T>::front() {
        return <#initializer#>;
    }

    template<typename T>
    const T &RingBuffBase<T>::front() const {
        return <#initializer#>;
    }

    template<typename T>
    bool RingBuffBase<T>::popFirst() {
        return false;
    }

    template<typename T>
    bool RingBuffBase<T>::popLast() {
        return false;
    }

    template<typename T>
    bool RingBuffBase<T>::pushFirst(const T &val) {
        return false;
    }

    template<typename T>
    bool RingBuffBase<T>::pushLast(const T &val) {
        return false;
    }


}

#endif //PATRICK_STANDARD_LIB_RINGBUFF_H
