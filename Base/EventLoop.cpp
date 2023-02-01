#include "EventLoop.h"
#include<unistd.h>
#include<sys/epoll.h>
#include <sys/eventfd.h>

auto EventLoop::addChannel(int fd)->std::shared_ptr<Channel> {
	this->channels.try_emplace(fd, std::make_shared<Channel>(fd));
	auto ch = this->channels[fd];
	this->epollMgr.addEvent(ch.get());
	return ch;
}

EventLoop::EventLoop() :epollMgr(){
	notifiChannel = this->addChannel(eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK));
	notifiChannel->setReadCallback([&]() {
		uint64_t one = 1;
		ssize_t n = read(notifiChannel->getFd(), &one, sizeof one);
		if (n != sizeof one)
		{
			printf("notified");
		}
		});
	notifiChannel->enableEvent(Channel::READ_EVE);
	isLooping = false;
}

void EventLoop::loop() {
	while (true)
	{
		auto activeChannels = epollMgr.pollWait();
		for (auto ch : activeChannels) {
			ch->handleIO();
		}
		for (auto func : pendingFuncList) {
			func();
		}
		pendingFuncList.clear();
	}
}


void EventLoop::addPendingFunc(std::function<void(void)> func) {
	if (!pendingFuncList.empty()) {
		char noti = '0';
		write(notifiChannel->getFd(), &noti, 1);
	}
	pendingFuncList.push_back(func);
}

void EventLoop::removeChannel(int fd) {
	this->epollMgr.deleteEvent(this->channels[fd].get());
	this->channels.erase(fd);
}

void EventLoop::startLoop() {
	isLooping = true;
}

