#include "../include/combiner.h"
int main() {
    // splitter::SPLITTER();
    // header::Print_Mini_Header(utils::Get_File_Name(1));

    std::string file = combiner::Detect_PCORE_Files();
    std::cout << file;
    auto var = combiner::BuildMinHeapForFile(file);
    combiner::PrintMinHeap(var);

    return 0;
}
