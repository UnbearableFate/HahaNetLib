#pragma once
#include<vector>
#include<thread>
#include"EventLoopThread.h"
class ThreadPool
{
public:
	std::vector<EventLoopThread> elThreads;
	size_t index;

	ThreadPool(int size = 2) :elThreads(size),index(-1) {}
	auto initThreads() {
		for (auto& t : elThreads) {
			t.initThread();
		}
	}
	/*
	auto startThreads() {
		EventLoopThread::startNotified = true;
		EventLoopThread::cv.notify_all();
	}
	*/
	auto getNextLoopPtr() {
		index = (index + 1) % elThreads.size();
		return elThreads[index].getLoopPtr();
	}
};

