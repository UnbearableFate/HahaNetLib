#pragma once

#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>
#include <thread>

#include "../Base/Channel.h"
#include "EpollManager.h"
class EventLoop {
   public:
    void loop();
    bool isLooping;
    EventLoop();
    auto addChannel(int fd) -> std::shared_ptr<Channel>;
    void addPendingFunc(std::function<void(void)>);
    void removeChannel(int fd);
    void startLoop();

   private:
    std::map<int, std::shared_ptr<Channel>> channels;
    bool used = false;
    EpollManager epollMgr;
    std::mutex mtx = std::mutex();
    std::vector<std::function<void(void)>> pendingFuncList = std::vector<std::function<void(void)>>();  // mutex
    EventLoop(EventLoop&) = delete;
    EventLoop& operator=(const EventLoop&) = delete;
    std::shared_ptr<Channel> notifiChannel;
};
