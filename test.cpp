#include "test.hpp"

bool TEST_IsCoPrime() {
	uint64_t x = 2731;
	uint64_t y = 764;
	if(!IsCoPrime(x, y))
		return false;
	x = 2732;
	if(IsCoPrime(x, y))
		return false;	
	return true;
}

bool TEST_StringToBits() {
	aligned_vector<int> test {0,1,0,1,1,0,0,0};
	if(test == StringToBits("X"))
		return true;
	return false;
}

bool TEST_BitsToString() {
	aligned_vector<int> test {0,1,0,1,1,0,0,0};
	if("X" == BitsToString(test))
		return true;
	return false;
}

bool TEST_GetDotProduct_64() {
	std::vector<int> v1 {0,1,0,1};
	std::vector<uint64_t> v2 {1,2,3,4};
	if(6 == GetDotProduct_64(v1, v2))
		return true;
	return false;
}

bool TEST_GetVector_64() {
	unsigned int size = 42;
	std::vector<uint64_t> test = GetVector_64(size);
	if(size != test.size())
		return false;
	std::sort(test.begin(), test.end());
	for(unsigned int i=0; i<test.size(); ++i) {
		if(i < (test.size()+1)) {
			if(test[i] == test[i+1])
				return false;
		}
	}
	return true;
}

bool TEST_Encrypt_64() {
	std::vector<int> message {1,0,0,1};
	std::vector<uint64_t> key {1,2};
	std::vector<uint64_t> crypt {1,2};
	if(crypt == Encrypt_64(message, key))
		return true;
	return false;
}













