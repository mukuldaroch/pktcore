#include "full_header.h"
#include "pkt_utils.h"
using namespace std;
int main() {
  std::string filename = "part_no_0";

  int no_of_splits = 888;

  // get the size
  int filesize = utils::getFileSize(filename);

  // calculate no of packets
  int payload_size = filesize / no_of_splits;

  // create a empty file
  utils::create_empty_file(0);
  // write full header in full header packet
  int version = 1, flag = 0;
  Full_Header full_header = header(utils::generate_file_id(),0, no_of_splits,
                                   flag, payload_size, filesize, filename);
  Write_Full_Header("part_no_0", full_header);
  cout << utils::getFileSize("part_no_0") << '\n';
  Read_And_Print_Full_Header("part_no_0");

  // split::printBinaryRangeAsText("filetest", 40, 80);
  return 0;
}
