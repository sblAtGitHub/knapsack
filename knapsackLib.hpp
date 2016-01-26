#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <random>
#include <algorithm>
#include <boost/align/aligned_allocator.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/random.hpp>
#include <omp.h>

template <typename T>
using aligned_alloc_ = boost::alignment::aligned_allocator<T, 64>;

template <typename T>
using aligned_vector = std::vector<T, aligned_alloc_<T>>;

//--------------------------------------//
// helpermethods for string handling	//
//--------------------------------------//

//converts a ASCII string into its bit representation, most significant bit first, blocks of 8 ints per character
aligned_vector<int> StringToBits(std::string aString);

//converts a sequence of ints, representing ASCII characters, most significant bit first, into a string
std::string BitsToString(aligned_vector<int> aBits);

//------------------------------------------------------------------//
// merkle-hellman with 64-bit integers								//
// -there is no private key generation due to the limit of 64-bit	//
//------------------------------------------------------------------//

void CreateKey_64(int aSize, std::string aPublicKeyFile);

void Encrypt_64(std::string aMessage, std::string aPublicKeyFile, std::string aCryptFile);

void Crack_64(std::string aPublicKeyFile, std::string aCryptFile);

//finds the vector (decrypted text) to resolve the given key to the given dot product (encrypted text)
//parallel
void ResolveDP_64_P(uint64_t const* aKeyVector, uint64_t const aKeyLength, uint64_t const aDP, int * aPlainVector, int const aPlainVectorPosition);
//parallel, vectorized
void ResolveDP_64_PV(uint64_t const* aKeyVector, uint64_t const aKeyLength, uint64_t const aDP, int * aPlainVector, int const aPlainVectorPosition);
//sequential
void ResolveDP_64_S(uint64_t const* aKeyVector, uint64_t const aKeyLength, uint64_t const aDP, int * aPlainVector, int const aPlainVectorPosition);

//------------------------------------------------------------------//
// complete implementation of merkle-hellman with 512-bit integers	//
// -uses Boost Multiprcision library for big integers
//------------------------------------------------------------------//

namespace bm = boost::multiprecision;
namespace br = boost::random;

void CreateKey_BN(int aSize, std::string aPrivateKeyFile, std::string aPublicKeyFile);

void Encrypt_BN(std::string aMessage, std::string aPublicKeyFile, std::string aCryptFile);

void Decrypt_BN(std::string aPrivateKeyFile, std::string aCryptFile);

//calculates the multiplicative inverse module, needed for decryption
bm::int512_t MultInverse_BN(bm::int512_t aW, bm::int512_t aM);

void Crack_BN(std::string aPublicKeyFile, std::string aCryptFile);

//finds the vector (decrypted text) to resolve the given key to the given dot product (encrypted text)
//parallel
void ResolveDP_BN_P(bm::int512_t const* aKeyVector, int const aKeyLength, bm::int512_t const aDP, int * aPlainVector, int const aPlainVectorPosition);

