#pragma once
#include <fstream>
#include <iostream>
#include <string>
//==============================================================================
// AVAILABLE FUNCTIONS:
// 1) write_line
// 2) read_line
// 3) print_line
// 4) getFileSize
//==============================================================================
namespace utils {

// Read a single line from a file
inline std::string read_line(const std::string &name) {
    std::ifstream var(name);
    std::string output;
    std::getline(var, output);
    return output;
}

// Print a line from a file
inline void print_line(const std::string &name) {
    std::string line = read_line(name);
    std::cout << line << std::endl;
}
// Check if a file stream opened successfully
inline bool check_file_open(const std::ios &file, const std::string &filename,
                            const std::string &mode) {
    if (!file) {
        std::cerr << "Error: Could not open file '" << filename << "' for "
                  << mode << ".\n";
        return false;
    }
    return true;
}

// Get the size of a file in bytes
inline std::streampos getFileSize(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Error: Could not open file '" << filename
                  << "' to get size.\n";
        return -1;
    }
    return file.tellg(); // position at end
}
} // namespace utils
