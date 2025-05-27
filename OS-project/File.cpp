#include "File.hpp"
#include <sstream>
#include <ctime>
#include <iomanip>

const  int File_manager::disk_num=10;
const  int File_manager::block_num=100;
const  int File_manager::block_size = 4;
int File_manager::used_num = 0;
vector<vector<string>> File_manager::Disk(File_manager::disk_num, vector<string>(File_manager::block_num));
vector<std::bitset<File_manager::block_num>> File_manager::bitmap(File_manager::disk_num);
map<string, File>  File_manager::file_map;




File::File() : file_size(0), index_block({ -1, -1 }) {}

File::File(std::string name, int file_size, std::pair<int, int> index_block) {
    std::time_t t = std::time(nullptr);
    std::tm* local_tm = std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(local_tm, "%Y-%m-%d %H:%M:%S");

    this->creat_time = oss.str();
    this->file_name = name;
    this->file_size = file_size;
    this->index_block = index_block;
}

std::vector<int> File_manager::parse_index_block(std::pair<int, int> index) {
    auto s = Disk[index.first][index.second];
    std::istringstream ss(s);
    std::vector<int> data_block;
    int block;

    while (ss >> block) {
        data_block.push_back(block);
    }
    return data_block;
}

std::pair<std::vector<int>, int> File_manager::alloca_file(int disk_num, int block_size) {
    std::vector<int> allco;
    int cnt = 0;
    int i;

    for (i = 0; cnt < block_size - 1; i++) {
        if (bitmap[disk_num].test(i)) continue;
        bitmap[disk_num].set(i);
        cnt++;
        allco.push_back(i);
    }

    int ans;
    for (; i < File_manager::block_num; i++) {
        if (!bitmap[disk_num].test(i)) {
            ans = i;
            break;
        }
    }

    return { allco, ans };
}

int File_manager::is_allco(int block_size) {
    for (int i = 0; i < File_manager::disk_num; i++) {
        if (File_manager::block_num - bitmap[i].count() > block_size) {
            return i;
        }
    }
    return -1;
}

std::string File_manager::unparse(std::vector<int> arr) {
    std::ostringstream oss;
    for (int i = 0; i < arr.size(); i++) {
        oss << arr[i];
        if (i == arr.size() - 1) continue;
        oss << ' ';
    }
    return oss.str();
}

void File_manager::free_file(std::pair<int, int> index_block) {
    auto disk_index = index_block.first;
    auto block_arr = parse_index_block(index_block);

    // 释放位图标记
    for (auto a : block_arr) {
        bitmap[disk_index].reset(a);
    }
    bitmap[disk_index].reset(index_block.second);

    // 释放索引块的记录
    Disk[disk_index][index_block.second] = "";
}

void File_manager::create_file(std::string file_name, int file_size) {
    if (file_map.count(file_name)) {
        Debug::ERROR_DEBUG("file name has exist");
        return;
    }

    int need_block = (file_size + block_size - 1) / block_size + 1;
    int allco_disk = is_allco(need_block);

    if (allco_disk == -1) {
        Debug::ERROR_DEBUG("file_size is too big");
        return;
    }

    auto allco_arr = alloca_file(allco_disk, need_block);
    std::string unparse_string = unparse(allco_arr.first);
    Disk[allco_disk][allco_arr.second] = unparse_string;

    File allco_file{ file_name, file_size, std::make_pair(allco_disk, allco_arr.second) };
    file_map[file_name] = allco_file;
    Debug::OUTPUT_DEBUG("alloc file success!!!");
}

void File_manager::delete_file(std::string file_name) {
    if (!file_map.count(file_name)) {
        Debug::ERROR_DEBUG("file not exist");
        return;
    }

    auto delete_file = file_map[file_name];
    file_map.erase(file_name);
    free_file(delete_file.index_block);
    Debug::OUTPUT_DEBUG("delete " + file_name + " success!!!");
}

void File_manager::show_file(std::string file_name) {
    if (!file_map.count(file_name)) {
        Debug::ERROR_DEBUG("file not exist");
        return;
    }
    Debug::LIST_DEBUG("file_list");
    auto ls_file = file_map[file_name];
    Debug::INFO_DEBUG("file_name: " + ls_file.file_name +
        " create_time: " + ls_file.creat_time +
        " file_size: " + std::to_string(ls_file.file_size) +
        " disk: " + std::to_string(ls_file.index_block.first) +
        " index_blcok: " + std::to_string(ls_file.index_block.second) +
        " data_block: " + Disk[ls_file.index_block.first][ls_file.index_block.second]);
}