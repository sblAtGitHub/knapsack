#include <stdint.h>
#include <vector>
#include <iostream>

//verifies if the given numbers have no mutual divisor
bool IsCoPrime (uint64_t aX, uint64_t aY);

//converts a string into a bit representation (ASCII, 8 bit per character)
std::vector<int> StringToBits(std::string aString);
