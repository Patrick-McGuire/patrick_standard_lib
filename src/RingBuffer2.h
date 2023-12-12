#ifndef TIDERIDERARDUINOV2_RINGBUFFER_H
#define TIDERIDERARDUINOV2_RINGBUFFER_H

namespace psl {
    template<typename T, unsigned N>
    class Ringbuffer2 {
    public:
        explicit Ringbuffer2() = default;

        const T &get(int index) {
            return m_buff[(m_start + index) % m_maxSize];
        }

        const T &operator[](int index) {
            return get(index);
        }

        int size() {
            return m_size;
        }

        int maxSize() {
            return m_maxSize;
        }

        bool full() {
            return size() == m_maxSize;
        }

        bool empty() {
            return size() <= 0;
        }

        bool pop_back() {
            if (empty()) return false;
            // Remove element
//            m_buff[m_start] = 0;
            m_start++;
            if (m_start == m_maxSize) m_start = 0;
            m_size--;
            return true;
        }

        bool pop_front() {
            if (empty()) return false;
            // Remove element
//            m_buff[(m_start + m_size - 1) % m_maxSize] = 0;
            m_size--;
            return true;
        }

        bool push_back(const T &val) {
            if (full()) {
                return false;
            }
            // Add element
            m_start--;
            if (m_start < 0) m_start = m_maxSize - 1;
            m_buff[m_start] = val;
            m_size++;
            return true;
        }

        bool push_front(const T &val) {
            if (full()) {
                return false;
            }
            // Add element
            m_size++;
            m_buff[(m_start + m_size - 1) % m_maxSize] = val;
            return true;
        }

        const T &back() {
            return m_buff[m_start];
        }

        const T &front() {
            return m_buff[(m_start + m_size - 1) % m_maxSize];
        }

//        void print() {
//            if (size() != 0) {
//                int pos = m_start;
//                for (int i = 0; i < size(); i++) {
//                    TideRiderHAL::consolePrint(get(i, false));
//                }
//            }
//            TideRiderHAL::consolePrint("");
//        }

    private:
        int m_maxSize = N;
        int m_start = 0;
        int m_size = 0;
        T m_buff[N];

    }; // pmcguire
}
#endif //TIDERIDERARDUINOV2_RINGBUFFER_H
