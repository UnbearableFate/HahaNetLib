#pragma once

#include<thread>
#include "Channel.h"
#include <memory>
#include <mutex>
#include <condition_variable>
#include "EpollManager.h"
#include <map>
class EventLoop
{
public:
	void loop();
	bool isLooping;
	EventLoop();
	auto addChannel(int fd)->std::shared_ptr<Channel>;
	void addPendingFunc(std::function<void(void)>);
	void removeChannel(int fd);
	void startLoop();
private:
	std::map<int,std::shared_ptr<Channel>> channels;
	bool used = false;
	EpollManager epollMgr;
	std::vector<std::function<void(void)>> pendingFuncList;
	EventLoop(EventLoop&) = delete;
	EventLoop& operator=(const EventLoop&) = delete;
	std::shared_ptr<Channel> notifiChannel;
};

