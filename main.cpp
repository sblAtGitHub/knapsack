#include "knapsackLib.hpp"

int main(int argc, char *argv[] ) {
	
	//test IsCoPrinme
	uint64_t x = 2731;
	uint64_t y = 764;
	bool isCoPrime = IsCoPrime(x, y);
	std::cout << "IsCoPrime(" << x << ", " << y << "): " << (isCoPrime ? "true" : "false") << std::endl;
	
	//test StringToBits
	std::string s = "test";
	std::vector<int> bits = StringToBits(s);
	std::cout << "StringToBits(\"" << s << "\"): ";
	for(unsigned int i=0; i<bits.size(); ++i) {
		if(i>0 && !(i%8)) std::cout << '.';
		std::cout << bits[i];
	}
	std::cout << std::endl;
}
