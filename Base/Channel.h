#pragma once
#include <sys/epoll.h>

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
    // Channel(int f, EventLoop* mgr = nullptr) :fd(f), event(0),
    // readCallback(nullptr), writeCallback(nullptr), errorCallback(nullptr),
    // ownerLoop(mgr) {} ~Channel(); void setOwnerLoop(EventLoop* loop) {
    // ownerLoop = loop; }; auto getOwnerLoop() { return ownerLoop; };
    int getFd() const { return fd; }
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

   private:
    EventCallback readCallback;
    EventCallback writeCallback;
    EventCallback closeCallback;
    EventCallback errorCallback;
    int fd;
    int event;
    int revent;
};
