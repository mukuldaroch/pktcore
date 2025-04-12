#pragma once
#include "full_header.h"
#include "mini_header.h"
#include "pkt_utils.h"
#include <cstdint>
#include <iostream>
#include <string>

//==============================================================================
// AVAILABLE FUNCTIONS:
//==============================================================================
namespace split {
//==============================================================================
inline void no_of_split() {
    std::cout << "Enter the file name " << '\n';
    std::string filename;
    std::cin >> filename;

    // take no of splits
    std::cout << "Enter no of splits" << '\n';
    int no_of_splits;
    std::cin >> no_of_splits;

    // get the size
    std::uint64_t filesize = utils::get_file_size(filename);

    // calculate no of packets
    std::uint32_t payload_size = filesize / no_of_splits;

    // create a empty file
    utils::create_empty_file(0);
    // write full header in full header packet
    std::array file_id = utils::generate_file_id();
    int flag = 0;
    Full_Header full_header = header(file_id, 0, no_of_splits, flag,
                                     payload_size, filesize, filename);

    Write_Full_Header(utils::get_file_no(0), full_header);
    Read_And_Print_Full_Header("part_no_0");
    int a = 0, b = payload_size;
    // for loop {
    for (int i = 1; i < no_of_splits + 1; i++) {
        utils::create_empty_file(i);
        Write_Mini_Header(
            utils::get_file_no(i),
            Mini_Header(file_id, i,
                        utils::get_file_size(utils::get_file_no(i))));
        utils::write_bits_in_end(
            utils::get_file_no(i),
            utils::read_bits_in_range(utils::get_file_no(i), a, b));
        a = a + payload_size;
        b = b + payload_size;
        Read_And_Print_Mini_Header(utils::get_file_no(i));
    }
    //     create a empyt packet
    //     create a mini header and write it into empy packet file
    //     read the data and store it in a tem variable
    //     opem the mini header packet
    //     move offset to 15 bytes
    //     write the data after the Mini header which is 15 bytes
    // }
}
//
//==============================================================================
} // namespace split
