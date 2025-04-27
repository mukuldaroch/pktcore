#pragma once
#include "explorer.h"
#include "full_header.h"
#include "mini_header.h"
#include "pkt_utils.h"
#include <array>
#include <cstdint>
#include <cstring>
#include <iosfwd>
#include <vector>

/*
 * Splitter module namespace: Contains all logic to split a file into multiple
 * packets with headers and payload.
 */
namespace splitter {

/*
 * Prompt user to input a file name.
 * return: filename as a std::string
 */
inline std::string input_file();

/*
 * Prompt user to input number of splits.
 * return: number of splits as an int
 */
inline int input_splits();

/*
 * Create and write the full header into the 0th split file.
 * param file_id: unique ID for the file (5 bytes)
 * param splits: total number of splits/chunks
 * param file_name: name of the original file
 * param payload_len: size of each chunk (excluding header)
 * param file_size: total original file size
 */
inline void full_header(std::array<uint8_t, 5> file_id, int splits,
                        std::string file_name, std::streampos payload_len,
                        std::streampos file_size);

/*
 * Create an individual packet file with a mini header and corresponding data.
 * param file: original filename to read from
 * param file_id: 5-byte file identifier
 * param splits: current split number
 * param starting_ptr: start byte offset for this chunk
 * param end_ptr: end byte offset for this chunk
 */
inline void create_packet(std::string file, std::array<uint8_t, 5> file_id,
                          int splits, std::streampos starting_ptr,
                          std::streampos end_ptr);

/*
 * Main driver function to perform the file splitting operation.
 * It selects a file, asks for number of splits, generates headers, and creates
 * packet files.
 */
inline void SPLITTER();

/*
 * Main driver function to perform the file splitting operation.
 * it takes file name inside parameters
 * packet files.
 */
inline void SPLITTER(const std::string &file);

/*
 * Main driver function to perform the file splitting operation.
 * it takes file name and no of splits inside parameters
 * packet files.
 */
inline void SPLITTER(const std::string &file, int splits);
//=================================================================================
//=================================================================================
// function coding here
// these files are here for easy debugging
// later when the projects gets bigger and this part is bug free
// we will move this section to different file
//
std::string input_file() {
    std::cout << "Enter the file name " << '\n';
    std::string filename;
    std::cin >> filename;
    return filename;
}

int input_splits() {
    std::cout << "Enter no of splits" << '\n';
    int no_of_splits;
    std::cin >> no_of_splits;
    return no_of_splits;
}

void full_header(std::array<uint8_t, 5> file_id, int splits,
                 std::string file_name, std::streampos payload_len,
                 std::streampos file_size) {
    std::string fname = utils::CREATE_EMPTY_HEADER_FILE(file_id, 0);
    header::Full_Header file_header = header::FULL_HEADER(
        file_id, 0, splits, 0, payload_len, file_size, file_name);
    header::WRITE_FULL_HEADER(fname, file_header);
    header::Print_Full_Header(fname);
}

void create_packet(std::string file, std::array<uint8_t, 5> file_id, int splits,
                   std::streampos starting_ptr, std::streampos end_ptr) {
    std::string fname = utils::CREATE_EMPTY_HEADER_FILE(file_id, splits);
    std::streampos payload_len = end_ptr - starting_ptr;

    // Write mini header
    header::WRITE_MINI_HEADER(
        fname, header::Mini_Header(file_id, splits, payload_len));

    // Read payload data from file
    auto data = utils::Fetch_Bytes(file, starting_ptr, end_ptr);

    // Append payload data to new file
    utils::Append_Bytes(fname, data);
}

void SPLITTER() {
    // File selection
    std::vector<std::string> files = utils::FETCH_FILES(".");
    std::string file = tui::SHOW_SELECT_FILES(files);
    int splits = input_splits();

    // Generate unique file ID
    auto file_id = utils::Genrate_File_ID();

    // Get file size
    auto size = utils::Get_File_Size(file);
    std::cout << size << std::endl;

    // Calculate split size and leftover bytes
    auto payload_len = size / splits;
    auto leftover_bytes = size % splits;

    // Create full header (split 0)
    full_header(file_id, splits, file, payload_len, size);

    // Splitting logic
    std::streampos starting_ptr = 0, end_ptr = 0;

    for (int i = 1; i < splits + 1; i++) {
        if (leftover_bytes > 0) {
            end_ptr += payload_len + 1;
            leftover_bytes--;
        } else {
            end_ptr += payload_len;
        }

        create_packet(file, file_id, i, starting_ptr, end_ptr);
        starting_ptr = end_ptr;
    }
}

void SPLITTER(const std::string &file) {
    // File selection
    std::vector<std::string> files = utils::FETCH_FILES(".");
    // Since the file is already passed in as a parameter, we skip file
    // selection
    int splits = input_splits();

    // Generate unique file ID
    auto file_id = utils::Genrate_File_ID();

    // Get file size
    auto size = utils::Get_File_Size(file);
    std::cout << "File size: " << size << std::endl;

    // Calculate split size and leftover bytes
    auto payload_len = size / splits;
    auto leftover_bytes = size % splits;

    // Create full header (split 0)
    full_header(file_id, splits, file, payload_len, size);

    // Splitting logic
    std::streampos starting_ptr = 0, end_ptr = 0;

    for (int i = 1; i <= splits; i++) {
        if (leftover_bytes > 0) {
            end_ptr += payload_len + 1;
            leftover_bytes--;
        } else {
            end_ptr += payload_len;
        }

        create_packet(file, file_id, i, starting_ptr, end_ptr);
        starting_ptr = end_ptr;
    }
}

void SPLITTER(const std::string &file, int no_of_splits) {
    // File selection
    std::vector<std::string> files = utils::FETCH_FILES(".");
    // Since the file is already passed in as a parameter, we skip file
    // selection
    int splits = no_of_splits;

    // Generate unique file ID
    auto file_id = utils::Genrate_File_ID();

    // Get file size
    auto size = utils::Get_File_Size(file);
    std::cout << "File size: " << size << std::endl;

    // Calculate split size and leftover bytes
    auto payload_len = size / splits;
    auto leftover_bytes = size % splits;

    // Create full header (split 0)
    full_header(file_id, splits, file, payload_len, size);

    // Splitting logic
    std::streampos starting_ptr = 0, end_ptr = 0;

    for (int i = 1; i <= splits; i++) {
        if (leftover_bytes > 0) {
            end_ptr += payload_len + 1;
            leftover_bytes--;
        } else {
            end_ptr += payload_len;
        }

        create_packet(file, file_id, i, starting_ptr, end_ptr);
        starting_ptr = end_ptr;
    }
}
} // namespace splitter
