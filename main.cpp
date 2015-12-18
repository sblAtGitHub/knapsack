#include "knapsackLib.hpp"
#include <iostream>

int main(int argc, char *argv[] ) {

	
	
	uint64_t x = 2731;
	uint64_t y = 764;
	
	bool isCoPrime = IsCoPrime(x, y);
	std::cout << "Test:" << (isCoPrime ? "true" : "false") << std::endl;
}
