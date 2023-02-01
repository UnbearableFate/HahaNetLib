#include "EpollManager.h"

#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <cerrno>
#include <cstdio>
EpollManager::EpollManager() { eventsFd = epoll_create1(EPOLL_CLOEXEC); }

EpollManager::~EpollManager() { close(eventsFd); }

void EpollManager::addEvent(Channel* channel) {
    epoll_event e;
    e.data.ptr = channel;
    e.events = channel->getEvent();  // | EPOLLEXCLUSIVE;
    int fd = channel->getFd();
    epoll_ctl(eventsFd, EPOLL_CTL_ADD, channel->getFd(), &e);
    channel->update = std::bind(&EpollManager::modifyEvent, this, channel);
}

void EpollManager::modifyEvent(Channel* channel) {
    if (channel->getEvent() == 0) {
        deleteEvent(channel);
    }
    epoll_event e;
    e.data.ptr = channel;
    e.events = channel->getEvent();  // | EPOLLEXCLUSIVE;
    epoll_ctl(eventsFd, EPOLL_CTL_MOD, channel->getFd(), &e);
}

void EpollManager::deleteEvent(Channel* channel) {
    epoll_ctl(eventsFd, EPOLL_CTL_DEL, channel->getFd(), nullptr);
}

std::vector<Channel*> EpollManager::pollWait() {
    int num;
    std::vector<epoll_event> eventsVector(64);
    while (true) {
        num = epoll_wait(this->eventsFd, eventsVector.data(),
                         static_cast<int>(eventsVector.size()), -1);
        if (num < 0) {
            if (errno != EINTR) {
                printf("%s\n", strerror(errno));
            }
        } else {
            break;
        }
    }
    std::vector<Channel*> activeChennel;
    for (int i = 0; i != num; ++i) {
        auto ch = static_cast<Channel*>(eventsVector[i].data.ptr);
        ch->setRevent(eventsVector[i].events);
        activeChennel.push_back(ch);
    }
    return activeChennel;
}
