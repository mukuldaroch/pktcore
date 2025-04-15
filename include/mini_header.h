#pragma once
#include <array>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
namespace header {
/*
 * Mini_Header: Structure representing a minimal version of the header.
 * This can contain just the essential identifiers or a compact set of
 * data for a quicker or smaller header format.
 */
struct Mini_Header {
    std::array<uint8_t, 5> PKTCORE;
    std::array<uint8_t, 5> file_id;     // 5-byte unique ID per file
    std::array<uint8_t, 4> part_number; // 4-byte part number
    std::array<uint8_t, 4> packet_size; // 4-byte packet size
    uint8_t flag;

    Mini_Header() = default;

    /*
    file_id, part_no, packet_size
     */
    Mini_Header(const std::array<uint8_t, 5> &file_id_input, uint32_t part,
                uint32_t packet_size_value) {
        file_id = file_id_input;
        std::memcpy(PKTCORE.data(), "PCORE", 5);
        std::memcpy(part_number.data(), &part, 4);
        std::memcpy(packet_size.data(), &packet_size_value, 4);
    }
};

/*
file_id, part_no, packet_size
 */
inline Mini_Header MINI_HEADER(const std::array<uint8_t, 5> &file_id,
                               uint32_t part_no, uint32_t packet_size) {
    return Mini_Header(file_id, part_no, packet_size);
}

/*
 * Write_Mini_Header:
 * This function writes a mini header to a given file.
 * - @filename: The name of the file where the header will be written.
 * - @header: The Mini_Header structure that contains the mini header data.
 * Similar to Write_Full_Header, but this function handles only the compact
 * set of information contained in the Mini_Header.
 */
inline void WRITE_MINI_HEADER(const std::string &filename,
                              const Mini_Header &header) {
    std::ofstream out(filename, std::ios::binary | std::ios::trunc);
    if (!out) {
        std::cerr << "❌ Failed to open file: " << filename << "\n";
        return;
    }
    out.write(reinterpret_cast<const char *>(&header), sizeof(Mini_Header));
    out.close();
}

/*
 * Read_And_Print_Mini_Header:
 * This function reads the mini header from a file and prints its contents.
 * - @filepath: The path to the file from which the mini header will be read.
 * Similar to Read_And_Print_Full_Header, but works with the compact mini
 * header.
 */
inline void Read_And_Print_Mini_Header(const std::string &filepath) {
    std::ifstream in(filepath, std::ios::binary);

    if (!in) {
        std::cerr << "Failed to open file: " << filepath << "\n";
        return;
    }

    constexpr size_t headerSize = 7 + 4 + 4; //  15 bytes
    std::vector<uint8_t> buffer(headerSize);
    in.read(reinterpret_cast<char *>(buffer.data()), buffer.size());

    if (!in) {
        std::cerr << "Failed to read mini header from: " << filepath << "\n";
        return;
    }

    size_t offset = 0;

    std::string extention;
    for (int i = 0; i < 5; ++i)
        extention += static_cast<char>(buffer[offset++]);

    std::string file_id;
    for (int i = 0; i < 5; ++i)
        file_id += static_cast<char>(buffer[offset++]);

    uint32_t part_number;
    std::memcpy(&part_number, &buffer[offset], 4);
    offset += 4;

    uint32_t packet_size;
    std::memcpy(&packet_size, &buffer[offset], 4);

    std::cout << "📦 Header Info from file: " << filepath << "\n";
    std::cout << "   Extention    : " << extention << "\n";
    std::cout << "   File ID      : " << file_id << "\n";
    std::cout << "   Part Number  : " << part_number << "\n";
    std::cout << "   Packet Size  : " << packet_size << "\n";
}
} // namespace header
