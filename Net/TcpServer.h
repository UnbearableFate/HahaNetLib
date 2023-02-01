#pragma once
#include <functional>
#include <map>
#include <memory>
#include <vector>

#include "../Base/EventLoop.h"
#include "../Base/ThreadPool.h"
#include "Acceptor.h"
#include "TCPConnection.h"
class TcpServer {
   private:
    std::shared_ptr<EventLoop> mainLoop;
    Acceptor acceptor;
    ThreadPool threadPool;
    std::map<int, std::shared_ptr<TcpConnection>> connMap;

    auto disconntion(int fd) {
        connMap[fd]->ownerloop->removeChannel(fd);
        connMap.erase(fd);
        printf("close connection");
    }

    auto createNewConnetion(int connfd, sockaddr_in cli) -> void;

   public:
    virtual void requestHandle(TcpConnection& conn) {}
    virtual void establishConnectionHandle(TcpConnection& conn){};
    ~TcpServer() {}
    TcpServer(int p = 8082, int threadNum = 2)
        : mainLoop(new EventLoop()), threadPool(threadNum), acceptor(p) {
        auto acceptorCh = mainLoop->addChannel(this->acceptor.getListenFd());
        acceptorCh->setReadCallback([this]() {
            sockaddr_in cliAddr;
            socklen_t len = sizeof(cliAddr);
            int connfd =
                accept(this->acceptor.getListenFd(), (sockaddr*)&cliAddr, &len);
            if (connfd > 0) {
                this->createNewConnetion(connfd, cliAddr);
                this->establishConnectionHandle(*(this->connMap[connfd]));
            }
        });
        acceptorCh->enableEvent(Channel::READ_EVE);
    }
    void start() {
        threadPool.initThreads();
        mainLoop->loop();
    }
};
