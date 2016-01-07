#include "knapsackLib.hpp"
#include "test.hpp"

#include <chrono>

void test();

int main(int argc, char *argv[] ) {
	//run test cases
	test();

	//read public key from file	
	aligned_vector<uint64_t> keyVector;
	std::string line;
	std::ifstream keyFile ("/teams/team1/key.txt");
	if (keyFile.is_open()) {
		uint64_t key;
		while(keyFile >> key) {
			keyVector.push_back(key);
		}
    }
    keyFile.close();
    
	//read encrypted message from file
    aligned_vector<uint64_t> cryptVector;
	std::ifstream cryptFile ("/teams/team1/crypt.txt");
	if (cryptFile.is_open()) {
		uint64_t crypt;
		while(cryptFile >> crypt) {
			cryptVector.push_back(crypt);
		}
    }
    cryptFile.close();
    
   	//create vector to hold decryption result
   	int keyLength = (int) keyVector.size();
	int cryptLength = (int) cryptVector.size();   	
   	aligned_vector<int> plainVector(keyLength*cryptLength);

	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	
	//the work of this loop could be shared over several machines (MPI)
    for(unsigned int i=0; i<cryptVector.size(); ++i) {    
    	ResolveDP_64_P(keyVector.data(), keyLength, cryptVector[i], plainVector.data(), i*keyLength);
    }
    
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
	std::cout << "elapsed seconds: " << elapsed_seconds << std::endl << std::endl;
    
	std::string plainString = BitsToString(plainVector);
	std::cout << plainString << std::endl;
}

void test() {
	std::cout << (TEST_IsCoPrime() ? "OK" : "ERROR") << "\tTEST_IsCoPrime() " << std::endl;
	std::cout << (TEST_StringToBits() ? "OK" : "ERROR") << "\tTEST_StringToBits() " << std::endl;
	std::cout << (TEST_BitsToString() ? "OK" : "ERROR") << "\tTEST_BitsToString() " << std::endl;
	std::cout << (TEST_GetDotProduct_64() ? "OK" : "ERROR") << "\tTEST_GetDotProduct_64() " << std::endl;
	std::cout << (TEST_GetVector_64() ? "OK" : "ERROR") << "\tTEST_GetVector_64() " << std::endl;
	std::cout << (TEST_Encrypt_64() ? "OK" : "ERROR") << "\tTEST_Encrypt_64() " << std::endl;
	std::cout << std::endl;
}
