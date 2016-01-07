#include "knapsackLib.hpp"

//holds the amount of bits used to represent an ASCII character
const int CHAR_BITS = 8;

bool IsCoPrime (uint64_t aX, uint64_t aY) {
	return (aX<aY) ? IsCoPrime(aY,aX) : !(aX%aY) ? (aY==1) : IsCoPrime (aY, aX%aY);
}

aligned_vector<int> StringToBits(std::string aString) {
	aligned_vector<int> result(aString.size() * CHAR_BITS, 0);
	for(unsigned int i=0; i<aString.size(); ++i) {
		for(int j=0; j<CHAR_BITS; ++j) {
			result[(int)i*CHAR_BITS+j] = (aString[i] >> (CHAR_BITS-1-j)) & 1;
		}
	}
	return result;
}

std::string BitsToString(aligned_vector<int> aBits) {
	int len = aBits.size()/CHAR_BITS;
	std::string result(len, 0);
	for(int i=0; i<len; ++i) {
		for(int j=0; j<CHAR_BITS; ++j) {
			if(aBits[i*CHAR_BITS+j]) {
				result[i] = result[i] | (1<<(CHAR_BITS-1-j));
			}
			else {
				result[i] = result[i] & ~(1<<(CHAR_BITS-1-j));
			}
		}
	}
	return result;
}

uint64_t GetDotProduct_64(std::vector<int>& aBits, std::vector<uint64_t>& aKey) {
	uint64_t result=0;
	for(unsigned int i=0; i<aBits.size(); ++i) {
		result += aBits[i] * aKey[i];
	}
	return result;
}

std::vector<uint64_t> GetVector_64(int aSize) {
	std::random_device rd;
	std::mt19937_64 e2(rd());
	std::uniform_int_distribution<uint64_t> dist(0,(0ULL - 1)/aSize);
	std::vector<uint64_t> result(aSize);
	for(int i=0; i<aSize; ++i) {
		uint64_t r = dist(e2);
		bool insert = true;
		for(int j=0; j<i; ++j) {
			if(result[j]==r) {
				--i;
				insert = false;
				break;
			}
		}		
		if(insert)
			result[i] = r;
	}
	return result;
}

std::vector<uint64_t> Encrypt_64(std::vector<int>& aMessage, std::vector<uint64_t>& aKey) {
	int segSize = aKey.size();
	int segs = ceil((double)aMessage.size()/segSize);
	std::vector<uint64_t> result(segs, 0);
	for(int seg=0; seg<segs; ++seg) {
		int first = seg*segSize;
		int last = first+segSize > (int)aMessage.size() ? aMessage.size() : first+segSize;
		std::vector<int> messageSeg(&aMessage[first], &aMessage[last]);
		result[seg] = GetDotProduct_64(messageSeg, aKey);
	}
	return result;
}

void ResolveDP_64_P(uint64_t const* aKeyVector, uint64_t const aKeyLength, uint64_t const aDP, int * aPlainVector, int const aPlainVectorPosition) {
	uint64_t n_max = pow(2, aKeyLength);
	#pragma omp parallel
	{
		#pragma omp for schedule(static)
		for(uint64_t n=0; n<n_max; ++n) {		
			uint64_t result=0;
			#pragma omp simd aligned(aKeyVector,aPlainVector:64) reduction(+:result)
			for(int i=0; i<aKeyLength; ++i) {
				result += ((n >> i) & 1) * aKeyVector[i];
			}
			if(aDP == result) {
				for(int i=0; i<aKeyLength; ++i) {
					aPlainVector[aPlainVectorPosition + i] = (n >> i) & 1;
				}
				#pragma omp cancel for
			}
		}
	}
}
	
void ResolveDP_64_S(uint64_t const* aKeyVector, uint64_t const aKeyLength, uint64_t const aDP, int * aPlainVector, int const aPlainVectorPosition) {
	for(uint64_t n=0; n<pow(2, aKeyLength); ++n) {		
		uint64_t result=0;
		#pragma omp simd aligned(aKeyVector,aPlainVector:64) reduction(+:result)
		for(int i=0; i<aKeyLength; ++i) {
			aPlainVector[aPlainVectorPosition + i] = (n >> i) & 1;
			result += aPlainVector[aPlainVectorPosition + i] * aKeyVector[i];
		}
		if(aDP == result) {
			return;
		}
	}
}
