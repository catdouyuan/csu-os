#ifndef TIMETASK_HPP
#define TIMETASK_HPP

#include "Link.hpp"
#include "Debug.hpp"
#include <chrono>
#include <string>

class TimeTask {
public:
    std::string cpu_pro;
    std::string io_pro;
    int time = 1;
    myLink* cpuhead;  // �洢cpu�ȴ�����
    int cpu_place;
    int io_place;
    myLink* iohead;   // io�ȴ�����

    void cpustart();
    void iostart();
    void killproc(std::string id);
    void lsproc();
};

#endif