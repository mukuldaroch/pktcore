#include "combiner.h"
// #include "header.h"
#include "header.h"
#include "splitter.h"
#include <iostream>
#include <vector>
using namespace std;
int main() {
    split::createEmptyFile(1);
    // header(part, total, payload, packet = 0, version = 1, flag = 0)
    // header(1, 2, 3, 4, 5, 6);
   Write_Mini_Header("part_no_1", Mini_Header(1, 2, 3));
   Read_And_Print_Mini_Header("part_no_1");
    return 0;
}
