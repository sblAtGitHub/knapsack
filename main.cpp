#include "knapsackLib.hpp"
#include "test.hpp"

#include <chrono>

int main(int argc, char *argv[] ) {
	
	//run test cases
	test();	
	
	//initialize
	int keyLength = 26;
	std::string message = "One noise that troubled him not at all was the pipe organ in the chapel at Bolger Christian College.";
	std::string path = "/teams/team1/";

	//Testing the 64-bit version
	CreateKey_64(keyLength, path + "publicKey_64.txt");
	Encrypt_64(message, path + "publicKey_64.txt", path + "encrypted_64.txt");
	
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	Crack_64(path + "publicKey_64.txt", path + "encrypted_64.txt", 1);
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
		std::cout << "Crack_64: " << elapsed_seconds << " seconds" << std::endl;
		
		t1 = std::chrono::high_resolution_clock::now();
	Crack_64(path + "publicKey_64.txt", path + "encrypted_64.txt", 2);
		t2 = std::chrono::high_resolution_clock::now();
		elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
		std::cout << "Crack_64 (V): " << elapsed_seconds << " seconds" << std::endl;
		
	//Testing the final Merkle-Hellman implementation with BigNum
//	CreateKey_BN(keyLength, path + "privateKey_BN.txt", path + "publicKey_BN.txt");
//	Encrypt_BN(message, path + "publicKey_BN.txt", path + "encrypted_BN.txt");
//	
//		t1 = std::chrono::high_resolution_clock::now();
//	Decrypt_BN(path + "privateKey_BN.txt", path + "encrypted_BN.txt");
//		t2 = std::chrono::high_resolution_clock::now();
//		elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
//		std::cout << "Decrypt_BN: " << elapsed_seconds << " seconds" << std::endl;

//		t1 = std::chrono::high_resolution_clock::now();
//	Crack_BN(path + "publicKey_BN.txt", path + "encrypted_BN.txt");
//		t2 = std::chrono::high_resolution_clock::now();
//		elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
//		std::cout << "Crack_BN: " << elapsed_seconds << " seconds" << std::endl;
//		
//		t1 = std::chrono::high_resolution_clock::now();
//	Crack_BN(path + "publicKey_64.txt", path + "encrypted_64.txt");
//		t2 = std::chrono::high_resolution_clock::now();
//		elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
//		std::cout << "Crack_BN (64): " << elapsed_seconds << " seconds" << std::endl;
}
