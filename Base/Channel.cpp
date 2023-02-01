#include "Channel.h"
#include <unistd.h>
#include <sys/epoll.h>
void Channel::handleIO() {
    if (revent & (EPOLLERR))
    {
        if (errorCallback) errorCallback();
    }
    if (revent & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
    {
        if (readCallback) readCallback();
    }
    if (revent & EPOLLOUT)
    {
        if (writeCallback) writeCallback();
    }
}

