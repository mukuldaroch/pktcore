#pragma once
#include "pkt_utils.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//==============================================================================
// NOTES:
// - Go to byte 0 (start):         file.seekg(0, std::ios::beg);
// - Go 5 bytes ahead:             file.seekg(5, std::ios::cur);
// - Go 3 bytes before end:        file.seekg(-3, std::ios::end);
//
// AVAILABLE FUNCTIONS:
// 1) createEmptyFile
// 2) readBinaryRange
// 5) split (to be implemented)
//==============================================================================
namespace split {

// Create an empty file with the given number in its name
inline std::string createEmptyFile(const int &number) {
    std::string filename = "part_no_" + std::to_string(number);
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to create file: " << filename << "\n";
        return "";
    }
    return filename;
}

// Read binary data from position a to b
inline std::vector<char> readBinaryRange(const std::string &filename,
                                         std::streampos a, std::streampos b) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not open file '" << filename
                  << "' for reading binary range.\n";
        return {};
    }

    std::streamsize size = b - a;
    if (size <= 0) {
        std::cerr << "Invalid read range: " << a << " to " << b << "\n";
        return {};
    }

    std::vector<char> buffer(size);
    file.seekg(a);
    file.read(buffer.data(), size);
    buffer.resize(file.gcount()); // keep only what's read
    return buffer;
}
//=====================================================================================
} // namespace split
