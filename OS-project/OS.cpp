#include "OS.hpp"

OS::OS() {
    head = new myLink();
    head2 = new myLink();
    task1.cpuhead = head;
    task1.iohead = head2;
    myLink::cputail = head;
    myLink::iotail = head2;

    // 开始定时任务
    std::thread cpu_thread(&TimeTask::cpustart, &task1);
    std::thread io_thread(&TimeTask::iostart, &task1);
    std::thread debug_thread(&Debug::thread_debug);

    debug_thread.detach();
    cpu_thread.detach();
    io_thread.detach();

    Mem::init(MEM_SIZE); // 初始化mem分配器
}

void OS::create_proc(std::string id, int utime, int memsize, int iotime, int etime) {
    Debug::INFO_DEBUG("try to creat proc now id: " + id + " time: " + std::to_string(utime) +
        " mem_size: " + std::to_string(memsize) + " io_time " + std::to_string(iotime));

    if (pro_id.find(id) == pro_id.end()) {
        pro_id.insert(id);

        try {
            myLink* creat_link = new myLink(id, utime, memsize, iotime, etime);
            head->add(creat_link, Link::cpu);
            Debug::OUTPUT_DEBUG("create process success !!!");
        }
        catch (const MyException& e) {
            Debug::ERROR_DEBUG(e.what());
        }
    }
    else {
        Debug::ERROR_DEBUG("pro_id has already exit id:" + id);
    }
}

void OS::kill_proc(std::string id) {
    Debug::INFO_DEBUG("try to kill proc_id: " + id);

    if (pro_id.find(id) != pro_id.end()) {
        task1.killproc(id);
    }
    else {
        Debug::ERROR_DEBUG("process not exit");
    }
}

void OS::ls_proc() {
    task1.lsproc();
}

void OS::ls_mem() {
    Mem::show_mem();
}

void OS::create_file(std::string file_name, int file_size) {
    File_manager::create_file(file_name, file_size);
}

void OS::delete_file(std::string file_name) {
    File_manager::delete_file(file_name);
}

void OS::ls_file(std::string file_name) {
    File_manager::show_file(file_name);
}

void OS::clean() {
    // 清理实现
}

void OS::help() {
    Debug::INFO_DEBUG("creatproc 进程名称 运行时长 内存需求大小 [I/O起始停止时间/-1] ");
    Debug::INFO_DEBUG("killproc 进程号");
    Debug::INFO_DEBUG("psproc");
    Debug::INFO_DEBUG("mem");
    Debug::INFO_DEBUG("creatfile  文件名");
    Debug::INFO_DEBUG("deletefile  文件名");
    Debug::INFO_DEBUG("lsfile  文件名");
}