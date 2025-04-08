#include "splitter.h"
#include "header.h"
#include <iostream>

void split() {
  std::string filename;
  int no_splits;
  std::cout << "Enter the file name > ";
  std::cin >> filename;
  std::cout << '\n';

  // open the file
  std::cout << "How many splits you want ? ";
  std::cin >> no_splits;

  // get the file size
  int size = fsplit::getFileSize("filename");

  // calculate the size of payload
  int packet_size = size / no_splits;

  // make packets one by one
  for (int i = 0; i < no_splits; i++) {
    // call the make empyt file function
    fsplit::createEmptyFile(i);

    // make header
    // make_header(int partNumber, int totalParts, int chunkSize);

    // add the header

    // calculate the starting and end pointer for payload

    // add the payload to the file
  }
  // TODO :
  // ask for the output directory
}
