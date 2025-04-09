#pragma once
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
namespace combiner {

struct Combiner_Full_Header {
    std::array<uint8_t, 1> version;
    std::array<uint8_t, 2> partNumber;
    std::array<uint8_t, 2> totalParts;
    std::array<uint8_t, 1> flag;
    std::array<uint8_t, 5> payloadSize;
    std::array<uint8_t, 5> packetSize;
    std::array<uint8_t, 5> reserved;
};

inline uint32_t bytes_to_int(const std::array<uint8_t, 5> &bytes,
                             size_t num_bytes) {
    uint32_t result = 0;
    for (size_t i = 0; i < num_bytes && i < bytes.size(); ++i) {
        result |= static_cast<uint32_t>(bytes[i]) << (8 * i); // Little endian
    }
    return result;
}

inline uint16_t bytes_to_short(const std::array<uint8_t, 2> &bytes) {
    return static_cast<uint16_t>(bytes[0] | (bytes[1] << 8)); // Little endian
}

inline void read_and_print_header(const std::string &filename) {
    Combiner_Full_Header header{};
    std::ifstream in(filename, std::ios::binary);

    if (!in) {
        std::cerr << "Failed to open file: " << filename << "\n";
        return;
    }

    in.read(reinterpret_cast<char *>(&header), sizeof(Combiner_Full_Header));
    if (!in) {
        std::cerr << "Failed to read full header from: " << filename << "\n";
        return;
    }

    std::cout << "📦 Combiner_Full_Header Info from file: " << filename << "\n";
    // std::cout << "  Version      : " << static_cast<int>(header.version[0])
    // << "\n";
    std::cout << "  Part Number  : " << bytes_to_short(header.partNumber)
              << "\n";
    std::cout << "  Total Parts  : " << bytes_to_short(header.totalParts)
              << "\n";
    // std::cout << "  Flag         : " << static_cast<int>(header.flag[0]) <<
    // "\n";
    std::cout << "  Payload Size : " << bytes_to_int(header.payloadSize, 5)
              << "\n";
    // std::cout << "  Packet Size  : " << bytes_to_int(header.packetSize, 5) <<
    // "\n";
}
} // namespace combiner
