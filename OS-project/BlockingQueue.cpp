#include "BlockingQueue.hpp"

BlockingQueue::BlockingQueue(int capacity)
    : capacity_(capacity), size(0) {}

void BlockingQueue::push(const std::string& value) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this]() { return size < capacity_; });

    queue.push(value);
    ++size;
    cv.notify_one();
}

std::string BlockingQueue::pop() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this]() { return size > 0; });

    std::string value = queue.front();
    queue.pop();
    --size;
    cv.notify_one();
    return value;
}

bool BlockingQueue::empty() {
    return my_size() == 0;
}

int BlockingQueue::my_size() {
    return size.load();
}