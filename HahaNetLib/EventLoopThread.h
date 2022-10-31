#pragma once
#include<thread>
#include"EventLoop.h"
#include<functional>
#include<condition_variable>
#include<mutex>
#include<semaphore>
class EventLoopThread
{
private:
	std::shared_ptr<EventLoop> eventLoop;
	//std::thread theThread;
public:
	/*
	static std::condition_variable cv;
	static bool startNotified;
	static std::mutex m;
	*/
	
	EventLoopThread() :eventLoop(new EventLoop())
		//,theThread([](std::shared_ptr<EventLoop> eloop) { eloop->loop(); }, eventLoop)
	{
		//theThread.detach();
	}

	auto initThread() {
		auto theThread = std::thread([](std::shared_ptr<EventLoop> eloop) {eloop->loop(); }, eventLoop);
		theThread.detach();
	}
	auto getLoopPtr() {
		return this->eventLoop;
	}
};

