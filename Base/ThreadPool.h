#pragma once
#include <atomic>
#include <memory>
#include <thread>
#include <vector>

#include "EventLoopThread.h"
class ThreadPool {
   public:
    std::vector<EventLoopThread> elThreads;
    std::atomic<int> index{-1};

    ThreadPool(int size = 2) : elThreads(size) {}
    auto initThreads() {
        for (auto& t : elThreads) {
            t.initThread();
        }
    }

    auto getNextLoopPtr() {
        index = (index + 1) % elThreads.size();
        return elThreads[index].getLoopPtr();
    }
};
