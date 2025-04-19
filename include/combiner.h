#include "explorer.h"
#include "pkt_utils.h"
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

/*
 * local functions
 */
struct MinHeapNode {
    uint32_t split_no;
    std::string filename;

    MinHeapNode(uint32_t sn, const std::string &fn)
        : split_no(sn), filename(fn) {}
};

struct CompareSplitNo {
    bool operator()(const MinHeapNode &a, const MinHeapNode &b) {
        return a.split_no > b.split_no; // Min-heap: smallest split number first
    }
};

/*
 * global functions
 */
namespace combiner {

inline std::string Detect_PCORE_Files() {
    std::map<std::string, std::string> fileID_name; // fileID → filename
    std::vector<std::string> file_names;

    for (const auto &file : utils::FETCH_FILES(".")) {
        std::ifstream current(file, std::ios::binary);

        char ext[6] = {0};
        current.read(ext, 5);
        if (std::string(ext) != "PCORE") {
            std::cout << "Skipping non-PCORE file: " << file << '\n';
            continue;
        }

        char file_id_buf[6] = {0};
        current.read(file_id_buf, 5);
        std::string file_id(file_id_buf);

        uint32_t split_no;
        current.read(reinterpret_cast<char *>(&split_no), 4);

        if (split_no == 0) {
            current.seekg(31); // Skipping to position where filename is stored
            char filename_buf[20] = {0};
            current.read(filename_buf, 19);
            std::string real_filename(filename_buf);

            // Save in vector and map
            file_names.push_back(real_filename);
            fileID_name[file_id] = real_filename;
        }
    }

    // Let user select filename
    std::string selected_filename = tui::SHOW_SELECT_FILES(file_names);

    // Find file_id for the selected filename
    for (const auto &pair : fileID_name) {
        if (pair.second == selected_filename) {
            return pair.first; // Return the file ID
        }
    }

    // If not found
    return "";
}

inline std::priority_queue<MinHeapNode, std::vector<MinHeapNode>,
                           CompareSplitNo>
BuildMinHeapForFile(const std::string &target_file_id) {

    std::priority_queue<MinHeapNode, std::vector<MinHeapNode>, CompareSplitNo>
        min_heap;

    for (const auto &file : utils::FETCH_FILES(".")) {
        std::ifstream current(file, std::ios::binary);
        if (!current)
            continue;

        char ext[6] = {0};
        current.read(ext, 5);
        if (std::string(ext) != "PCORE")
            continue;

        char file_id_buf[6] = {0};
        current.read(file_id_buf, 5);
        std::string file_id(file_id_buf);

        if (file_id != target_file_id)
            continue;

        uint32_t split_no;
        current.read(reinterpret_cast<char *>(&split_no), 4);

        if (split_no != 0) {
            min_heap.emplace(split_no, file);
        }
    }

    return min_heap;
}

inline void PrintMinHeap(
    std::priority_queue<MinHeapNode, std::vector<MinHeapNode>, CompareSplitNo>
        heap) {
    std::cout << "Min-Heap contents (split_no -> filename):\n";
    while (!heap.empty()) {
        const auto &node = heap.top();
        std::cout << node.split_no << " -> " << node.filename << '\n';
        heap.pop();
    }
}
} // namespace combiner
