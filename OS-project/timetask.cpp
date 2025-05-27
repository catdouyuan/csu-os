#include "TimeTask.hpp"

void TimeTask::cpustart() {
    while (1) {
        while (cpuhead->next == nullptr); // �������н��̴���

        myLink* it = cpuhead->next;
        while (it) {
            cpu_pro = it->pro->id;
            cpu_place = it->pro->place;
            it->pro->work(); // ���̿�ʼ���� ����Ϊwork

            auto start = std::chrono::steady_clock::now();
            while (true) {
                auto now = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();

                // ����Ϊ���״̬
                if (it->pro->is_delete) {
                    cpu_pro = "";
                    proc_status[it->pro->id] = make_pair(pro_status::FINISH, -1);
                    it->pro->status = pro_status::FINISH;
                    it->clean(Link::cpu);
                    it = it->before;
                    break;
                }

                // ����ΪBlock״̬
                if (!it->pro->io_finish && it->pro->work_time + elapsed >= it->pro->s_time) {
                    cpu_pro = "";
                    Debug::INFO_DEBUG("io request come id: " + it->pro->id);
                    proc_status[it->pro->id] = make_pair(pro_status::BLOCK, it->pro->place);
                    it->pro->update_time_status(elapsed, BLOCK);
                    auto before = it->before;
                    it->remove(Link::cpu);
                    iohead->add(it, Link::io);
                    it = before;
                    break;
                }

                // ����Ϊ���״̬
                if (elapsed + it->pro->work_time >= it->pro->need_time) {
                    Debug::INFO_DEBUG("����id:" + it->pro->id + " is finished");
                    cpu_pro = "";
                    proc_status[it->pro->id] = make_pair(pro_status::FINISH, -1);
                    it->pro->finish();
                    auto before = it->before;
                    it->clean(Link::cpu);
                    it = before;
                    break;
                }

                // ����Ϊ׼��״̬
                if (elapsed >= time) {
                    cpu_pro = "";
                    proc_status[it->pro->id] = make_pair(pro_status::PREPARE, it->pro->place);
                    it->pro->update_time_status(time, PREPARE);
                    break;
                }
            }

            it = it->next;
            if (it == nullptr) {
                it = cpuhead->next; // ��ͷ ����дѭ������
            }
        }
    }
}

void TimeTask::iostart() {
    while (1) {
        while (iohead->next == nullptr);
        myLink* it = iohead->next;

        auto start = std::chrono::steady_clock::now();
        while (it) {
            it->pro->status = pro_status::IO;
            io_pro = it->pro->id;
            io_place = it->pro->place;

            while (1) {
                auto now = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();

                // ɾ����������Ϊ���
                if (it->pro->is_delete) {
                    io_pro = "";
                    auto before = it->before;
                    proc_status[it->pro->id] = make_pair(pro_status::FINISH, -1);
                    it->pro->status = pro_status::FINISH;
                    it->clean(Link::io);
                    it = before;
                    break;
                }

                // ��io����ִ�����ʱ
                if (elapsed >= it->pro->io_time) {
                    Debug::INFO_DEBUG("io request is over id: " + it->pro->id);
                    io_pro = "";
                    it->pro->finish_io();
                    auto before = it->before;
                    it->remove(Link::io);

                    if (it->pro->work_time != it->pro->need_time) {
                        // ����cpu����Ϊ׼��
                        proc_status[it->pro->id] = make_pair(pro_status::PREPARE, it->pro->place);
                        it->pro->status = pro_status::PREPARE;
                        cpuhead->add(it, Link::cpu);
                    }
                    else {
                        // ����Ϊ���
                        proc_status[it->pro->id] = make_pair(pro_status::FINISH, -1);
                        it->pro->status = pro_status::FINISH;
                        it->clean(Link::io);
                    }
                    it = before;
                    break;
                }
            }

            it = it->next;
            if (it == nullptr) it = iohead->next;
        }
    }
}

void TimeTask::killproc(std::string id) {
    if (cpu_pro == id) cpu_pro = "";
    if (io_pro == id) io_pro = "";
    proc_status[id].first = pro_status::UNDO;
    myLink::kill(id);
}

void TimeTask::lsproc() {
    Debug::LIST_DEBUG("proc_list");
    if (cpu_pro != "")
        Debug::INFO_DEBUG(" proc_id:" + cpu_pro + " is WORK" + " place is " + std::to_string(cpu_place));
    if (io_pro != "")
        Debug::INFO_DEBUG(" proc_id:" + io_pro + "is IO" + " place is " + std::to_string(io_place));

    for (auto it : proc_status) {
        if (it.first == cpu_pro || it.first == io_pro) continue;

        if (it.second.first == pro_status::BLOCK) {
            Debug::INFO_DEBUG(" proc_id: " + it.first + " is BLOCK" + " place is " + std::to_string(it.second.second));
        }
        else if (it.second.first == pro_status::FINISH) {
            Debug::INFO_DEBUG(" proc_id: " + it.first + " is FINISH");
        }
        else if (it.second.first == pro_status::PREPARE) {
            Debug::INFO_DEBUG(" proc_id: " + it.first + " is PREPARE" + " place is " + std::to_string(it.second.second));
        }
        else if (it.second.first == pro_status::UNDO) {
            Debug::INFO_DEBUG(" proc_id: " + it.first + " is UNDO" + " place is " + std::to_string(it.second.second));
        }
    }
}