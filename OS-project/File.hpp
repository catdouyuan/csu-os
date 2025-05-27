#ifndef FILE_HPP
#define FILE_HPP

#include "common.hpp"
#include "excp.hpp"
#include "Debug.hpp"
#include <sstream>
#include <ctime>
#include <iomanip>
#include <bitset>
#include <vector>
#include <map>
#include <utility>

struct File {
    std::string file_name;
    std::string creat_time;
    int file_size;
    std::pair<int, int> index_block;

    File();
    File(std::string name, int file_size, std::pair<int, int> index_block);
};

class File_manager {
private:
    static const int disk_num;
    static const int block_num;
    static const int block_size;
    static int used_num;

    static std::map<std::string, File> file_map;
    static std::vector<std::vector<std::string>> Disk;
    static std::vector<std::bitset<100>> bitmap;

    static std::vector<int> parse_index_block(std::pair<int, int> index);
    static std::pair<std::vector<int>, int> alloca_file(int disk_num, int block_size);
    static int is_allco(int block_size);
    static std::string unparse(std::vector<int> arr);
    static void free_file(std::pair<int, int> index_block);

public:
    static void create_file(std::string file_name, int file_size);
    static void delete_file(std::string file_name);
    static void show_file(std::string file_name);
};

#endif