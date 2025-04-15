#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
// namespace fs = std::filesystem;

namespace combiner {

// void list_files(const std::filesystem::path &path) {
//     for (const auto &entry : std::filesystem::directory_iterator(path)) {
//         std::cout << entry.path() << std::endl;
//     }
// }
//

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
            char buffer[11];
            file.read(buffer, 11);

            // Do your logic here...
            file.close();
        }
    }
}
} // namespace combiner
