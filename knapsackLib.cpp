#include "knapsackLib.hpp"

//holds the amount of bits used to represent an ASCII character
const int CHAR_BITS = 8;

bool IsCoPrime (uint64_t aX, uint64_t aY) {
	return (aX<aY) ? IsCoPrime(aY,aX) : !(aX%aY) ? (aY==1) : IsCoPrime (aY, aX%aY);
}

std::vector<int> StringToBits(std::string aString) {
	std::vector<int> result(aString.size() * CHAR_BITS, 0);
	for(unsigned int i=0; i<aString.size(); ++i) {
		for(int j=0; j<CHAR_BITS; ++j) {
			result[(int)i*CHAR_BITS+j] = (aString[i] >> (CHAR_BITS-1-j)) & 1;
		}
	}
	return result;
}

std::string BitsToString(std::vector<int> aBits) {
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

void IncrementVector(std::vector<int>& aVector) {
	for(int i=aVector.size()-1; i>=0; --i) {
		if(aVector[i] == 0 ) {
            aVector[i] = 1;
            break;
		}
        aVector[i] = 0;
	}
}

uint64_t GetDotProduct_64(std::vector<int>& aBits, std::vector<uint64_t>& aKey) {
	uint64_t result=0;
	for(unsigned int i=0; i<aBits.size(); ++i) {
		if(aBits[i]) {
			result += aKey[i];
		}
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

std::vector<int> Break_64(std::vector<uint64_t>& aCrypt, std::vector<uint64_t>& aKey) {
	int n = aKey.size();
	std::vector<int> result(aCrypt.size()*aKey.size(), 0);
	
	//just for interest, compute the average tries (out of 100%=2^n) needed, to find the right combination for each segment
	double ratio=0;
	
	for(unsigned int i=0; i<aCrypt.size(); ++i) {
		std::vector<int> test(n, 0);
		for(int t=0; t<pow(2, n); ++t) {
			if(aCrypt[i] == GetDotProduct_64(test, aKey)) {
				for(unsigned int r=0; r<test.size(); ++r) {
					result[(i*aKey.size())+r] = test[r];
				}
				ratio += (double)t*100/pow(2, n);
				break;
			}
			else {
				IncrementVector(test);
			}
		}
	}
	
	ratio = ratio/aCrypt.size();
	//std::cout << "ratio=" << ratio << std::endl;
	return result;
}











