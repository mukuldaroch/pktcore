#pragma once
#include "pkt_utils.h"
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

//============================================================================
// STRUCTS
//============================================================================

struct Full_Header {
    std::array<uint8_t, 1> version;
    std::array<uint8_t, 2> partNumber;
    std::array<uint8_t, 2> totalParts;
    std::array<uint8_t, 1> flag;
    std::array<uint8_t, 5> payloadSize;
    std::array<uint8_t, 3> packetSize;
    std::array<uint8_t, 2> filenameSize;
    std::array<uint8_t, 20> filename;
    std::array<uint8_t, 5> fileSize;

    Full_Header() = default;

    Full_Header(uint8_t ver, uint16_t part, uint16_t total, uint8_t flg,
                uint32_t payload, uint32_t packet, uint32_t fnameSize,
                const std::string &fname, uint64_t fsize) {
        version = utils::int_to_1byte(ver);
        partNumber = utils::int_to_2bytes(part);  // Correct usage of function
        totalParts = utils::int_to_2bytes(total); // Correct usage of function
        flag = utils::int_to_1byte(flg);          // Correct usage of function
        payloadSize =
            utils::int_to_5bytes(payload);         // Correct usage of function
        packetSize = utils::int_to_3bytes(packet); // Correct usage of function

        filenameSize =
            utils::int_to_2bytes(fname.size()); // Size of filename in bytes

        // Fill filename array with characters from fname
        for (size_t i = 0; i < filename.size(); ++i)
            filename[i] = (i < fname.size())
                              ? fname[i]
                              : 0; // Zero padding if fname is shorter

        fileSize = utils::int_to_5bytes(
            fsize); // Convert file size with correct function
    }
};

struct Mini_Header {
    std::array<uint8_t, 5> file_id;     // 5-byte unique ID per file
    std::array<uint8_t, 1> part_number; // 1-byte part number (0–255)
    std::array<uint8_t, 4> packet_size; // 4-byte chunk size (big endian maybe)

    Mini_Header() = default;

    Mini_Header(const std::array<uint8_t, 5> &file_id_input, uint8_t part,
                uint32_t packet_size_value) {
        file_id = file_id_input;
        part_number = {part};
        packet_size = utils::int_to_4bytes(packet_size_value);
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
                          uint32_t filenameSize, const std::string &filename,
                          uint64_t fileSize) {
    // uint8_t filenameSize = static_cast<uint8_t>(filename.size());
    return Full_Header(version, partNumber, totalParts, flag, payloadSize,
                       packetSize, filenameSize, filename, fileSize);
}

/*
file_id, part_no, packet_size
 */
inline Mini_Header header(const std::array<uint8_t, 5> &file_id,
                          uint8_t part_no, uint32_t packet_size) {
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
inline void Read_And_Print_Full_Header(const std::string &filename) {
    Full_Header header;
    std::ifstream in(filename, std::ios::binary);

    if (!in) {
        std::cerr << "❌ Failed to open file: " << filename << "\n";
        return;
    }

    in.read(reinterpret_cast<char *>(&header), sizeof(Full_Header));
    if (!in) {
        std::cerr << "❌ Failed to read full header from: " << filename << "\n";
        return;
    }

    std::cout << "📦 Full Header Info from: " << filename << "\n";
    std::cout << "  Version       : " << static_cast<int>(header.version[0])
              << "\n";
    std::cout << "  Part Number   : "
              << utils::bytes_to_short(header.partNumber) << "\n";
    std::cout << "  Total Parts   : "
              << utils::bytes_to_short(header.totalParts) << "\n";
    std::cout << "  Flag          : " << static_cast<int>(header.flag[0])
              << "\n";
    std::cout << "  Payload Size  : " << bytes_to_int(header.payloadSize)
              << "\n";
    std::cout << "  Packet Size   : " << bytes_to_int(header.packetSize)
              << "\n";
    std::cout << "  Filename Size : "
              << utils::bytes_to_short(header.filenameSize) << "\n";

    std::string fname;
    for (uint8_t byte : header.filename) {
        if (byte == 0)
            break;
        fname += static_cast<char>(byte);
    }

    std::cout << "  Filename      : " << fname << "\n";
    std::cout << "  File Size     : " << bytes_to_long(header.fileSize) << "\n";
}
//============================================================================
inline void Read_And_Print_Mini_Header(const std::string &filename) {
    Mini_Header header;
    std::ifstream in(filename, std::ios::binary);

    if (!in) {
        std::cerr << "Failed to open file: " << filename << "\n";
        return;
    }

    in.read(reinterpret_cast<char *>(&header), sizeof(Mini_Header));
    if (!in) {
        std::cerr << "Failed to read mini header from: " << filename << "\n";
        return;
    }

    std::cout << "📦 Header Info from file: " << filename << "\n";

    std::cout << "  File ID      : ";
    for (auto byte : header.file_id) {
        std::cout << static_cast<char>(byte);
    }
    std::cout << "\n";

    std::cout << "  Part Number  : " << static_cast<int>(header.part_number[0])
              << "\n";
    std::cout << "  Packet Size  : " << bytes_to_int(header.packet_size)
              << "\n";
}
