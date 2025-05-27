#include "mem.hpp"

#include <algorithm>
#include <thread>

 list<pair<int, int>>Mem::free_mem;
 list<pair<int, pair<string, int >>>Mem::used_mem;
 int Mem::SIZE=2048;
 atomic<bool> Mem::is_compacting{ false };
 std::mutex Mem::mem_mutex;
 atomic<int> Mem::version{ 0 };
 const int Mem::exp = 0;
 std::atomic<int> Mem::total_fragment{ 0 };
   const float Mem::resize_=0.1f;




   void Mem::init(int size) {
       SIZE = size;
       free_mem.push_back(std::make_pair(0, size));
   }

   int Mem::allocate_memory(std::string id, int size, int& mem_place) {
       int ver_before = version.load(std::memory_order_acquire);
       while (1) {
           auto it = std::find_if(free_mem.begin(), free_mem.end(), [size](const std::pair<int, int>& x) {
               return x.second >= size; // 首次适应算法
               });

           if (it != free_mem.end()) {
               int place = it->first;
               if (it->second > size + exp) {
                   used_mem.push_back(std::make_pair(it->first, std::make_pair(id, size)));
                   free_mem.push_back(std::make_pair(it->first + size, it->second - size));
               }
               else {
                   used_mem.push_back(std::make_pair(it->first, std::make_pair(id, it->second)));
               }
               free_mem.erase(it);

               int ver_after = version.load(std::memory_order_acquire);
               if (ver_before == ver_after) {
                   mem_place = place;
                   return place;
               }
               else {
                   std::this_thread::yield();
                   continue;
               }
           }
           else {
               return -1;
           }
       }
   }

   bool Mem::free_memory(std::string id) {
       std::lock_guard<std::mutex> lock(mem_mutex);
       auto it = std::find_if(used_mem.begin(), used_mem.end(), [id](const std::pair<int, std::pair<std::string, int>>& x) {
           return id == x.second.first;
           });

       if (it != used_mem.end()) {
           total_fragment.fetch_add(it->second.second);
           free_mem.push_back(std::make_pair(it->first, it->second.second));
           used_mem.erase(it);
           version.fetch_add(1, std::memory_order_release);

           if (static_cast<float>(total_fragment.load()) / Mem::SIZE > Mem::resize_) {
               std::thread resize_thread(&Mem::compact_memory);
               resize_thread.detach();
           }
           return true;
       }
       return false;
   }

   void Mem::show_mem() {
       if (!free_mem.empty()) {
           Debug::LIST_DEBUG("free_mem_list");
           int cnt = 1;
           for (auto& list : free_mem) {
               Debug::INFO_DEBUG("free Block: " + std::to_string(cnt) +
                   ". start place: " + std::to_string(list.first) +
                   " memory size: " + std::to_string(list.second));
               cnt++;
           }
       }
       else {
           Debug::INFO_DEBUG("free memory list is null");
       }

       if (!used_mem.empty()) {
           Debug::LIST_DEBUG("used_mem_list");
           int cnt = 1;
           for (auto& list : used_mem) {
               Debug::INFO_DEBUG("used Block: " + std::to_string(cnt) +
                   ". proc_id: " + list.second.first +
                   " start place: " + std::to_string(list.first) +
                   " memory size: " + std::to_string(list.second.second));
               cnt++;
           }
       }
       else {
           Debug::INFO_DEBUG("used memory list is null");
       }
   }

   void Mem::compact_memory() {
       bool expected = false;
       if (!is_compacting.compare_exchange_strong(expected, true, std::memory_order_acq_rel)) {
           return;
       }

       std::list<std::pair<int, int>> local_free;
       {
           std::lock_guard<std::mutex> lock(mem_mutex);
           local_free = free_mem;
       }

       local_free.sort([](const std::pair<int, int>& a, const std::pair<int, int>& b) {
           return a.first < b.first;
           });

       for (auto it = local_free.begin(); it != local_free.end();) {
           auto next_it = std::next(it);
           if (next_it != local_free.end() && (it->first + it->second) >= next_it->first) {
               it->second = std::max(it->first + it->second, next_it->first + next_it->second) - it->first;
               local_free.erase(next_it);
           }
           else {
               ++it;
           }
       }

       local_free.sort([](std::pair<int, int> a, std::pair<int, int> b) {
           return a.second < b.second;
           });

       {
           std::lock_guard<std::mutex> lock(mem_mutex);
           free_mem = local_free;
           version.fetch_add(1, std::memory_order_release);
       }

       is_compacting.store(false, std::memory_order_release);
   }