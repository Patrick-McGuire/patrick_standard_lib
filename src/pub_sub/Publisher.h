#ifndef PATRICK_STANDARD_LIB_PUBLISHER_H
#define PATRICK_STANDARD_LIB_PUBLISHER_H

#include "Manager.h"
#include "../Function.h"

namespace psl {

    template<typename T>
    class Publisher {
    private:
        int m_id;
        const char *m_name;
        PubSubManager &m_manager;

    public:
        Publisher(PubSubManager &manager, const char *name) : m_manager(manager) {
            m_id = m_manager.getAddTopicID(name);
            m_name = m_manager.getName(m_id);
        }

        void publish(T t, double time = -1) {
            m_manager.publish(m_id, &t, time);
        }

    };

} // psl

#endif //PATRICK_STANDARD_LIB_PUBLISHER_H
