#pragma once
#include "pkt_utils.h"
#include <array>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

//============================================================================
// STRUCTS
//============================================================================

struct Full_Header {
    std::array<uint8_t, 1> version;
    std::array<uint8_t, 4> partNumber;
    std::array<uint8_t, 4> totalParts;
    std::array<uint8_t, 1> flag;
    std::array<uint8_t, 8> payloadSize;
    std::array<uint8_t, 8> packetSize;
    std::array<uint8_t, 8> fileSize;
    std::array<uint8_t, 16> filename;

    Full_Header() = default;

    Full_Header(uint8_t ver, uint32_t part, uint32_t total, uint8_t flg,
                uint64_t payload, uint64_t packet, uint64_t fsize,
                const std::string &fname) {
        version[0] = ver;
        std::memcpy(partNumber.data(), &part, 4);
        std::memcpy(totalParts.data(), &total, 4);
        flag[0] = flg;
        std::memcpy(payloadSize.data(), &payload, 8);
        std::memcpy(packetSize.data(), &packet, 8);
        std::memcpy(fileSize.data(), &fsize, 8);
        // Fill filename array with characters from fname
        for (size_t i = 0; i < filename.size(); ++i)
            filename[i] = (i < fname.size()) ? fname[i] : 0;
    }
};

struct Mini_Header {
    std::array<uint8_t, 7> file_id;     // 5-byte unique ID per file
    std::array<uint8_t, 4> part_number; // 4-byte part number
    std::array<uint8_t, 4> packet_size; // 4-byte packet size

    Mini_Header() = default;

    Mini_Header(const std::array<uint8_t, 7> &file_id_input, uint32_t part,
                uint32_t packet_size_value) {
        file_id = file_id_input;
        std::memcpy(part_number.data(), &part, 4);
        std::memcpy(packet_size.data(), &packet_size_value, 4);
    }
};

//============================================================================
// FACTORY FUNCTIONS
//============================================================================
/*
version, partNumber, totalParts, flag, payloadSize, packetSize, filenameSize,
filename, fileSize,
 */
inline Full_Header header(uint8_t version, uint16_t partNumber,
                          uint16_t totalParts, uint8_t flag,
                          uint32_t payloadSize, uint32_t packetSize,
                          uint64_t fileSize, const std::string &filename) {
    // uint8_t filenameSize = static_cast<uint8_t>(filename.size());
    return Full_Header(version, partNumber, totalParts, flag, payloadSize,
                       packetSize, fileSize, filename);
}

/*
file_id, part_no, packet_size
 */
inline Mini_Header header(const std::array<uint8_t, 7> &file_id,
                          uint32_t part_no, uint32_t packet_size) {
    return Mini_Header(file_id, part_no, packet_size);
}

//============================================================================
//============================================================================
inline void Write_Full_Header(const std::string &filename,
                              const Full_Header &header) {
    std::ofstream out(filename, std::ios::binary | std::ios::trunc);
    if (!out) {
        std::cerr << "❌ Failed to open file: " << filename << "\n";
        return;
    }
    out.write(reinterpret_cast<const char *>(&header), sizeof(Full_Header));
    out.close();
}

//============================================================================
//============================================================================
inline void Write_Mini_Header(const std::string &filename,
                              const Mini_Header &header) {
    std::ofstream out(filename, std::ios::binary | std::ios::trunc);
    if (!out) {
        std::cerr << "❌ Failed to open file: " << filename << "\n";
        return;
    }
    out.write(reinterpret_cast<const char *>(&header), sizeof(Mini_Header));
    out.close();
}

//============================================================================
// PRINTER
//============================================================================
template <size_t N> uint64_t bytes_to_int(const std::array<uint8_t, N> &arr) {
    uint64_t value = 0;
    for (size_t i = 0; i < N; ++i) {
        value = (value << 8) | arr[i];
    }
    return value;
}

template <size_t N> uint64_t bytes_to_long(const std::array<uint8_t, N> &arr) {
    uint64_t value = 0;
    for (size_t i = 0; i < N; ++i) {
        value = (value << 8) | arr[i];
    }
    return value;
}
//====================================
inline void Read_And_Print_Full_Header(const std::string &filepath) {
    std::ifstream in(filepath, std::ios::binary);

    if (!in) {
        std::cerr << "❌ Failed to open file: " << filepath << "\n";
        return;
    }

    constexpr size_t headerSize = 1 + 4 + 4 + 1 + 8 + 8 + 8 + 16; // = 54 bytes
    std::vector<uint8_t> buffer(headerSize);
    in.read(reinterpret_cast<char *>(buffer.data()), buffer.size());

    if (!in) {
        std::cerr << "❌ Failed to read full header from: " << filepath << "\n";
        return;
    }

    size_t offset = 0;

    uint8_t version = buffer[offset];
    offset += 1;

    uint32_t partNumber;
    std::memcpy(&partNumber, &buffer[offset], 4);
    offset += 4;

    uint32_t totalParts;
    std::memcpy(&totalParts, &buffer[offset], 4);
    offset += 4;

    uint8_t flag = buffer[offset];
    offset += 1;

    uint64_t payloadSize;
    std::memcpy(&payloadSize, &buffer[offset], 8);
    offset += 8;

    uint64_t packetSize;
    std::memcpy(&packetSize, &buffer[offset], 8);
    offset += 8;

    uint64_t fileSize;
    std::memcpy(&fileSize, &buffer[offset], 8);
    offset += 8;

    std::string fname;
    for (size_t i = 0; i < 16; ++i) {
        if (buffer[offset + i] == 0)
            break;
        fname += static_cast<char>(buffer[offset + i]);
    }

    std::cout << "📦 Full Header Info from: " << filepath << "\n";
    std::cout << "  Version       : " << static_cast<int>(version) << "\n";
    std::cout << "  Part Number   : " << partNumber << "\n";
    std::cout << "  Total Parts   : " << totalParts << "\n";
    std::cout << "  Flag          : " << static_cast<int>(flag) << "\n";
    std::cout << "  Payload Size  : " << payloadSize << "\n";
    std::cout << "  Packet Size   : " << packetSize << "\n";
    std::cout << "  File Size     : " << fileSize << "\n";
    std::cout << "  Filename      : " << fname << "\n";
}
//============================================================================
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

    std::string file_id;
    for (int i = 0; i < 7; ++i)
        file_id += static_cast<char>(buffer[offset++]);

    uint32_t part_number;
    std::memcpy(&part_number, &buffer[offset], 4);
    offset += 4;

    uint32_t packet_size;
    std::memcpy(&packet_size, &buffer[offset], 4);

    std::cout << "📦 Header Info from file: " << filepath << "\n";
    std::cout << "  File ID      : " << file_id << "\n";
    std::cout << "  Part Number  : " << part_number << "\n";
    std::cout << "  Packet Size  : " << packet_size << "\n";
}
