#include <vector>
#include <iostream>
#include <random>
#include <algorithm>

//verifies if the given numbers have no mutual divisor
bool IsCoPrime (uint64_t aX, uint64_t aY);

//converts a ASCII string into its bit representation, most significant bit first, blocks of 8 ints per character
std::vector<int> StringToBits(std::string aString);

//converts a sequence of ints, representing ASCII characters, most significant bit first, into a string
std::string BitsToString(std::vector<int> aBits);

//incremets the given vector by one; aVector is a representation of bits, most significant bit first
void IncrementVector(std::vector<int>& aVector);

//computes des dot product of the given vectors, expecting vector aBits is a represantion of bits
uint64_t GetDotProduct_64(std::vector<int>& aBits, std::vector<uint64_t>& aKey);

//creates a vector of th given size, containing distinct random numbers
std::vector<uint64_t> GetVector_64(int aSize);

//encrypts the given message with the given key and returns the calculated dot products (the encrypted message)
std::vector<uint64_t> Encrypt_64(std::vector<int>& aMessage, std::vector<uint64_t>& aKey);

//breaks the given cryptotext in combination with the given (public) key just by trying all possible combinations
std::vector<int> Break_64(std::vector<uint64_t>& aCrypt, std::vector<uint64_t>& aKey);
