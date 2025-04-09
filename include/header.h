#pragma once
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

//============================================================================
// BYTE UTILS FIRST (needed by constructors)
//============================================================================

template <size_t N> inline std::array<uint8_t, N> int_to_bytes(uint32_t value) {
    std::array<uint8_t, N> result{};
    for (size_t i = 0; i < N; ++i) {
        result[i] = (value >> (8 * i)) & 0xFF;
    }
    return result;
}

inline uint32_t bytes_to_int(const std::array<uint8_t, 5> &bytes,
                             size_t num_bytes) {
    uint32_t result = 0;
    for (size_t i = 0; i < num_bytes && i < bytes.size(); ++i) {
        result |= static_cast<uint32_t>(bytes[i]) << (8 * i); // little-endian
    }
    return result;
}

inline uint16_t bytes_to_short(const std::array<uint8_t, 2> &bytes) {
    return static_cast<uint16_t>(bytes[0] | (bytes[1] << 8));
}

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
    std::array<uint8_t, 10> filename;
    std::array<uint8_t, 15> fileSize;

    Full_Header() = default;

    Full_Header(uint8_t ver, uint16_t part, uint16_t total, uint8_t flg,
                uint32_t payload, uint32_t packet, const std::string &fname,
                uint64_t fsize) {
        version = int_to_bytes<1>(ver);
        partNumber = int_to_bytes<2>(part);
        totalParts = int_to_bytes<2>(total);
        flag = int_to_bytes<1>(flg);
        payloadSize = int_to_bytes<5>(payload);
        packetSize = int_to_bytes<3>(packet);

        filenameSize = int_to_bytes<2>(fname.size());

        for (size_t i = 0; i < filename.size(); ++i)
            filename[i] = (i < fname.size()) ? fname[i] : 0;

        fileSize = int_to_bytes<15>(fsize); // Assuming fsize fits in 15 bytes
    }
};

struct Mini_Header {
    std::array<uint8_t, 2> partNumber;
    std::array<uint8_t, 5> payloadSize;
    std::array<uint8_t, 5> packetSize;

    Mini_Header() = default;

    Mini_Header(uint16_t part, uint32_t payload, uint32_t packet) {
        partNumber = int_to_bytes<2>(part);
        payloadSize = int_to_bytes<5>(payload);
        packetSize = int_to_bytes<5>(packet);
    }
};

//============================================================================
// FACTORY FUNCTIONS
//============================================================================

inline Full_Header header(uint16_t part, uint16_t total, uint32_t payload,
                          const std::string &filename, uint64_t fileSize,
                          uint32_t packet = 0, uint8_t version = 1,
                          uint8_t flag = 0) {
    return Full_Header(version, part, total, flag, payload, packet, filename,
                       fileSize);
}

inline Mini_Header header(uint16_t part_no, uint32_t payload, uint32_t packet_size) {
    return Mini_Header(part_no, payload, packet_size);
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
/*
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

// Still keep this if you're using 2-byte values as short:

inline uint16_t bytes_to_short(const std::array<uint8_t, 2> &arr) {
    return (static_cast<uint16_t>(arr[0]) << 8) | static_cast<uint16_t>(arr[1]);
}
//============================================================================
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
    std::cout << "  Part Number   : " << bytes_to_short(header.partNumber)
              << "\n";
    std::cout << "  Total Parts   : " << bytes_to_short(header.totalParts)
              << "\n";
    std::cout << "  Flag          : " << static_cast<int>(header.flag[0])
              << "\n";
    std::cout << "  Payload Size  : " << bytes_to_int(header.payloadSize)
              << "\n";
    std::cout << "  Packet Size   : " << bytes_to_int(header.packetSize)
              << "\n";
    std::cout << "  Filename Size : " << bytes_to_short(header.filenameSize)
              << "\n";

    std::string fname;
    for (uint8_t byte : header.filename) {
        if (byte == 0)
            break;
        fname += static_cast<char>(byte);
    }

    std::cout << "  Filename      : " << fname << "\n";
    std::cout << "  File Size     : " << bytes_to_long(header.fileSize) << "\n";
}
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

// Still keep this if you're using 2-byte values as short:

inline uint16_t bytes_to_short(const std::array<uint8_t, 2> &arr) {
    return (static_cast<uint16_t>(arr[0]) << 8) | static_cast<uint16_t>(arr[1]);
}
//============================================================================
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
    std::cout << "  Part Number   : " << bytes_to_short(header.partNumber)
              << "\n";
    std::cout << "  Total Parts   : " << bytes_to_short(header.totalParts)
              << "\n";
    std::cout << "  Flag          : " << static_cast<int>(header.flag[0])
              << "\n";
    std::cout << "  Payload Size  : " << bytes_to_int(header.payloadSize)
              << "\n";
    std::cout << "  Packet Size   : " << bytes_to_int(header.packetSize)
              << "\n";
    std::cout << "  Filename Size : " << bytes_to_short(header.filenameSize)
              << "\n";

    std::string fname;
    for (uint8_t byte : header.filename) {
        if (byte == 0)
            break;
        fname += static_cast<char>(byte);
    }

    std::cout << "  Filename      : " << fname << "\n";
    std::cout << "  File Size     : " << bytes_to_long(header.fileSize) << "\n";
}
*/
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
        std::cerr << "Failed to read full header from: " << filename << "\n";
        return;
    }

    // return Mini_Header(part_no, payload, packet_size);
    std::cout << "📦 Header Info from file: " << filename << "\n";
    std::cout << "  Part Number  : " << bytes_to_short(header.partNumber)
              << "\n";
    std::cout << "  Payload Size : " << bytes_to_int(header.payloadSize, 5)
              << "\n";
    std::cout << "  Payload Size : " << bytes_to_int(header.packetSize, 5)
              << "\n";
}
