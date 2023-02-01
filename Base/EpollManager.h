#pragma once
#include <vector>

#include "../Base/Channel.h"
class EpollManager {
   private:
    int eventsFd;

   public:
    EpollManager();
    ~EpollManager();
    void addEvent(Channel*);
    void modifyEvent(Channel*);
    void deleteEvent(Channel*);
    auto pollWait() -> std::vector<Channel*>;
};
