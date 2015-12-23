#include "knapsackLib.hpp"

bool IsCoPrime (uint64_t aX, uint64_t aY) {
	return (aX<aY) ? IsCoPrime(aY,aX) : !(aX%aY) ? (aY==1) : IsCoPrime (aY, aX%aY);
}

std::vector<int> StringToBits(std::string aString) {
	std::vector<int> result(aString.size() * 8, 0);
	for(unsigned int i=0; i<aString.size(); ++i) {
		for(int j=0; j<8; ++j) {
			result[(int)i*8+j] = (aString[i] >> j) & 1;
		}
	}
	return result;
}
