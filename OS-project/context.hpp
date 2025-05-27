#include <set>
#include <map>
#ifndef CONTEXT_HPP  
#define CONTEXT_HPP 
enum pro_status
{
	CREATE,
	PREPARE,
	UNDO,
	BLOCK,
	WORK,
	FINISH,
	IO


};
extern map<string, pair<pro_status,int>>proc_status;
extern set<string>pro_id;
const int MEM_SIZE = 2048;//Ä¬ÈÏkb´óÐ¡
#endif