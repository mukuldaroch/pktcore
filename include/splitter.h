#ifndef FILE_SPLITTER_H
#define FILE_SPLITTER_H

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
// 1) write_line
// 2) read_line
// 3) print_line
// 4) getFileSize
// 5) createEmptyFile
// 6) readBinaryRange
// 7) check_file_open
// 8) add_header
// 9) split (to be implemented)
//==============================================================================
namespace fsplit {

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

// Write a line to a file (overwrite mode)
inline void write_line(const std::string &str, const std::string &name) {
    std::ofstream var(name);
    if (!check_file_open(var, name, "writing"))
        return;
    var << str;
}

// Read a single line from a file
inline std::string read_line(const std::string &name) {
    std::ifstream var(name);
    if (!check_file_open(var, name, "reading"))
        return "";
    std::string output;
    std::getline(var, output);
    return output;
}

// Print a line from a file
inline void print_line(const std::string &name) {
    std::string line = read_line(name);
    std::cout << line << std::endl;
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

// Create an empty file with the given number in its name
inline bool createEmptyFile(const int &number) {
    std::string filename = "part_no_" + std::to_string(number);
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to create file: " << filename << "\n";
        return false;
    }
    return true;
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

// void add_header(const std::string &name, const int &number,
//                 const int &s_pointer, const int &e_pointer) {
//     int s;
//     return;
// }

// Dummy split function (to be implemented)
inline void split(const std::string &name, int position, int number) {
    createEmptyFile(number);
}

//=====================================================================================

} // namespace fsplit

#endif // FILE_SPLITTER_H
