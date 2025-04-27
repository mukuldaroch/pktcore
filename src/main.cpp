#include "../include/combiner.h"
#include "../include/splitter.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    if (argc > 1) {
        std::string arg1 = argv[1];

        if (arg1 == "help" || arg1 == "--help") {
            std::cout << "--version" << '\n';
            std::cout << "--split" << '\n';
            std::cout << "--combine" << '\n';
            std::cout << "--show" << '\n';
            return 0;

        } else if (arg1 == "--version" || arg1 == "version" || arg1 == "vr") {
            std::cout << "pktcore version 1.0.0\n";
            return 0;

        } else if (arg1 == "split") {
            // Check if there's a second argument (filename)
            if (argc > 2) {
                std::string file =
                    argv[2]; // Get the file name from the second argument
                if (argc > 3) {
                    try {
                        // Try converting the third argument to an integer
                        int x = std::stoi(argv[3]); // Convert the third
                                                    // argument to an integer
                        splitter::SPLITTER(file, x);
                        // Call SPLITTER with file and int x
                    } catch (const std::invalid_argument &e) {
                        // If it's not an integer, show an error
                        std::cerr << "Error: The third argument must be an "
                                     "integer.\n";
                        std::cerr
                            << "Example: ./pcore split <filename> <integer>\n";
                        return 1; // Exit with an error code
                    }
                }
            } else {
                // If no filename is provided, just call SPLITTER() without
                // arguments
                splitter::SPLITTER();
            }
            return 0;

        } else if (arg1 == "combine" || arg1 == "--combine") {

            if (argc > 2) {
                std::string fname = argv[2];

                std::string file = combiner::Detect_PCORE_Files(fname);
                auto var = combiner::BuildMinHeapForFile(file);
                combiner::COMBINE(var);
                return 0;
                // Get the file name from the second argument
            } else {
                std::string file = combiner::Detect_PCORE_Files();
                auto var = combiner::BuildMinHeapForFile(file);
                combiner::COMBINE(var);
                return 0;
            }

        } else if (arg1 == "list" || arg1 == "--list") {
            if (argc > 2) {
                std::string fname = argv[2];

                std::string file = combiner::Detect_PCORE_Files(fname);
                auto var = combiner::BuildMinHeapForFile(file);
                combiner::PrintMinHeap(var);
                return 0;
                // Get the file name from the second argument
            } else {
                std::cerr << "Unknown argument: " << arg1 << "\n";
                std::cerr << "try : --help to list available commands\n";
                return 0;
            }
        } else if (arg1 == "show" || arg1 == "--show") {
            combiner::SHOW_PCORE_FILES();
            return 0;
        } else {

            std::cerr << "Unknown argument: " << arg1 << "\n";
            std::cerr << "try : --help to list available commands\n";
        }
    } else {
        std::cout << "No arguments provided.\n";
        std::cout << "try : --help to list available commands\n";
    }
    return 0;
}
