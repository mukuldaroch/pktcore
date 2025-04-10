#pragma once
#include <array>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
//==============================================================================
// AVAILABLE FUNCTIONS:
// 1) write_line
// 2) read_line
// 3) print_line
// 4) getFileSize
//==============================================================================
namespace utils {

//============================================================================
// BYTE UTILS FIRST (needed by constructors)
//============================================================================
inline std::array<uint8_t, 10> int_to_10bytes(long long the_integer) {
  std::array<uint8_t, 10> result = {}; // Initialize the array with 0's

  // Convert the integer to binary and store the last 80 bits (10 bytes) in
  // the array
  for (int i = 0; i < 10; ++i) {
    result[9 - i] = (the_integer >> (i * 8)) & 0xFF; // Extract each byte
  }

  return result;
}

inline std::array<uint8_t, 8> int_to_8bytes(int64_t the_integer) {
  std::array<uint8_t, 8> result = {}; // Initialize the array with 0's

  // Convert the integer to binary and store the last 64 bits (8 bytes) in the
  // array
  for (int i = 0; i < 8; ++i) {
    result[7 - i] = (the_integer >> (i * 8)) & 0xFF; // Extract each byte
  }

  return result;
}

inline std::array<uint8_t, 5> int_to_5bytes(int the_integer) {
  std::array<uint8_t, 5> result = {}; // Initialize the array with 0's

  // Convert the integer to binary and store the last 40 bits (5 bytes) in the
  // array
  for (int i = 0; i < 5; ++i) {
    result[4 - i] = (the_integer >> (i * 8)) & 0xFF; // Extract each byte
  }

  return result;
}

inline std::array<uint8_t, 4> int_to_4bytes(uint32_t the_integer) {
  std::array<uint8_t, 4> result = {}; // Initialize the array with 0's

  // Convert the integer to binary and store the bytes in the array
  for (int i = 0; i < 4; ++i) {
    result[3 - i] = (the_integer >> (i * 8)) & 0xFF; // Extract each byte
  }

  return result;
}

inline std::array<uint8_t, 3> int_to_3bytes(int the_integer) {
  std::array<uint8_t, 3> result = {}; // Initialize the array with 0's

  // Convert the integer to binary and store the last 24 bits (3 bytes) in the
  // array
  for (int i = 0; i < 3; ++i) {
    result[2 - i] = (the_integer >> (i * 8)) & 0xFF; // Extract each byte
  }

  return result;
}

inline std::array<uint8_t, 2> int_to_2bytes(int the_integer) {
  std::array<uint8_t, 2> result = {}; // Initialize the array with 0's

  // Convert the integer to binary and store the last 16 bits (2 bytes) in the
  // array
  for (int i = 0; i < 2; ++i) {
    result[1 - i] = (the_integer >> (i * 8)) & 0xFF; // Extract each byte
  }

  return result;
}

inline std::array<uint8_t, 1> int_to_1byte(int the_integer) {
  std::array<uint8_t, 1> result = {}; // Initialize the array with 0's

  // Convert the integer to binary and store the last 8 bits (1 byte) in the
  // array
  result[0] = static_cast<uint8_t>(the_integer & 0xFF); // Extract the byte

  return result;
}
//============================================================================
template <size_t N>
inline uint64_t from_bytes(const std::array<uint8_t, N> &arr) {
  uint64_t result = 0;
  for (size_t i = 0; i < N; ++i)
    result |= static_cast<uint64_t>(arr[i]) << (8 * i);
  return result;
}

// Optional helper for 2-byte short
inline uint16_t bytes_to_short(const std::array<uint8_t, 2> &bytes) {
  return static_cast<uint16_t>(bytes[0] | (bytes[1] << 8));
}
/*
 * Read a single line from a file
 */
inline std::string read_line(const std::string &name) {
  std::ifstream var(name);
  std::string output;
  std::getline(var, output);
  return output;
}

/*
 * print a single line from a file
 */
inline void print_line(const std::string &name) {
  std::string line = read_line(name);
  std::cout << line << std::endl;
}

/*
 * Check if a file stream opened successfully
 */
inline bool check_file_open(const std::ios &file, const std::string &filename,
                            const std::string &mode) {
  if (!file) {
    std::cerr << "Error: Could not open file '" << filename << "' for " << mode
              << ".\n";
    return false;
  }
  return true;
}
/*
 * brief Get the size of a file in bytes.
 * param filename The name of the file to check.
 * return The size of the file as a `std::streampos`.
 */
inline std::streampos getFileSize(const std::string &filename) {
  std::ifstream file(filename, std::ios::binary | std::ios::ate);
  if (!file) {
    std::cerr << "Error: Could not open file '" << filename
              << "' to get size.\n";
    return -1;
  }
  return file.tellg(); // position at end
}

inline std::vector<char> readFileChunk(const std::string &filename,
                                       std::streampos start,
                                       std::streampos end) {
  std::ifstream file(filename, std::ios::binary);
  std::vector<char> buffer;

  // Ensure start < end
  if (start >= end) {
    std::cerr << "Invalid range: start >= end\n";
    return buffer;
  }

  std::streamsize chunkSize = end - start;
  buffer.resize(chunkSize);

  file.seekg(start);
  if (!file.read(buffer.data(), chunkSize)) {
    std::cerr << "Failed to read file chunk\n";
    buffer.clear();
  }

  return buffer;
}

/*
 * generate a arandom file id
 * returns the id in binary vector size 5 bytes
 */

inline std::array<uint8_t, 5> generate_file_id() {
  static bool seeded = false;
  if (!seeded) {
    srand(static_cast<unsigned>(time(0)));
    seeded = true;
  }

  const char charset[] =
      "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

  std::array<uint8_t, 5> file_id; // no need for {}
  for (int i = 0; i < 5; ++i) {
    file_id[i] = static_cast<uint8_t>(charset[rand() % (sizeof(charset) - 1)]);
  }

  return file_id;
}
} // namespace utils
