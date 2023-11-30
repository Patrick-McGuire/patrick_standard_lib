#ifndef PATRICK_STANDARD_LIB_MANAGER_H
#define PATRICK_STANDARD_LIB_MANAGER_H

#include "../psl.h"
#include "../Array.h"
#include "../Function.h"
#include "functional"
#include <iostream>
#include <cstring>

#ifndef PSL_PUB_SUB_NAME_SIZE
#define PSL_PUB_SUB_NAME_SIZE 20
#endif

namespace psl {
    struct SubscriberInfo {
        int id = 0;
        std::function<void(void *data, double time)> publishFuntion;

        SubscriberInfo() = default;

        SubscriberInfo(int id_, const std::function<void(void *data, double time)> &publishFuntion_) {
            id = id_;
            publishFuntion = publishFuntion_;
        }
    };

    struct TopicInfo {
        char name[PSL_PUB_SUB_NAME_SIZE + 1] = "";
        int id = 0;
        int firstIndex = -1;

        TopicInfo() = default;

        TopicInfo(const char *name_, int id_) {
            id = id_;
            if (strlen(name_) < 20) {
                strcpy(name, name_);
            } else {
                std::cout << "ERROR\n";
            }
        }
    };


    class PubSubManager {
        int nextID = 0;

        Array<TopicInfo> &topics;
        Array<SubscriberInfo> &subs;

        int getFirstSubscriber(const char *name) {
            for (int i = 0; i < topics.length(); i++) {
                if (strcmp(name, topics[i].name) != 0) {
                    return topics[i].id;
                }
            }
            return -1;
        }

        void calculateFirstSubs() {
            for (int i = 0; i < topics.length(); i++) {
                topics[i].firstIndex = -1;      // Reset, should not be needed but why not
                for (int j = 0; j < subs.length(); j++) {
                    if (subs[j].id == topics[i].id) {
                        topics[i].firstIndex = j;
                        break;
                    }
                }
            }
        }

    public:
        PubSubManager(Array<TopicInfo> &topics_, Array<SubscriberInfo> &subs_) : topics(topics_), subs(subs_) {

        }

        template<unsigned MaxTopics, unsigned MaxSubscribers>
        class size;

        int getTopicID(const char *name) const {
            for (int i = 0; i < topics.length(); i++) {
                if (strcmp(name, topics[i].name) == 0) {
                    return topics[i].id;
                }
            }
            return -1;
        }

        int getAddTopicID(const char *name) {
            int id = getTopicID(name);
            if (id < 0) {
                id = nextID++;
                topics.append({name, id});
            }
            return id;
        }

        const char *getName(int id) {
            for (int i = 0; i < topics.length(); i++) {
                if (topics[i].id == id) {
                    return topics[i].name;
                }
            }
            return "";
        }


        int subscribe(const char *name, const std::function<void(void *data, double)> &onPublish) {
            int id = getAddTopicID(name);
            int firstOfTopic = getFirstSubscriber(name);
            if (firstOfTopic < 0) {
                subs.appendSafe({id, onPublish});
            } else {
                subs.insertSafe(firstOfTopic + 1, {id, onPublish});
            }
            calculateFirstSubs();
            return id;
        }

        void publish(int id, void *data, double time) const {
            int startIndex = topics.getSafe(id).firstIndex;
            if (startIndex >= 0) {
                for (int i = startIndex; i < subs.length() && subs[i].id == id; i++) {
                    subs[i].publishFuntion(data, time);
                }
            }
        }
    };

    template<unsigned MaxTopics, unsigned MaxSubscribers>
    class PubSubManager::size final : public PubSubManager {
    private:
        Array<TopicInfo>::size<10> topicsStorage;
        Array<SubscriberInfo>::size<10> subsStorage;
    public:
        size() : PubSubManager(topicsStorage, subsStorage) {}
    };

    typedef PubSubManager::size<30, 100> Manager;

}


#endif //PATRICK_STANDARD_LIB_MANAGER_H
