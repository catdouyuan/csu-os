#ifndef DEBUG_HPP
#define DEBUG_HPP

#include "common.hpp"
#include "BlockingQueue.hpp"
#include <string>
#include <atomic>
#include <fstream>

class Debug {
public:
    static const std::string FILE_NAME;
    static const int QUEUE_SIZE;
    static std::string INFO;
    static std::string ERROR;
    static std::string OUTPUT;
    static std::string RED;
    static std::string BLACK;
    static std::string GREEN;

    static std::ofstream file;
    static BlockingQueue blockingqueue;
    static std::atomic<bool> stop_thread;

    static void file_debug(std::string mes, std::string debug);
    static void thread_debug();
    static void debug1(std::string mes, std::string debug, std::string color);
    static void INFO_DEBUG(std::string mes);
    static void ERROR_DEBUG(std::string mes);
    static void OUTPUT_DEBUG(std::string mes);
    static void LIST_DEBUG(std::string mes);
    static void stop();
    static void clean();
};

#endif