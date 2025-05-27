#include "Link.hpp"
#include "Debug.hpp"

 map<string, myLink*> myLink::link_map;
 myLink* myLink::cputail = nullptr;
 myLink* myLink::iotail = nullptr;




 myLink::myLink() : next(nullptr), before(nullptr), pro(nullptr) {}

 myLink::myLink(string id, int need_time, int mem_size, int s_time, int e_time) {

     this->pro = new Process(id, need_time, mem_size, s_time, e_time);

 }

 void myLink::add(myLink* get_next, Link kind) {
     myLink*& tail = (kind == Link::cpu) ? myLink::cputail : myLink::iotail;

     if (!link_map.count(get_next->pro->id)) {
         link_map[get_next->pro->id] = get_next;
     }

     get_next->pro->status = PREPARE;
     tail->next = get_next;
     get_next->before = tail;
     tail = get_next; // ¸üÐÂÎ²²¿
 }

 void myLink::clean(Link kind) {
     this->remove(kind);
     this->pro->kill();
     delete this;
 }

 void myLink::remove(Link kind) {
     myLink*& tail = (kind == Link::cpu) ? myLink::cputail : myLink::iotail;

     if (this->before) {
         this->before->next = this->next;
     }

     if (this->next) {
         this->next->before = this->before;
     }

     if (tail == this) {
         tail = this->before;
     }
 }

 void myLink::kill(std::string id) {
     if (link_map.find(id) != link_map.end()) {
         link_map[id]->pro->set_is_delete();
     }
 }

 myLink::~myLink() {
     if (pro) {
         link_map.erase(this->pro->id);
         delete this->pro;
     }
     before = nullptr;
     next = nullptr;
     pro = nullptr;
 }