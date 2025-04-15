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
 * Full_Header: Structure representing the full header of the file.
 * This will typically contain metadata such as file ID, size, or any
 * other critical information that identifies and characterizes the file.
 */
struct Full_Header {
    std::array<uint8_t, 5> PKTCORE;
    std::array<uint8_t, 5> file_id;
    std::array<uint8_t, 4> part_number;
    std::array<uint8_t, 4> no_of_parts;
    std::array<uint8_t, 1> flags;
    std::array<uint8_t, 4> payloadSize;
    std::array<uint8_t, 8> filesize;
    std::array<uint8_t, 19> filename;

    Full_Header() = default;

    /*
     file_id, totalParts, flags, payloadSize, filename, fileSize,
     */
    Full_Header(const std::array<uint8_t, 5> &file_id_input,
                uint32_t part_number0, uint32_t num_parts, uint8_t flag_val,
                uint32_t payloadsize, uint64_t fsize,
                const std::string &fname) {
        std::memcpy(PKTCORE.data(), "PCORE", 5);
        file_id = file_id_input;
        std::memcpy(part_number.data(), &part_number0, 4);
        std::memcpy(no_of_parts.data(), &num_parts, 4);
        flags[0] = flag_val;
        std::memcpy(payloadSize.data(), &payloadsize, 4);
        std::memcpy(filesize.data(), &fsize, 8);
        // Fill filename array with characters from fname
        for (size_t i = 0; i < filename.size(); ++i)
            filename[i] = (i < fname.size()) ? fname[i] : 0;
    }
};

/*
version, partNumber, totalParts, flag, payloadSize, packetSize, filenameSize,
filename, fileSize,
 */
inline Full_Header FULL_HEADER(const std::array<uint8_t, 5> &file_id_input,
                               uint32_t part_number0, uint32_t num_parts,
                               uint8_t flag_val, uint32_t payloadsize,
                               uint64_t fsize, const std::string &fname) {
    return Full_Header(file_id_input, part_number0, num_parts, flag_val,
                       payloadsize, fsize, fname);
}

/*
 * Write_Full_Header:
 * This function writes a full header to a given file.
 * - @filename: The name of the file where the header will be written.
 * - @header: The Full_Header structure that contains the full header data.
 * This function will perform necessary checks, encode the header, and
 * save it to the specified file.
 */
inline void WRITE_FULL_HEADER(const std::string &filename,
                              const Full_Header &header) {
    std::ofstream out(filename, std::ios::binary | std::ios::trunc);
    if (!out) {
        std::cerr << "❌ Failed to open file: " << filename << "\n";
        return;
    }
    out.write(reinterpret_cast<const char *>(&header), sizeof(Full_Header));
    out.close();
}

/*
 * Read_And_Print_Full_Header:
 * This function reads the full header from a file and prints its contents.
 * - @filepath: The path to the file from which the full header will be read.
 * This will typically read the file's header, interpret the contents,
 * and print it in a human-readable format (for debugging or logging).
 */
inline void Print_Full_Header(const std::string &filepath) {
    std::ifstream in(filepath, std::ios::binary);

    if (!in) {
        std::cerr << "❌ Failed to open file: " << filepath << "\n";
        return;
    }

    constexpr size_t headerSize = 7 + 4 + 4 + 1 + 4 + 8 + 20; // = 54 bytes
    std::vector<uint8_t> buffer(headerSize);
    in.read(reinterpret_cast<char *>(buffer.data()), buffer.size());

    if (!in) {
        std::cerr << "❌ Failed to read full header from: " << filepath << "\n";
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
    uint32_t totalParts;
    std::memcpy(&totalParts, &buffer[offset], 4);

    offset += 4;
    uint8_t flag = buffer[offset];

    offset += 1;
    uint32_t payloadSize;
    std::memcpy(&payloadSize, &buffer[offset], 4);

    offset += 4;
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
    std::cout << "  Extention     : " << extention << "\n";
    std::cout << "  file_id       : " << file_id << "\n";
    std::cout << "  part Number   : " << part_number << "\n";
    std::cout << "  Total Parts   : " << totalParts << "\n";
    std::cout << "  Flag          : " << static_cast<int>(flag) << "\n";
    std::cout << "  Payload Size  : " << payloadSize << "\n";
    std::cout << "  File Size     : " << fileSize << "\n";
    std::cout << "  Filename      : " << fname << "\n";
}
} // namespace header
