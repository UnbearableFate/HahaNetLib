#pragma once
#include <arpa/inet.h>

#include <array>
#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "../Base/Channel.h"
#include "../Base/EventLoop.h"
#include "../Buffer/ReadBuffer.h"
#include "../Buffer/WriteBuffer.h"
class TcpConnection {
   private:
    int connfd;
    ReadBuffer readBuffer;
    WriteBuffer writeBuffer;
    std::shared_ptr<Channel> chan;
    std::function<void()> msgHandleCb;
    sockaddr_in clientAddr;
    std::function<void()> disconnection;
    std::shared_ptr<EventLoop> ownerloop;
    bool keepAlive;
    std::chrono::_V2::system_clock::time_point finishTimeStamp;

   protected:
    auto sendDataInBuf() {
        if (writeBuffer.writeTo(connfd) == 0) {
            chan->disableEvent(Channel::WRITE_EVE);
        }
    }

    auto saveDataToBufFromSocket() {
        if (readBuffer.readFromSocket(connfd) > 0) {
            this->finishTimeStamp += TcpConnection::aliveMinute;
            msgHandleCb();
        } else {
            if (this->keepAlive) {
                auto timeDiff = std::chrono::system_clock::now() - this->finishTimeStamp;
                if (timeDiff <= std::chrono::nanoseconds::zero()) {
                    disconnection();
                }
            } else {
                disconnection();
            }
        };
    }

    auto setDisconnectionCb(std::function<void()> &&cb) {
        disconnection = std::move(cb);
    }

    auto setSocketChannel(std::shared_ptr<Channel> ch) { chan = ch; }

    auto setMsgHandleCb(std::function<void()> &&cb) {
        msgHandleCb = std::move(cb);
    }

   public:
    friend class TcpServer;
    static constexpr std::chrono::minutes aliveMinute{1};
    auto getClientAddr() {
        std::string res(INET_ADDRSTRLEN, '\0');
        auto decimalIpAddr = inet_ntop(AF_INET, &clientAddr.sin_addr,
                                       res.data(), INET_ADDRSTRLEN);
        return std::string(decimalIpAddr);
    }
    void setKeepAlive(bool alive = true) {
        this->keepAlive = alive;
        this->finishTimeStamp = std::chrono::system_clock::now() + TcpConnection::aliveMinute;
    }
    TcpConnection(TcpConnection &) = delete;
    TcpConnection() : connfd(-1){};
    TcpConnection(sockaddr_in &&ch, int fd) : connfd{fd}, clientAddr{std::move(ch)} {}

    auto receiveMsg() -> std::string { return readBuffer.getStringFromBuf(); }

    auto sendMsg(std::string msg) {
        writeBuffer.saveToBuf(msg);
        this->chan->enableEvent(Channel::WRITE_EVE);
    }

    ~TcpConnection() {
        // shutdown(connfd, SHUT_RDWR);
        // chan.reset();
        printf("xigou");
    }
};

// constexpr int TcpConnection::aliveMinute = 2;