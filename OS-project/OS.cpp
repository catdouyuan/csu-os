#include "OS.hpp"

OS::OS() {
    head = new myLink();
    head2 = new myLink();
    task1.cpuhead = head;
    task1.iohead = head2;
    myLink::cputail = head;
    myLink::iotail = head2;

    // ��ʼ��ʱ����
    std::thread cpu_thread(&TimeTask::cpustart, &task1);
    std::thread io_thread(&TimeTask::iostart, &task1);
    std::thread debug_thread(&Debug::thread_debug);

    debug_thread.detach();
    cpu_thread.detach();
    io_thread.detach();

    Mem::init(MEM_SIZE); // ��ʼ��mem������
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
    // ����ʵ��
}

void OS::help() {
    Debug::INFO_DEBUG("creatproc �������� ����ʱ�� �ڴ������С [I/O��ʼֹͣʱ��/-1] ");
    Debug::INFO_DEBUG("killproc ���̺�");
    Debug::INFO_DEBUG("psproc");
    Debug::INFO_DEBUG("mem");
    Debug::INFO_DEBUG("creatfile  �ļ���");
    Debug::INFO_DEBUG("deletefile  �ļ���");
    Debug::INFO_DEBUG("lsfile  �ļ���");
}