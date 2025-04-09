#include "splitter.h"
#include "header.h"
#include <charconv>
#include <cstdio>
#include <iostream>

void fsplit() {
    std::string filename;
    int no_splits;
    std::cout << "Enter the file name > ";
    std::cin >> filename;
    std::cout << '\n';

    // open the file
    std::cout << "How many splits you want ? ";
    std::cin >> no_splits;

    // get the file size
    int size = split::getFileSize("filename");

    // calculate the size of payload
    int packet_size = size / no_splits;

    // make packets one by one
    // for (int i = 0; i < no_splits; i++) {
    //     // call the make empyt file function
    //     std::string split_name = split::createEmptyFile(i);
    //
    //     // make header
    //     File_Header header = FFile_Header();
    //
    //     // add the header
    //     write_header_to_file(split_name, header);
    //
    //     // calculate the starting and end pointer for payload
    //
    //     // add the payload to the file
    // }
    // TODO :
    // ask for the output directory
}
