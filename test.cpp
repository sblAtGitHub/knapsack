#include "test.hpp"

void test() {
	std::cout << (TEST_StringToBits() ? "OK" : "ERROR") << "\tTEST_StringToBits() " << std::endl;
	std::cout << (TEST_BitsToString() ? "OK" : "ERROR") << "\tTEST_BitsToString() " << std::endl;
	std::cout << std::endl;
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













