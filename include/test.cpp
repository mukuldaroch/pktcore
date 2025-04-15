#include "splitter.h"
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<uint8_t> FETCH_BYTES(const std::string &filename, size_t byte_start,
                                 size_t byte_end) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Could not open file: " << filename << "\n";
        return {};
    }

    if (byte_end <= byte_start) {
        std::cerr << "Invalid byte range.\n";
        return {};
    }

    file.seekg(byte_start);
    size_t num_bytes = byte_end - byte_start;
    std::vector<uint8_t> bytes(num_bytes);
    file.read(reinterpret_cast<char *>(bytes.data()), num_bytes);

    if (file.gcount() != num_bytes) {
        std::cerr << "Error reading the expected number of bytes.\n";
        return {};
    }
    return bytes;
}

int main() {
    splitter::SPLITTER();
    return 0;
}
