#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <random>
#include <algorithm>
#include <boost/align/aligned_allocator.hpp>
#include <omp.h>

template <typename T>
using aligned_alloc_ = boost::alignment::aligned_allocator<T, 64>;

template <typename T>
using aligned_vector = std::vector<T, aligned_alloc_<T>>;

//verifies if the given numbers have no mutual divisor
bool IsCoPrime (uint64_t aX, uint64_t aY);

//converts a ASCII string into its bit representation, most significant bit first, blocks of 8 ints per character
aligned_vector<int> StringToBits(std::string aString);

//converts a sequence of ints, representing ASCII characters, most significant bit first, into a string
std::string BitsToString(aligned_vector<int> aBits);

//computes des dot product of the given vectors, expecting vector aBits is a represantion of bits
uint64_t GetDotProduct_64(std::vector<int>& aBits, std::vector<uint64_t>& aKey);

//creates a vector of th given size, containing distinct random numbers
std::vector<uint64_t> GetVector_64(int aSize);

//encrypts the given message with the given key and returns the calculated dot products (the encrypted message)
std::vector<uint64_t> Encrypt_64(std::vector<int>& aMessage, std::vector<uint64_t>& aKey);

//finds the vector (decrypted text) to resolve the given key to the given dot product (encrypted text)
//parallel, vectorized
void ResolveDP_64_P(uint64_t const* aKeyVector, uint64_t const aKeyLength, uint64_t const aDP, int * aPlainVector, int aPlainVectorPosition);
//sequential, vectorized
void ResolveDP_64_S(uint64_t const* aKeyVector, uint64_t const aKeyLength, uint64_t const aDP, int * aPlainVector, int aPlainVectorPosition);
