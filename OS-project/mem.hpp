#ifndef MEM_HPP
#define MEM_HPP

#include <list>
#include <utility>
#include <string>
#include <atomic>
#include <mutex>
#include "Debug.hpp"

class Mem {
public:
    static std::list<std::pair<int, int>> free_mem;
    static std::list<std::pair<int, std::pair<std::string, int>>> used_mem;
    static std::atomic<bool> is_compacting;
    static std::mutex mem_mutex;
    static std::atomic<int> total_fragment;
    static std::atomic<int> version;

    static void init(int size);
    static int allocate_memory(std::string id, int size, int& mem_place);
    static bool free_memory(std::string id);
    static void show_mem();
    static void compact_memory();

private:
    static int SIZE;
    static const int exp;
    static const float resize_;
};

#endif