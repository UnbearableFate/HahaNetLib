#pragma once
#include<string>
#include<array>
#include<memory>
#include "ReadBuffer.h"
#include "WriteBuffer.h"
#include <functional>
#include "Channel.h"
#include <arpa/inet.h>
#include "EventLoop.h"
#include <arpa/inet.h>
class TcpConnection
{
private:
	int connfd;
	ReadBuffer readBuffer;
	WriteBuffer writeBuffer;
	std::shared_ptr<Channel> chan;
	std::function<void()> msgHandleCb;
	sockaddr_in clientAddr;
	std::function<void()> disconnection;
	std::shared_ptr<EventLoop> ownerloop;
protected:
	auto sendDataInBuf() {
		writeBuffer.writeTo(connfd);
		chan->disableEvent(Channel::WRITE_EVE);
	}

	auto saveDataToBufFromSocket() {
		if (readBuffer.readFrom(connfd) > 0) {
			msgHandleCb();
		}
		else
		{
			disconnection();
		};
	}
public:
	friend class TcpServer;
	auto getClientAddr() {
		std::string res(INET_ADDRSTRLEN,'\0');
		auto decimalIpAddr = inet_ntop(AF_INET, &clientAddr.sin_addr, res.data(), INET_ADDRSTRLEN);
		return std::string(decimalIpAddr);
	}
	TcpConnection(TcpConnection&) = delete;
	TcpConnection():connfd(-1) {}
	TcpConnection(sockaddr_in&& ch, int fd) :connfd{ fd }, clientAddr{ std::move(ch) } {

	}
	
	auto setDisconnectionCb(std::function<void()> &&cb) {
		disconnection = std::move(cb);
	}

	auto setSocketChannel(std::shared_ptr<Channel> ch) {
		chan = ch;
	}

	auto setMsgHandleCb(std::function<void()> &&cb) {
		msgHandleCb = std::move(cb);
	}

	auto receiveMsg() -> std::string {
		return readBuffer.getStringFromBuf();
	}

	auto sendMsg(std::string msg) {
		writeBuffer.saveToBuf(msg);
		this->chan->enableEvent(Channel::WRITE_EVE);
	}
	
	~TcpConnection()
	{
		//shutdown(connfd, SHUT_RDWR);
		//chan.reset();
		printf("xigou");
	}
};

