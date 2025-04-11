#include "pkt_utils.h"
#include "splitter.h"
using namespace std;
int main() {
    // utils::create_empty_file(0);
    // utils::create_empty_file(1);

    // Write_Full_Header("part_no_0",
    //                   Full_Header(1, 2, 3, 4, 5, 6, 7, "test"));
    // Write_Mini_Header("part_no_1",
    //                   Mini_Header(utils::generate_file_id(), 7, 2));
    // cout << utils::getFileSize("part_no_1") << '\n';
    // Read_And_Print_Mini_Header("part_no_1");
    // cout << utils::getFileSize("part_no_0") << '\n';
    // Read_And_Print_Full_Header("part_no_0");

    split::printBinaryRangeAsText("filetest", 40, 80);
    return 0;
}
