#pragma once
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
// namespace fs = std::filesystem;

namespace search {
/*
1. Current directory
list_files(".");
This lists files in the current directory — where the program is being run.

2. Subdirectory
list_files("chunks");
If there's a folder named chunks/ in the current working directory, this will
list all its contents.

3. Go up one level
list_files("..");
This lists files in the parent directory.

4. Absolute path (if you want)
list_files("/home/yourname/projects/pktcore/chunks");
Works fine too — absolute paths are always valid.

To check where you are (cwd):
You can print the current working directory like this:

std::cout << std::filesystem::current_path() << "\n";

*/
inline void list_files(const std::filesystem::path &path) {
    for (const auto &entry : std::filesystem::directory_iterator(path)) {
        std::cout << entry.path() << std::endl;
    }
}

inline void Search_FullHeader(const std::filesystem::path &path) {
    for (const auto &entry : std::filesystem::directory_iterator(path)) {
        if (std::filesystem::is_regular_file(entry)) {
            std::string filename = entry.path().string();
            std::cout << "Found file: " << filename << "\n";

            // You can now open this file like:
            std::ifstream file(filename, std::ios::binary);
            if (!file) {
                std::cerr << "Failed to open: " << filename << "\n";
                continue;
            }

            // Example: read first 11 bytes (7 for fileID + 4 for partno)
            // char buffer[11];
            // file.read(buffer, 11);

            constexpr size_t headerSize = 7 + 4 + 4; //  15 bytes
            std::vector<uint8_t> buffer(headerSize);
            file.read(reinterpret_cast<char *>(buffer.data()), buffer.size());

            size_t offset = 0;

            std::string file_id;
            for (int i = 0; i < 7; ++i)
                file_id += static_cast<char>(buffer[offset++]);

            uint32_t part_number;
            std::memcpy(&part_number, &buffer[offset], 4);
            offset += 4;

            uint32_t packet_size;
            std::memcpy(&packet_size, &buffer[offset], 4);

            std::cout << "📦 Header Info from file: " << path << "\n";
            std::cout << "   File ID      : " << file_id << "\n";
            std::cout << "   Part Number  : " << part_number << "\n";

            // Do your logic here...
            file.close();
        }
    }
}
} // namespace search
