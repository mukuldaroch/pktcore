#include "combiner.h"
// #include "header.h"
#include "header.h"
#include "pkt_utils.h"
#include "splitter.h"
#include <iostream>
#include <vector>
using namespace std;
int main() {
    split::createEmptyFile(0);
    // header(part, total, payload, packet = 0, version = 1, flag = 0)
    // header(1, 2, 3, 4, 5, 6);
    // Full_Header header(version, partNumber, totalParts, flag, payloadSize,
    // packetSize, filenameSize, filename, fileSize)
    // Write_Full_Header("part_no_0",
    //                   Full_Header(1, 0, 2, 3, 4, 5, 6, "test1", 8));
    Write_Mini_Header("part_no_1",
                      Mini_Header(utils::generate_file_id(), 2, 3));
    // Read_And_Print_Full_Header("part_no_0");
    cout << utils::getFileSize("part_no_1") << '\n';
    Read_And_Print_Mini_Header("part_no_1");

    return 0;
}
