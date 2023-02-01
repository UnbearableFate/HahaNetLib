#include "TcpServer.h"

auto TcpServer::createNewConnetion(int connfd, sockaddr_in cli)->void {
	connMap.try_emplace(connfd, std::make_shared<TcpConnection>(std::move(cli), connfd));
	//connMap[connfd]->setMsgHandleCb(std::bind(&TcpServer::requestHandle, this , connMap[connfd]));
	auto newConnectionPtr = connMap[connfd];
	newConnectionPtr->setMsgHandleCb([this, connfd]() {
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
