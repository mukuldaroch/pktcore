#pragma once
#include "full_header.h"
#include "mini_header.h"
#include "pkt_utils.h"
#include <array>
#include <cstdint>
#include <cstring>
#include <iosfwd>
namespace splitter {
/*
 *input the filename
 */
std::string input_file();

/*
 * input no_of_splits
 */
inline int input_splits();

/*
 *
 */
inline void create_full_header();

/*
 *
 */
inline void create_packet();
/*
 *
 */
inline void SPLITTER();
//--------------------------------------------------------------
inline std::string input_file() {
    std::cout << "Enter the file name " << '\n';
    std::string filename;
    std::cin >> filename;
    return filename;
}
//--------------------------------------------------------------

//--------------------------------------------------------------
inline int input_splits() {
    std::cout << "Enter no of splits" << '\n';
    int no_of_splits;
    std::cin >> no_of_splits;
    return no_of_splits;
}
//--------------------------------------------------------------

//--------------------------------------------------------------
// NOTE flag value is currently at default 0
//--------------------------------------------------------------
inline void full_header(std::array<uint8_t, 5> file_id, int splits,
                        std::string file_name, std::streampos payload_len,
                        std::streampos file_size) {
    utils::Create_Empty_File(0);
    header::Full_Header file_header = header::FULL_HEADER(
        file_id, 0, splits, 0, payload_len, file_size, file_name);
    header::WRITE_FULL_HEADER(utils::Get_File_Name(0), file_header);
}
//--------------------------------------------------------------

//--------------------------------------------------------------
inline header::Mini_Header mini_header(std::array<uint8_t, 5> file_id,
                                       int split_number, int pkt_payload) {
    return header::MINI_HEADER(file_id, split_number, pkt_payload);
}
//--------------------------------------------------------------
//
//--------------------------------------------------------------
inline void create_packet(std::string file, std::array<uint8_t, 5> file_id,
                          int split_number, std::streampos starting_ptr,
                          std::streampos end_ptr) {
    // creates a empty file
    utils::Create_Empty_File(split_number);
    std::streampos payload_len = end_ptr - starting_ptr;
    // write the header in that file
    header::WRITE_MINI_HEADER(utils::Get_File_Name(split_number),
                              mini_header(file_id, split_number, payload_len));
    // fetch the data from the file in a range from starting_ptr to end_ptr
    auto data = utils::Fetch_Bytes(file, starting_ptr, end_ptr);
    // write the data from the file in the end of the packet file
    utils::Append_Bytes(utils::Get_File_Name(split_number), data);
    header::Read_And_Print_Mini_Header(utils::Get_File_Name(split_number));
}

//==================================================================================
inline void SPLITTER() {
    // takes inputs
    std::string file = input_file();
    int splits = input_splits();

    // generate_file_id one time per file
    auto file_id = utils::Genrate_File_ID();

    // returns the size of the file in bits
    auto size = utils::Get_File_Size(file);
    std::cout << size << std::endl;
    auto payload_len = size / splits;
    auto leftover_bytes = size % splits;

    // fullheader genearation
    full_header(file_id, splits, file, payload_len, size);

    // some variables
    int pkt_payload;
    std::streampos starting_ptr = 0, end_ptr;

    // loop for n splits
    for (int i = 1; i < splits + 1; i++) {
        if (leftover_bytes > 0) {
            end_ptr += payload_len;
            end_ptr += 1;
            leftover_bytes--;
        } else {
            end_ptr += payload_len;
        }
        // inline void create_packet(std::array<uint8_t, 7> file_id, int
        // split_number, int pkt_payload, int starting_ptr, int end_ptr) {
        create_packet(file, file_id, i, starting_ptr, end_ptr);
        starting_ptr = end_ptr;
    }
}
} // namespace splitter
