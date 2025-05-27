
#include "Debug.hpp"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
 string Debug::INFO = "[INFO] ";
 string Debug::ERROR = "[ERROR] ";
 string Debug::OUTPUT = "[RESULT] ";
 string Debug::RED = "\033[31m";    
 string Debug::BLACK = "\033[0m";
 string Debug::GREEN = "\033[32m";
 const std::string Debug::FILE_NAME = "log.txt";
 const int Debug::QUEUE_SIZE = 100;
 std::ofstream Debug::file(Debug::FILE_NAME, std::ios::app);
 BlockingQueue Debug::blockingqueue{ Debug::QUEUE_SIZE };
 std::atomic<bool> Debug::stop_thread(false);




 void Debug::file_debug(std::string mes, std::string debug) {
     std::time_t t = std::time(nullptr);
     std::tm* local_tm = std::localtime(&t);

     std::ostringstream oss;
     oss << std::put_time(local_tm, "%Y-%m-%d %H:%M:%S");
     std::string time_str = oss.str();

     blockingqueue.push(debug + "[ " + time_str + " ]" + mes + '\n');
 }

 void Debug::thread_debug() {
     Debug::INFO_DEBUG("start debug");
     while (!stop_thread) {
         std::string s = blockingqueue.pop();
         file << s;
         file.flush();
     }
     file.close();
 }

 void Debug::debug1(std::string mes, std::string debug, std::string color) {
     std::cout << color + debug + BLACK + mes << std::endl;
 }

 void Debug::INFO_DEBUG(std::string mes) {
     debug1(mes, Debug::INFO, Debug::BLACK);
     file_debug(mes, Debug::INFO);
 }

 void Debug::ERROR_DEBUG(std::string mes) {
     debug1(mes, Debug::ERROR, Debug::RED);
     file_debug(mes, Debug::ERROR);
 }

 void Debug::OUTPUT_DEBUG(std::string mes) {
     debug1(mes, Debug::OUTPUT, Debug::GREEN);
     file_debug(mes, Debug::OUTPUT);
 }
 void Debug::LIST_DEBUG(std::string mes) {
     std::cout << "================="+ mes+"===================" << std::endl;

 }

 void Debug::stop() {
     stop_thread = true;
 }

 void Debug::clean() {
     Debug::stop();
 }