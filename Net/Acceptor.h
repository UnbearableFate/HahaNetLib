#pragma once
#include <arpa/inet.h>
#include <sys/socket.h>

#include <functional>
#include <stdexcept>
#include <string>

#include "../Base/Channel.h"

class Acceptor {
   private:
    int listenfd;
    std::function<void(int, sockaddr_in&)> createNewConnCallback;

   public:
    Acceptor(Acceptor&) = delete;
    // auto createNewConnCallback(std::function<void(int, sockaddr_in)> cb) {
    // createNewConnCallback = std::move(cb); }

    Acceptor(int port = 8082)
        : listenfd(
              socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, 0)) {
        sockaddr_in servAddr;
        servAddr.sin_family = AF_INET;
        servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servAddr.sin_port = htons(port);
        if (bind(this->listenfd, (sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
            throw std::runtime_error("bind error");
            abort();
        }
        if (listen(this->listenfd, 2048)) {
            throw std::runtime_error("listen error");
        }
        // this->acceptChan.setReadCallback(std::bind(&Acceptor::acceptWhenReady,
        // this)); this->acceptChan.enableEvent(Channel::READ_EVE);
    }

    int getListenFd() { return listenfd; }
    void acceptWhenReady() {
        sockaddr_in cliAddr;
        socklen_t len = sizeof(cliAddr);
        int connfd = accept(this->listenfd, (sockaddr*)&cliAddr, &len);
        if (connfd > 0) {
            createNewConnCallback(connfd, cliAddr);
        }
    }
};
