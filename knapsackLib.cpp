#include <knapsackLib.hpp>

bool IsCoPrime (uint64_t x, uint64_t y) {
	return (x<y) ? IsCoPrime(y,x) : !(x%y) ? (y==1) : IsCoPrime (y, x%y);
}
