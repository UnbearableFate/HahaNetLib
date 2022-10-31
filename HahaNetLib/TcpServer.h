#pragma once
#include<vector>
#include "Acceptor.h"
#include "EventLoop.h"
#include "TCPConnection.h"
#include <functional>
#include <map>
#include <memory>
#include "ThreadPool.h"
class TcpServer
{
private:
	std::shared_ptr<EventLoop> mainLoop;
	Acceptor acceptor;
	ThreadPool threadPool;
	std::map<int,std::shared_ptr<TcpConnection>> connMap;
	
	auto disconntion(int fd) {
		connMap[fd]->ownerloop->removeChannel(fd);
		connMap.erase(fd);
	}

	auto createNewConnetion(int connfd, sockaddr_in cli) {
		connMap.try_emplace(connfd,std::make_shared<TcpConnection>(std::move(cli), connfd));
		//connMap[connfd]->setMsgHandleCb(std::bind(&TcpServer::requestHandle, this , connMap[connfd]));
		auto newConnectionPtr = connMap[connfd];
		newConnectionPtr ->setMsgHandleCb([this,connfd]() {
			this->requestHandle(std::ref(*connMap[connfd]));
			});
		newConnectionPtr->setDisconnectionCb(std::bind(&TcpServer::disconntion, this, connfd));
		auto loop = threadPool.getNextLoopPtr();
		newConnectionPtr->ownerloop = loop;
		auto ch = loop->addChannel(connfd);
		ch->setWriteCallback(std::bind(&TcpConnection::sendDataInBuf, newConnectionPtr));
		ch->setReadCallback(std::bind(&TcpConnection::saveDataToBufFromSocket, newConnectionPtr));
		ch->enableEvent(Channel::READ_EVE);
		newConnectionPtr->setSocketChannel(ch);
	}

public:
	virtual void requestHandle(TcpConnection& conn) {}
	virtual void establishConnectionHandle(TcpConnection& conn) {};
	~TcpServer() {}
	TcpServer(int p = 8082) :mainLoop(new EventLoop()), threadPool(),
		acceptor(p) {
		auto acceptorCh = mainLoop->addChannel(this->acceptor.getListenFd());
		acceptorCh->setReadCallback([this]() {
			sockaddr_in cliAddr;
			socklen_t len = sizeof(cliAddr);
			int connfd = accept(this->acceptor.getListenFd(), (sockaddr*)&cliAddr, &len);
			if (connfd > 0) {
				this->createNewConnetion(connfd, cliAddr);
				this->establishConnectionHandle(*(this->connMap[connfd]));
			}
			}
		);
		acceptorCh->enableEvent(Channel::READ_EVE);
	}
	void start() {
		threadPool.initThreads();
		mainLoop->loop();
	}
};

