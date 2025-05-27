#ifndef LINK_HPP
#define LINK_HPP

#include "process.hpp"
#include <string>
#include <map>

enum Link {
    cpu,
    io
};

class myLink {
public:
    static std::map<std::string, myLink*> link_map;
    static myLink* cputail;
    static myLink* iotail;

    myLink* next;
    myLink* before;
    Process* pro;

    myLink();
    myLink(std::string id, int need_time, int mem_size, int s_time, int e_time);
    ~myLink();

    void add(myLink* get_next, Link kind);
    void clean(Link kind);
    void remove(Link kind);
    static void kill(std::string id);
};

#endif