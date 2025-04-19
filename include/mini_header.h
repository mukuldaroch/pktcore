#pragma once
#include <array>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/*
 * global functions
 */
namespace header {

/*
 * Mini_Header: Structure representing a minimal version of the header for each
 * packet This contains just the essential identifiers for each packet
 */
struct Mini_Header {
    std::array<uint8_t, 5> PKTCORE;     // constant extention
    std::array<uint8_t, 5> file_id;     // 5-byte unique ID per file
    std::array<uint8_t, 4> packet_no;   // 4-byte part number
    std::array<uint8_t, 4> payload_len; // 4-byte packet size
    uint8_t flag;

    /*
     * constructor
     */
    Mini_Header(const std::array<uint8_t, 5> &file_id_input, uint32_t part,
                uint32_t payload_size_value) {
        file_id = file_id_input;
        std::memcpy(PKTCORE.data(), "PCORE", 5);
        std::memcpy(packet_no.data(), &part, 4);
        std::memcpy(payload_len.data(), &payload_size_value, 4);
    }
};

/*
 * Global constructor for MINI_HEADER
 * - @file_id      :randomly genrated file_id for every packet of file
 * - @ packet_no   :to combine each packet orderwise
 * - @ payload_size :contains size of payload
 */
inline Mini_Header MINI_HEADER(const std::array<uint8_t, 5> &file_id,
                               uint32_t packet_no, uint32_t payload_size);

/*
 * Write_Mini_Header:
 * This function writes a Mini_Header to a given file.
 * - @filename: The name of the file where the header will be written.
 * - @header: The Mini_Header structure that contains the mini header data.
 */
inline void WRITE_MINI_HEADER(const std::string &filename,
                              const Mini_Header &header);

/*
 * Read_And_Print_Mini_Header:
 * This function reads the mini header from a file and prints its contents.
 * - @filename: The path to the file from which the full header will be read.
 * and print it in a human-readable format (for debugging).
 */
inline void Print_Mini_Header(const std::string &filename);

//=================================================================================
//=================================================================================
// function coding here
// these files are here for easy debugging
// later when the projects gets bigger and this headr file will be bug free
// we will move this section to different file
//
inline Mini_Header MINI_HEADER(const std::array<uint8_t, 5> &file_id,
                               uint32_t packet_no, uint32_t payload_size) {
    return Mini_Header(file_id, packet_no, payload_size);
}

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

inline void Print_Mini_Header(const std::string &filepath) {
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

    uint32_t packet_no;
    std::memcpy(&packet_no, &buffer[offset], 4);
    offset += 4;

    uint32_t packet_size;
    std::memcpy(&packet_size, &buffer[offset], 4);

    std::cout << "📦 Header Info from file: " << filepath << "\n";
    std::cout << "   Extention      : " << extention << "\n";
    std::cout << "   File ID        : " << file_id << "\n";
    std::cout << "   Part Number    : " << packet_no << "\n";
    std::cout << "   Payload Length : " << packet_size << "\n";
}
} // namespace header
