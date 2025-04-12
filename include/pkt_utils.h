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
// 1) readBitsFromRange
// 2)
// 3)
// 4) getFileSize
//==============================================================================
namespace utils {

// - Go to byte 0 (start):         file.seekg(0, std::ios::beg);
// - Go 5 bytes ahead:             file.seekg(5, std::ios::cur);
// - Go 3 bytes before end:        file.seekg(-3, std::ios::end);
/*
 * Read binary data from position a to b
 */

/*
 * brief Get the size of a file in bytes.
 * param filename The name of the file to check.
 * return The size of the file as a `std::streampos`.
 */
inline std::streampos get_file_size(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Error: Could not open file '" << filename
                  << "' to get size.\n";
        return -1;
    }

    std::streampos size = file.tellg();
    if (size == static_cast<std::streampos>(-1)) {
        std::cerr << "Error: tellg() failed on file '" << filename << "'.\n";
        return -1;
    }

    return size;
}

inline std::vector<bool> read_bits_in_range(const std::string &filename,
                                            std::streampos a,
                                            std::streampos b) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Could not open file: " << filename << "\n";
        return {};
    }

    std::streampos filesize = get_file_size(filename);
    if (b > filesize) {
        b = filesize; // Clamp range to EOF
    }

    std::streamsize size = b - a;
    if (size <= 0) {
        std::cerr << "Invalid byte range.\n";
        return {};
    }

    file.seekg(a);
    std::vector<char> buffer(size);
    file.read(buffer.data(), size);

    std::vector<bool> bits;
    bits.reserve(size * 8); // Pre-allocate for speed

    for (char byte : buffer) {
        for (int i = 7; i >= 0; --i) { // MSB to LSB
            bits.push_back((byte >> i) & 1);
        }
    }

    return bits;
}

inline void write_bits_in_end(const std::string &filename,
                              const std::vector<bool> &bits) {
    std::ofstream file(filename,
                       std::ios::binary | std::ios::app); // open in append mode
    if (!file) {
        std::cerr << "Could not open file for appending: " << filename << "\n";
        return;
    }

    std::vector<char> buffer;
    char byte = 0;
    int bit_count = 0;

    for (bool bit : bits) {
        byte = (byte << 1) | (bit ? 1 : 0);
        bit_count++;

        if (bit_count == 8) {
            buffer.push_back(byte);
            byte = 0;
            bit_count = 0;
        }
    }

    // If there are leftover bits (not a full byte), pad with zeros at the end
    if (bit_count > 0) {
        byte <<= (8 - bit_count);
        buffer.push_back(byte);
    }

    file.write(buffer.data(), buffer.size());
}
/*
 * Check if a file stream opened successfully
 */
inline bool check_file_open(const std::ios &file, const std::string &filename,
                            const std::string &mode) {
    if (!file) {
        std::cerr << "Error: Could not open file '" << filename << "' for "
                  << mode << ".\n";
        return false;
    }
    return true;
}
inline std::string get_file_no(const int &i) {
    std::string filename = "part_no_" + std::to_string(i);
    return filename; // position at end
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

inline std::array<uint8_t, 7> generate_file_id() {
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned>(time(0)));
        seeded = true;
    }

    const char charset[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    std::array<uint8_t, 7> file_id; // no need for {}
    for (int i = 0; i < 7; ++i) {
        file_id[i] =
            static_cast<uint8_t>(charset[rand() % (sizeof(charset) - 1)]);
    }

    return file_id;
}

// Create an empty file with the given number in its name
inline std::string create_empty_file(const int &number) {
    std::string filename = "part_no_" + std::to_string(number);
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to create file: " << filename << "\n";
        return "";
    }
    return filename;
}

//============================================================================
// grave yard of functions
//============================================================================
// /*
//  * Read a single line from a file
//  */
// inline std::string read_line(const std::string &name) {
//     std::ifstream var(name);
//     std::string output;
//     std::getline(var, output);
//     return output;
// }
//
// /*
//  * print a single line from a file
//  */
// inline void print_line(const std::string &name) {
//     std::string line = read_line(name);
//     std::cout << line << std::endl;
// }
//
/*
 * brief Get the size of a file in bytes.
 * param filename The name of the file to check.
 * return The size of the file as a `std::streampos`.
 */
/*
// inline std::streampos get_file_size(const std::string &filename) {
//     std::ifstream file(filename, std::ios::binary | std::ios::ate);
//     if (!file) {
//         std::cerr << "Error: Could not open file '" << filename
//                   << "' to get size.\n";
//         return -1;
//     }
//
//     std::streampos size = file.tellg();
//     if (size == static_cast<std::streampos>(-1)) {
//         std::cerr << "Error: tellg() failed on file '" << filename << "'.\n";
//         return -1;
//     }
//
//     return size;
// }

 *
 *
 *
 *
inline std::vector<bool> readBitsFromRange(const std::string &filename,
                                           std::streampos a, std::streampos b) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Could not open file: " << filename << "\n";
        return {};
    }

    std::streamsize size = b - a;
    if (size <= 0) {
        std::cerr << "Invalid byte range.\n";
        return {};
    }

    file.seekg(a);
    std::vector<char> buffer(size);
    file.read(buffer.data(), size);
    std::vector<bool> bits;

    for (char byte : buffer) {
        for (int i = 7; i >= 0; --i) { // MSB to LSB
            bits.push_back((byte >> i) & 1);
        }
    }

    return bits;
}
 *
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

     // Convert the integer to binary and store the last 64 bits (8 bytes) in
     the
     // array
     for (int i = 0; i < 8; ++i) {
         result[7 - i] = (the_integer >> (i * 8)) & 0xFF; // Extract each byte
     }

     return result;
 }

 inline std::array<uint8_t, 5> int_to_5bytes(int the_integer) {
     std::array<uint8_t, 5> result = {}; // Initialize the array with 0's

     // Convert the integer to binary and store the last 40 bits (5 bytes) in
     the
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

     // Convert the integer to binary and store the last 24 bits (3 bytes) in
     the
     // array
     for (int i = 0; i < 3; ++i) {
         result[2 - i] = (the_integer >> (i * 8)) & 0xFF; // Extract each byte
     }

     return result;
 }

 inline std::array<uint8_t, 2> int_to_2bytes(int the_integer) {
     std::array<uint8_t, 2> result = {}; // Initialize the array with 0's

     // Convert the integer to binary and store the last 16 bits (2 bytes) in
     the
     // array
     for (int i = 0; i < 2; ++i) {
         result[1 - i] = (the_integer >> (i * 8)) & 0xFF; // Extract each byte
     }

     return result;
 }

 inline std::array<uint8_t, 1> int_to_1byte(int the_integer) {
     std::array<uint8_t, 1> result = {}; // Initialize the array with 0's

     // Convert the integer to binary and store the last 8 bits (1 byte) in
     the
     // array
     result[0] = static_cast<uint8_t>(the_integer & 0xFF); // Extract the byte

     return result;
 }

 inline uint64_t bytes_5_to_int(const uint8_t *bytes) {
     return (static_cast<uint64_t>(bytes[0]) << 32) |
            (static_cast<uint64_t>(bytes[1]) << 24) |
            (static_cast<uint64_t>(bytes[2]) << 16) |
            (static_cast<uint64_t>(bytes[3]) << 8) |
            (static_cast<uint64_t>(bytes[4]));
 }
 inline uint16_t bytes_2_to_int(const uint8_t *bytes) {
     return (static_cast<uint16_t>(bytes[0]) << 8) |
            (static_cast<uint16_t>(bytes[1]));
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

 inline void printBinaryRangeAsText(const std::string &filename,
                                    std::streampos start, std::streampos end)
                                    {
     auto buffer = readBinaryRange(filename, start, end);

     if (buffer.empty()) {
         std::cerr << "Nothing to print.\n";
         return;
     }

     std::cout << "🧾 Printing binary data from bytes " << start << " to " <<
     end
               << ":\n\n";

     for (size_t i = 0; i < buffer.size(); ++i) {
         char c = buffer[i];
         uint8_t byte = static_cast<uint8_t>(c);

         // Show printable char or dot
         char displayChar = (std::isprint(c) ? c : '.');

         // Convert current byte index from streampos to integer safely
         std::streamoff current_pos =
             static_cast<std::streamoff>(start) +
             static_cast<std::streamoff>(i);

         std::cout << "Byte " << current_pos << ": '" << displayChar
                   << "'  |  Binary: ";
         for (int b = 7; b >= 0; --b)
             std::cout << ((byte >> b) & 1);
         std::cout << "\n";
     }

     std::cout << "\n✅ Done printing.\n";
 }
============================================================================
 PRINTER
============================================================================
 template <size_t N> uint64_t bytes_to_int(const std::array<uint8_t, N> &arr)
 {
     uint64_t value = 0;
     for (size_t i = 0; i < N; ++i) {
         value = (value << 8) | arr[i];
     }
     return value;
 }

 template <size_t N> uint64_t bytes_to_long(const std::array<uint8_t, N> &arr)
 {
     uint64_t value = 0;
     for (size_t i = 0; i < N; ++i) {
         value = (value << 8) | arr[i];
     }
     return value;
 }
====================================
*/
} // namespace utils
