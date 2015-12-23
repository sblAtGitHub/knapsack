#include <stdint.h>
#include <vector>
#include <iostream>

//verifies if the given numbers have no mutual divisor
bool IsCoPrime (uint64_t aX, uint64_t aY);

//converts a string into its bit representation (ASCII, 8 bits per character)
std::vector<uint64_t> StringToBits(std::string aString);
