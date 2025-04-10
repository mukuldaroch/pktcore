#include "splitter.h"
#include "header.h"
#include "pkt_utils.h"
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
    int file_size = utils::getFileSize(filename);

    int packet_size = file_size / no_splits;

    // calculate the size of payload
    //

    // make full header
    for (int i = 0; i < no_splits; i++) {
        // make empty file
        // attack header to the file
    }

    // TODO :
    // ask for the output directory
}
