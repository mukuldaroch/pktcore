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
/*
 */
struct Mini_Header {
  std::array<uint8_t, 7> file_id;     // 5-byte unique ID per file
  std::array<uint8_t, 4> part_number; // 4-byte part number
  std::array<uint8_t, 4> packet_size; // 4-byte packet size

  // Mini_Header() = default;

  /*
  file_id, part_no, packet_size
   */
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
file_id, part_no, packet_size
 */
inline Mini_Header header(const std::array<uint8_t, 7> &file_id,
                          uint32_t part_no, uint32_t packet_size) {
  return Mini_Header(file_id, part_no, packet_size);
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
// template <size_t N> uint64_t bytes_to_int(const std::array<uint8_t, N> &arr)
// {
//     uint64_t value = 0;
//     for (size_t i = 0; i < N; ++i) {
//         value = (value << 8) | arr[i];
//     }
//     return value;
// }
//
// template <size_t N> uint64_t bytes_to_long(const std::array<uint8_t, N> &arr)
// {
//     uint64_t value = 0;
//     for (size_t i = 0; i < N; ++i) {
//         value = (value << 8) | arr[i];
//     }
//     return value;
// }
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
  std::cout << "   File ID      : " << file_id << "\n";
  std::cout << "   Part Number  : " << part_number << "\n";
  std::cout << "   Packet Size  : " << packet_size << "\n";
}
