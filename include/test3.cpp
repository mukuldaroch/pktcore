#include "pkt_utils.h"
#include <array>
#include <cstdint>
#include <iostream>

// Function to print the binary representation of an array of uint8_t
void print_binary_array(const std::array<uint8_t, 4> &binaryArray) {
    for (size_t i = 0; i < binaryArray.size(); ++i) {
        // Print each byte's binary representation
        for (int j = 7; j >= 0; --j) {
            std::cout << ((binaryArray[i] >> j) &
                          1); // Print each bit in the byte
        }
    }
    std::cout << std::endl;
}

int main() {
    int the_integer = 3;

    std::array<uint8_t, 4> binaryArray = utils::int_to_4bytes(the_integer);

    // Print the result array for verification
    print_binary_array(binaryArray);

    return 0;
}
