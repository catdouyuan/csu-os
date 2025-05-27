#include "Process.hpp"
#include "Debug.hpp"

Process::Process(std::string id, int need_time, int mem_size, int s_time, int e_time)
    : place(-1), status(CREATE), io_finish(false), is_delete(false), work_time(0) {

    this->id = id;
    this->need_time = need_time;
    this->mem_size = mem_size;
    proc_status[id] = make_pair(pro_status::CREATE, -1);

    // ·ÖÅäÄÚ´æ
    std::thread mem_allocate(&Mem::allocate_memory, id, mem_size, std::ref(this->place));
    mem_allocate.detach();

    this->s_time = (s_time == -1) ? INT32_MAX : s_time;
    this->io_time = e_time - s_time;
    proc_status[id] = make_pair(pro_status::PREPARE, place);
    this->status = PREPARE;
}

Process::~Process() {
    Debug::INFO_DEBUG("clean process ok");
}

void Process::kill() {
    pro_id.erase(id);
    Mem::free_memory(id);
}

void Process::work() {
    this->status = pro_status::WORK;
}

void Process::finish() {
    this->status = pro_status::FINISH;
    this->work_time = this->need_time;
}

void Process::set_kill() {
    this->status = pro_status::UNDO;
}

void Process::update_time_status(int work_time, enum pro_status a) {
    this->work_time += work_time;
    this->status = a;
}

void Process::finish_io() {
    this->work_time += io_time;
    this->io_finish = true;
}

void Process::set_is_delete() {
    Debug::OUTPUT_DEBUG("delete ok£¡£¡£¡");
    this->is_delete = true;
    this->status = pro_status::UNDO;
}