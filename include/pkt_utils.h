#pragma once
#include <array>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <string>
#include <vector>

//==============================================================================
// AVAILABLE FUNCTIONS:
// 1) Get_File_Size
// 2) Fetch_Bytes
// 3) Append_Bytes
// 4) Genrate_File_ID
// 5) Create_Empty_File
//==============================================================================
namespace utils {

/*
 * This function returns all vector of all the files inside a directory
 * - @path : path of the directory
 */
inline std::vector<std::string> FETCH_FILES(const std::filesystem::path &path) {
    std::vector<std::string> files;
    for (const auto &entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path().filename().string());
        }
    }
    return files;
}
/*
 * This function returns the size of a given file in bytes.
 * - @param filename : The name or path of the file to check.
 * - @return         : The size of the file as std::streampos, or -1 if failed.
 */
inline std::streampos Get_File_Size(const std::string &filename) {
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

/*
 * Reads bytes from a file between a given byte range [byte_start, byte_end).
 * - @param filename   : The name of the file to read from.
 * - @param byte_start : The starting byte position.
 * - @param byte_end   : The ending byte position (exclusive).
 * - @return           : A vector of bytes read from the file, empty if failed.
 */
inline std::vector<uint8_t> Fetch_Bytes(const std::string &filename,
                                        std::streampos byte_start = 0,
                                        std::streampos byte_end = -1) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Could not open file: " << filename << "\n";
        return {};
    }

    std::streampos file_size = file.tellg();
    if (byte_end == -1 || byte_end > file_size) {
        byte_end = file_size;
    }

    if (byte_end <= byte_start) {
        std::cerr << "Invalid byte range.\n";
        return {};
    }

    size_t num_bytes = static_cast<size_t>(byte_end - byte_start);
    std::vector<uint8_t> bytes(num_bytes);

    file.clear(); // Clear any EOF or fail flags
    file.seekg(byte_start);
    if (!file) {
        std::cerr << "Seek failed in file: " << filename << "\n";
        return {};
    }

    file.read(reinterpret_cast<char *>(bytes.data()), num_bytes);
    if (static_cast<size_t>(file.gcount()) != num_bytes) {
        std::cerr << "Error reading the expected number of bytes. Got: "
                  << file.gcount() << " expected: " << num_bytes << "\n";
        return {};
    }

    return bytes;
}

/*
 * Appends binary data to the end of a file.
 * - @param filename : The file to append bytes to.
 * - @param bytes    : A vector of bytes to be written at the end of the file.
 */
inline void Append_Bytes(const std::string &filename,
                         const std::vector<uint8_t> &bytes) {
    std::ofstream file(filename,
                       std::ios::binary | std::ios::app); // open in append mode
    if (!file) {
        std::cerr << "Could not open file for appending: " << filename << "\n";
        return;
    }

    // Write the bytes directly to the file
    file.write(reinterpret_cast<const char *>(bytes.data()), bytes.size());
}

/*
 * Generates a random 5-byte alphanumeric file ID.
 * - @return: An array of 5 uint8_t representing the unique file ID.
 * This ID can be used as a unique identifier for a file packets.
 */
inline std::array<uint8_t, 5> Genrate_File_ID() {
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned>(time(0)));
        seeded = true;
    }

    const char charset[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    std::array<uint8_t, 5> file_id;
    for (int i = 0; i < 5; ++i) {
        file_id[i] =
            static_cast<uint8_t>(charset[rand() % (sizeof(charset) - 1)]);
    }

    return file_id;
}

/*
 * Creates an empty file with a filename based on the file ID and part number.
 * - @param f_id: The 5-byte file ID (as array of uint8_t).
 * - @param number: The number to append to the filename (e.g., part number).
 * - @return: The generated filename, or an empty string on failure.
 * The filename is in the format <HEX(file_id)>_<number>.
 */
inline std::string CREATE_EMPTY_HEADER_FILE(const std::array<uint8_t, 5> &f_id,
                                            const int &number) {
    std::string id_str;
    for (uint8_t byte : f_id) {
        // Convert each byte to 2-digit hex string
        char buf[3];
        snprintf(buf, sizeof(buf), "%02X", byte);
        id_str += buf;
    }

    std::string filename = id_str + "_" + std::to_string(number);

    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to create file: " << filename << "\n";
        return "";
    }
    return filename;
}

/*
 * Creates an empty file with a filename
 * - @filename : empty file name to be created
 * - @return   : The generated filename, or an empty string on failure.
 */
inline std::string CREATE_EMPTY_FILE(const std::string &filename) {
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
//
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
// //
-----------------------------------------------------------------------------------------
//  * Check if a file stream opened successfully
//  */
// inline bool check_file_open(const std::ios &file, const std::string
// &filename,
//                             const std::string &mode) {
//     if (!file) {
//         std::cerr << "Error: Could not open file '" << filename << "' for "
//                   << mode << ".\n";
//         return false;
//     }
//     return true;
// }
//  *
//  *
//  *
//  /*
/*
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
// inline std::vector<char> readFileChunk(const std::string &filename,
//                                        std::streampos start,
//                                        std::streampos end) {
//     std::ifstream file(filename, std::ios::binary);
//     std::vector<char> buffer;
//
//     // Ensure start < end
//     if (start >= end) {
//         std::cerr << "Invalid range: start >= end\n";
//         return buffer;
//     }
//
//     std::streamsize chunkSize = end - start;
//     buffer.resize(chunkSize);
//
//     file.seekg(start);
//     if (!file.read(buffer.data(), chunkSize)) {
//         std::cerr << "Failed to read file chunk\n";
//         buffer.clear();
//     }
//
//     return buffer;
// }
//
*/
} // namespace utils
