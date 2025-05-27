#include <iostream>	
#include <queue>
#include "Link.hpp"
#include "timetask.hpp"
#include <string>
#include <sstream>
#include <set>
#include "OS.hpp"
using namespace std;
//���� �ڴ� ���
//��ʱ���� ʱ��Ƭ ��ʱ��
map<string, pair<pro_status,int >>proc_status;
std::set<std::string> pro_id;
//TODO ����һ������������Ϣ����Ĳ��� psproc
//TODO file_size �ӵ�λ

string id, file_name;
int file_size;
int  utime, memsize, iotime, etime = -1;
int  getorder() {
	//�õ�����������������������
	string line;
	std::vector<std::string> order;
	std::getline(std::cin, line);
	std::istringstream iss(line);  // �����ַ�����
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
//��kill����������ʱ��ô֪ͨtimetask ��һ���ź�������kill�����ǵ�ǰ������̾�ֱ��remove ���������  ��Ҫ�ͷ��ڴ棡������
int main() {
	OS my_os;

	//������ʱ�� �����ڴ棨��һ���ӿڵõ����䵽�ڴ����ʼ��ַ���ɣ�
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