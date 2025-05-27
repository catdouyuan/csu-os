#ifndef BLOCKINGQUEUE_HPP
#define BLOCKINGQUEUE_HPP

#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>
#include <atomic>

class BlockingQueue {
private:
    std::queue<std::string> queue;
    std::mutex mtx;
    std::atomic<size_t> size;
    std::condition_variable cv;
    int capacity_;

public:
    explicit BlockingQueue(int capacity);

    void push(const std::string& value);
    std::string pop();
    bool empty();
    int my_size();
};

#endif