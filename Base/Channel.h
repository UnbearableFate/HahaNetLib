#pragma once
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <functional>
class Channel {
   public:
    using EventCallback = std::function<void()>;
    static const int READ_EVE = EPOLLIN | EPOLLPRI;
    static const int WRITE_EVE = EPOLLOUT;
    std::function<void(void)> update;
    Channel(int fd = -1)
        : fd(fd),
          readCallback(nullptr),
          writeCallback(nullptr),
          errorCallback(nullptr),
          event(0),
          revent(0) {}

    int getFd() { return fd; }
    int getEvent() const { return event; }
    void setRevent(int rev) { this->revent = rev; }
    void handleIO();
    void enableEvent(int e) {
        event |= e;
        this->update();
    }
    void disableEvent(int e) {
        event &= ~e;
        this->update();
    }
    void setReadCallback(const std::function<void(void)> func) {
        readCallback = std::move(func);
    }
    void setWriteCallback(std::function<void(void)> func) {
        writeCallback = std::move(func);
    }
    void setErrorCallback(std::function<void(void)> func) {
        errorCallback = std::move(func);
    }
    void setCloseCallback(std::function<void(void)> func) {
        closeCallback = std::move(func);
    }

    ~Channel() {
        shutdown(fd, SHUT_RDWR);
        readCallback = nullptr;
        writeCallback = nullptr;
        closeCallback = nullptr;
        errorCallback = nullptr;
    }

   private:
    EventCallback readCallback;
    EventCallback writeCallback;
    EventCallback closeCallback;
    EventCallback errorCallback;
    int fd;
    int event;
    int revent;
};
