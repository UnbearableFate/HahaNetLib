#include "EventLoop.h"

#include <errno.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>
auto EventLoop::addChannel(int fd) -> std::shared_ptr<Channel> {
    this->channels.try_emplace(fd, std::make_shared<Channel>(fd));
    auto ch = this->channels[fd];
    this->epollMgr.addEvent(ch.get());
    return ch;
}

EventLoop::EventLoop() : epollMgr() {
    notifiChannel = this->addChannel(eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK));
    notifiChannel->setReadCallback([&]() {
        uint64_t one = 1;
        ssize_t n = read(notifiChannel->getFd(), &one, sizeof one);
        if (n != sizeof one) {
            printf("notified\n");
        }
    });
    notifiChannel->enableEvent(Channel::READ_EVE);
    isLooping = false;
}

void EventLoop::loop() {
    while (true) {
        auto activeChannels = epollMgr.pollWait();
        for (auto ch : activeChannels) {
            ch->handleIO();
        }

        std::vector<std::function<void(void)>> funcs;
        {
            std::lock_guard<std::mutex> lock(mtx);
            funcs.swap(pendingFuncList);
        }
        for (auto func : funcs) {
            func();
        }
    }
}

void EventLoop::addPendingFunc(std::function<void(void)> func) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        pendingFuncList.push_back(func);
    }
    if (!pendingFuncList.empty()) {
        uint64_t one = 1;
        if (int ok = write(notifiChannel->getFd(), &one, sizeof(one)); ok < 0) {
            printf("errno: %2d\t%s\n", errno, strerror(errno));
        }
    }
}

void EventLoop::removeChannel(int fd) {
    this->epollMgr.deleteEvent(this->channels[fd].get());
    this->channels[fd].reset();
    this->channels.erase(fd);
}

void EventLoop::startLoop() {
    isLooping = true;
}
