#pragma once
#include <cstdint> // for uint8_t, uint16_t, uint32_t
#include <cstring> // for memset
#include <fstream>

struct FSplitHeader {
    uint8_t partNumber[2]; // 2 bytes
    uint8_t totalParts[2]; // 2 bytes
    uint8_t chunkSize[4];  // 4 bytes
    uint8_t reserved[2];   // 2 bytes padding (optional)

    // Constructor: takes normal integers and converts to bytes
    FSplitHeader(uint16_t part, uint16_t total, uint32_t chunk) {
        partNumber[0] = part & 0xFF;
        partNumber[1] = (part >> 8) & 0xFF;

        totalParts[0] = total & 0xFF;
        totalParts[1] = (total >> 8) & 0xFF;

        chunkSize[0] = chunk & 0xFF;
        chunkSize[1] = (chunk >> 8) & 0xFF;
        chunkSize[2] = (chunk >> 16) & 0xFF;
        chunkSize[3] = (chunk >> 24) & 0xFF;

        std::memset(reserved, 0, sizeof(reserved));
    }
};

// Factory function to easily create headers
inline FSplitHeader make_header(int partNumber, int totalParts, int chunkSize) {
    return FSplitHeader(partNumber, totalParts, chunkSize);
}
// funciton to insert the header in a file
void write_header_to_file(const std::string &filename,
                          const FSplitHeader &header) {
    std::ofstream out(filename, std::ios::binary | std::ios::trunc);
    // if (!out) {
    //     std::cerr << "Error opening file: " << filename << '\n';
    //     return;
    // }
    out.write(reinterpret_cast<const char *>(&header), sizeof(FSplitHeader));
    out.close();
}
//
