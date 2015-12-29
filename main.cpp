#include "knapsackLib.hpp"
#include "test.hpp"

int main(int argc, char *argv[] ) {
	
	std::cout << (TEST_IsCoPrime() ? "OK" : "ERROR") << "\tTEST_IsCoPrime() " << std::endl;
	std::cout << (TEST_StringToBits() ? "OK" : "ERROR") << "\tTEST_StringToBits() " << std::endl;
	std::cout << (TEST_BitsToString() ? "OK" : "ERROR") << "\tTEST_BitsToString() " << std::endl;
	std::cout << (TEST_IncrementVector() ? "OK" : "ERROR") << "\tTEST_IncrementVector() " << std::endl;
	std::cout << (TEST_GetDotProduct_64() ? "OK" : "ERROR") << "\tTEST_GetDotProduct_64() " << std::endl;
	std::cout << (TEST_GetVector_64() ? "OK" : "ERROR") << "\tTEST_GetVector_64() " << std::endl;
	std::cout << (TEST_Encrypt_64() ? "OK" : "ERROR") << "\tTEST_Encrypt_64() " << std::endl;
	std::cout << (TEST_Break_64() ? "OK" : "ERROR") << "\tTEST_Break_64() " << std::endl;
	std::cout << std::endl;
}
