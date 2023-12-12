
#ifndef PATRICK_STANDARD_LIB_SUBSCRIBER_H
#define PATRICK_STANDARD_LIB_SUBSCRIBER_H

#include "Manager.h"
#include "../RingBuffer2.h"
#include "functional"

namespace psl {
    template<typename T, unsigned BufferSize>
    class Subscriber {
    private:
        struct TimeSeries {
            T t;
            double time;

            TimeSeries(const T &t_, double time_) {
                t = t_;
                time = time_;
            }

            TimeSeries() {

            }
        };

        int m_id;
        const char *m_name;
        PubSubManager &m_manager;
        Ringbuffer2<TimeSeries, BufferSize> buff;
        std::function<void(T &, double)> userFunc;

        void onNewData(void *data, double time) {
            if (buff.full())
                buff.pop_front();
            buff.push_back({*((T *) data), time});
            if (userFunc)
                userFunc(*((T *) data), time);
        }

    public:
        Subscriber(PubSubManager &manager, const char *name) : m_manager(manager) {
            m_id = m_manager.subscribe(name, [this](void *data, double time) { this->onNewData(data, time); });
            m_name = m_manager.getName(m_id);
        }

        void setCallback(std::function<void(T &, double )> userFunc_) {
            userFunc = userFunc_;
        }

        bool available() {
            return !buff.empty();
        }

        const T &peek() {
            return buff.front().t;
        }

        double peekTime() {
            return buff.front().time;
        }

        void pop() {
            buff.pop_front();
        }
    };

} // psl

#endif //PATRICK_STANDARD_LIB_SUBSCRIBER_H
