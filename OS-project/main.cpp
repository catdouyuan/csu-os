#include <iostream>	
#include <queue>
#include "Link.hpp"
#include "timetask.hpp"
#include <string>
#include <sstream>
#include <set>
#include "OS.hpp"
using namespace std;
//进程 内存 外存
//定时任务？ 时间片 定时器
map<string, pair<pro_status,int >>proc_status;
std::set<std::string> pro_id;
//TODO 增加一个给出进程信息级别的参数 psproc
//TODO file_size 加单位

string id, file_name;
int file_size;
int  utime, memsize, iotime, etime = -1;
int  getorder() {
	//得到命令参数并返回命令的类型
	string line;
	std::vector<std::string> order;
	std::getline(std::cin, line);
	std::istringstream iss(line);  // 创建字符串流
	while (iss >> line) {
		order.push_back(line);
	}
	if (order[0] == "creatproc") 
	{
		if (order.size() < 5 || ((order.size() == 5) && (order[4] != "-1"))) {
			return -2;
		}
		id = order[1];
		utime = std::stoi(order[2]);
		memsize = std::stoi(order[3]);
		iotime = std::stoi(order[4]);
		if (iotime != -1)etime = std::stoi(order[5]);
		return 0;
	}
	else if (order[0] == "killproc") {
		if (order.size() < 2) {
			return -2;
		}
		id = order[1];
		return 1;
	}
	else if (order[0] == "psproc") {
		
		return 2;
	}
	else if (order[0] == "mem") {
		return 3;
	}
	else if (order[0] == "creatfile") {
		if (order.size() < 3)return -2;
		file_name = order[1];
		file_size = std::stoi(order[2]);
		return 4;
	}
	else if (order[0] == "deletefile") {
		if (order.size() < 2)return -2;
		file_name = order[1];
		return 5;
	}
	else if (order[0] == "lsfile") {
		if (order.size() < 2)return -2;
		file_name = order[1];
		return 6;
	}
	else if (order[0] == "clean") {
		return 7;
	}
	else if (order[0] == "help") {
		return 8;
	}
	return -1;
}
//当kill进程在运行时怎么通知timetask 给一个信号量？当kill进程是当前处理进程就直接remove 如果不是呢  还要释放内存！！！！
int main() {
	OS my_os;

	//创建的时候 分配内存（给一个接口得到分配到内存的起始地址即可）
	while (1) {
		
		
	
		switch(getorder()) {
			
		case 0:	{
			my_os.create_proc(id, utime, memsize, iotime, etime);
				break;
			}
		case 1: {
			//kill 
			my_os.kill_proc(id);
			break;
		}
		case 2: {
			my_os.ls_proc();
			break;
		}
		case 3: {
			my_os.ls_mem();
			break;
		}
		case 4: {
			my_os.create_file(file_name, file_size);
			break;

		}
		case 5: {
			my_os.delete_file(file_name);
			break;
		}
		case 6: {
			my_os.ls_file(file_name);
			break;
		}
		case 7: {
			my_os.clean();
			break;
		}
		case 8: {
			my_os.help();
			break;
		}
		case -1:
		{
			Debug::ERROR_DEBUG("cmd not found you can input help get more information");
			break;
		}
			
		case -2:Debug::ERROR_DEBUG("param is too less");

		}


	}
}