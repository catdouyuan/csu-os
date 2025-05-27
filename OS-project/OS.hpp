#ifndef OS_HPP
#define OS_HPP

#include "common.hpp"
#include "Debug.hpp"
#include "Link.hpp"
#include "mem.hpp"
#include "timetask.hpp"
#include "File.hpp"
#include <string>

class OS {
private:
    myLink* head;
    myLink* head2;
    TimeTask task1;

public:
    OS();

    void create_proc(std::string id, int utime, int memsize, int iotime, int etime);
    void kill_proc(std::string id);
    void ls_proc();
    void ls_mem();
    void create_file(std::string file_name, int file_size);
    void delete_file(std::string file_name);
    void ls_file(std::string file_name);
    void clean();
    void help();
};

#endif