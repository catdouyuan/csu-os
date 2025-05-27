#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <string>
#include <thread>
#include "mem.hpp"
#include "context.hpp"
#include "excp.hpp"
#include "common.hpp"

class Process {
public:
    pro_status status;
    std::string id;
    int place;
    int mem_size;
    int need_time;
    int s_time;
    int io_time;
    int work_time;
    bool io_finish;
    bool is_delete;

    Process(std::string id, int need_time, int mem_size, int s_time, int e_time);
    ~Process();

    void kill();
    void work();
    void finish();
    void set_kill();
    void update_time_status(int work_time, enum pro_status a);
    void finish_io();
    void set_is_delete();
};

#endif