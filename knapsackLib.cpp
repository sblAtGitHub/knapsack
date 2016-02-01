#include "knapsackLib.hpp"

//holds the amount of bits used to represent an ASCII character
const int CHAR_BITS = 8;

aligned_vector<int> StringToBits(std::string aString) {
	aligned_vector<int> result(aString.size() * CHAR_BITS, 0);
	for(unsigned int i=0; i<aString.size(); ++i) {
		for(int j=0; j<CHAR_BITS; ++j) {
			result[(int)i*CHAR_BITS+j] = (aString[i] >> (CHAR_BITS-1-j)) & 1;
		}
	}
	return result;
}

std::string BitsToString(aligned_vector<int> aBits) {
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

void CreateKey_64(int aSize, std::string aPublicKeyFile) {

	std::random_device rd;
	std::mt19937_64 e2(rd());
	std::uniform_int_distribution<uint64_t> dist(0,(0ULL - 1)/aSize);
	std::vector<uint64_t> vector_public(aSize);
	for(int i=0; i<aSize; ++i) {
		uint64_t r = dist(e2);
		bool insert = true;
		for(int j=0; j<i; ++j) {
			if(vector_public[j]==r) {
				--i;
				insert = false;
				break;
			}
		}		
		if(insert)
			vector_public[i] = r;
	}

	std::ofstream publicKeyFile (aPublicKeyFile);
	if (publicKeyFile.is_open()) {
		for(int i=0; i<aSize; ++i) {
			publicKeyFile << vector_public[i] << std::endl;
		}
		publicKeyFile.close();
	}
}

void Encrypt_64(std::string aMessage, std::string aPublicKeyFile, std::string aCryptFile) {
	
	//transform message into bits
	aligned_vector<int> messageVector = StringToBits(aMessage);
	
	//read public key from file
	aligned_vector<uint64_t> publicKeyVector;
	std::string line;
	std::ifstream publicKeyFile (aPublicKeyFile);
	if (publicKeyFile.is_open()) {
		uint64_t publicKeySegment;
		while(publicKeyFile >> publicKeySegment) {
			publicKeyVector.push_back(publicKeySegment);
		}
		publicKeyFile.close();
    }
	
	//divide message into segments depending on the key-length and compute the dot product for each segment
	int segSize = publicKeyVector.size();
	int segs = ceil((double)messageVector.size()/segSize);
	aligned_vector<uint64_t> cryptVector(segs, 0);
	for(int seg=0; seg<segs; ++seg) {
		int first = seg*segSize;
		int last = first+segSize > (int)messageVector.size() ? messageVector.size() : first+segSize;
		aligned_vector<int> messageSeg(&messageVector[first], &messageVector[last]);
		for(int i=0; i<segSize; ++i) {
			cryptVector[seg] += messageSeg[i] ? publicKeyVector[i] : 0;
		}
	}
	
	//write encrypted message to file
	std::ofstream cryptFile (aCryptFile);
	if (cryptFile.is_open()) {
		for(int i=0; i<(int)cryptVector.size(); ++i) {
			cryptFile << cryptVector[i] << std::endl;
		}
		cryptFile.close();
	}
}

void Crack_64(std::string aPublicKeyFile, std::string aCryptFile, int aVersion) {

	//read public key from file
	aligned_vector<uint64_t> publicKeyVector;
	std::string line;
	std::ifstream publicKeyFile (aPublicKeyFile);
	if (publicKeyFile.is_open()) {
		uint64_t publicKeySegment;
		while(publicKeyFile >> publicKeySegment) {
			publicKeyVector.push_back(publicKeySegment);
		}
		publicKeyFile.close();
    }
    
	//read encrypted message from file
    aligned_vector<uint64_t> cryptVector;
	std::ifstream cryptFile (aCryptFile);
	if (cryptFile.is_open()) {
		uint64_t cryptSegment;
		while(cryptFile >> cryptSegment) {
			cryptVector.push_back(cryptSegment);
		}
		cryptFile.close();
    }
    
    //start cracking
   	int keyLength = (int) publicKeyVector.size();
	int cryptLength = (int) cryptVector.size();   	
   	aligned_vector<int> plainVector(keyLength*cryptLength);
   	for(unsigned int i=0; i<cryptVector.size(); ++i) {
   		if(aVersion == 1)
			ResolveDP_64_P(publicKeyVector.data(), keyLength, cryptVector[i], plainVector.data(), i*keyLength);
		if(aVersion == 2)
			ResolveDP_64_PV2(publicKeyVector.data(), keyLength, cryptVector[i], plainVector.data(), i*keyLength);
    }
    
    std::string plainString = BitsToString(plainVector);
	std::cout << plainString << std::endl;
}

//version with one big test-vector (quite memory consuming)
void ResolveDP_64_PV(uint64_t const* aKeyVector, uint64_t const aKeyLength, uint64_t const aDP, int * aPlainVector, int const aPlainVectorPosition) {

	uint64_t n_max = pow(2, aKeyLength);
	
	aligned_vector<uint64_t> testVector(aKeyLength * n_max, 0);
	
	#pragma omp parallel
	{
		#pragma omp for schedule(static)
		for(uint64_t n=0; n<n_max; ++n) {		
			uint64_t result=0;
			
			uint64_t * pTestVector = testVector.data();
			//init testVector on position n*aKeyLength
			for(int k=0; k<aKeyLength; ++k) {
				pTestVector[n*aKeyLength+k] = ((n >> k) & 1);
			}
			
			#pragma omp simd aligned(pTestVector,aKeyVector:64) reduction(+:result)
			for(int i=0; i<aKeyLength; ++i) {
				result += pTestVector[n*aKeyLength+i] * aKeyVector[i];
			}
			if(aDP == result) {
				#pragma omp simd aligned(pTestVector,aPlainVector:64)
				for(int i=0; i<aKeyLength; ++i) {
					aPlainVector[aPlainVectorPosition + i] = pTestVector[n*aKeyLength+i];
				}
				#pragma omp cancel for
			}
		}
	}
}

//version with a short test-vector for every thread which is re-initialized in every loop step)
void ResolveDP_64_PV2(uint64_t const* aKeyVector, uint64_t const aKeyLength, uint64_t const aDP, int * aPlainVector, int const aPlainVectorPosition) {

	uint64_t n_max = pow(2, aKeyLength);
	
	int nThreads = 0;
	#pragma omp parallel
	{
		#pragma omp single
		nThreads = omp_get_num_threads();
	}

	int maxKeyLength = 64;
	aligned_vector<uint64_t> testVector(maxKeyLength * nThreads, 0);
	
	#pragma omp parallel
	{
		//get aligned section of test vector and plain vector for each thread
		int threadNo = omp_get_thread_num();
		uint64_t * pThreadTestVector = &testVector[threadNo * maxKeyLength];
		int * pThreadPlainVector = &aPlainVector[aPlainVectorPosition];				
		
		#pragma omp for schedule(static)
		for(uint64_t n=0; n<n_max; ++n) {		
			uint64_t result=0;
						
			#pragma omp simd aligned(pThreadTestVector,aKeyVector:64) reduction(+:result)
			for(int i=0; i<aKeyLength; ++i) {
				pThreadTestVector[i] = ((n >> i) & 1);
				result += pThreadTestVector[i] * aKeyVector[i];
			}
			if(aDP == result) {
				#pragma omp simd aligned(pThreadTestVector,pThreadPlainVector:64)
				for(int i=0; i<aKeyLength; ++i) {
					pThreadPlainVector[i] = pThreadTestVector[i];
				}
				#pragma omp cancel for
			}
		}
	}
}

void ResolveDP_64_P(uint64_t const* aKeyVector, uint64_t const aKeyLength, uint64_t const aDP, int * aPlainVector, int const aPlainVectorPosition) {

	uint64_t n_max = pow(2, aKeyLength);
	#pragma omp parallel
	{
		#pragma omp for schedule(static)
		for(uint64_t n=0; n<n_max; ++n) {		
			uint64_t result=0;
			#pragma omp simd aligned(aKeyVector:64) reduction(+:result)
			for(int i=0; i<aKeyLength; ++i) {
				result += ((n >> i) & 1) * aKeyVector[i];
			}
			if(aDP == result) {
				#pragma omp simd aligned(aPlainVector:64)
				for(int i=0; i<aKeyLength; ++i) {
					aPlainVector[aPlainVectorPosition + i] = ((n >> i) & 1);
				}
				#pragma omp cancel for
			}
		}
	}
}
	
void ResolveDP_64_S(uint64_t const* aKeyVector, uint64_t const aKeyLength, uint64_t const aDP, int * aPlainVector, int const aPlainVectorPosition) {
	for(uint64_t n=0; n<pow(2, aKeyLength); ++n) {		
		uint64_t result=0;
		#pragma omp simd aligned(aKeyVector,aPlainVector:64) reduction(+:result)
		for(int i=0; i<aKeyLength; ++i) {
			aPlainVector[aPlainVectorPosition + i] = (n >> i) & 1;
			result += aPlainVector[aPlainVectorPosition + i] * aKeyVector[i];
		}
		if(aDP == result) {
			return;
		}
	}
}

namespace bm = boost::multiprecision;
namespace br = boost::random;

void CreateKey_BN(int aSize, std::string aPrivateKeyFile, std::string aPublicKeyFile) {

	//interval for random numbers
	bm::int512_t min;
	bm::int512_t max;

	//random generator	
	br::mt19937 mt;
	br::uniform_int_distribution<bm::int512_t> uni;

	//create a super increasing vector of length n=aSize, whose elements a follow a__i=[(2^(i-1) - 1) * 2^100 + 1, 2^(i-1) * 2^100]
	std::vector<bm::int512_t> vector_private(aSize);
	for(int i=1; i<=aSize; ++i) {
		min = (bm::pow(bm::int512_t(2), (i-1))-1) * bm::pow(bm::int512_t(2), 100) + 1;
		max = bm::pow(bm::int512_t(2), i-1) * bm::pow(bm::int512_t(2), 100);
		uni = br::uniform_int_distribution<bm::int512_t>(min, max);
		vector_private[i-1] = uni(mt);
	}
	
	//choose m, with m=[2^201 + 1, 2^202 - 1]
	min = bm::pow(bm::int512_t(2), 201) + 1;
	max = bm::pow(bm::int512_t(2), 202) - 1;
	uni = br::uniform_int_distribution<bm::int512_t>(min, max);
	bm::int512_t m = uni(mt);
	
	//choose w_ with w_=[2, m-2]
	min = 2;
	max = m - 2;
	uni = br::uniform_int_distribution<bm::int512_t>(min, max);
	bm::int512_t w_ = uni(mt);
	
	//compute w by finding the greatest common divisor of (m,w_), so that w=w_/gcd(m,w_)
	bm::int512_t w = w_ / bm::gcd(m, w_);
	
	//create the public vector with elements a_i=w*a__i%m
	std::vector<bm::int512_t> vector_public(aSize);
	for(int i=0; i<aSize; ++i) {
		vector_public[i] = (w * vector_private[i]) % m;
	}
	
	//write keys to files
	std::ofstream privateKeyFile (aPrivateKeyFile);
	if (privateKeyFile.is_open()) {
		privateKeyFile << m << std::endl;
		privateKeyFile << w << std::endl;
		for(int i=0; i<aSize; ++i) {
			privateKeyFile << vector_private[i] << std::endl;
		}
		privateKeyFile.close();
	}
	std::ofstream publicKeyFile (aPublicKeyFile);
	if (publicKeyFile.is_open()) {
		for(int i=0; i<aSize; ++i) {
			publicKeyFile << vector_public[i] << std::endl;
		}
		publicKeyFile.close();
	}
}

void Encrypt_BN(std::string aMessage, std::string aPublicKeyFile, std::string aCryptFile) {
	
	//transform message into bits
	aligned_vector<int> messageVector = StringToBits(aMessage);
	
	//read public key from file
	aligned_vector<bm::int512_t> publicKeyVector;
	std::string line;
	std::ifstream publicKeyFile (aPublicKeyFile);
	if (publicKeyFile.is_open()) {
		bm::int512_t publicKeySegment;
		while(publicKeyFile >> publicKeySegment) {
			publicKeyVector.push_back(publicKeySegment);
		}
		publicKeyFile.close();
    }
	
	//divide message into segments depending on the key-length and compute the dot product for each segment
	int segSize = publicKeyVector.size();
	int segs = ceil((double)messageVector.size()/segSize);
	aligned_vector<bm::int512_t> cryptVector(segs, 0);
	for(int seg=0; seg<segs; ++seg) {
		int first = seg*segSize;
		int last = first+segSize > (int)messageVector.size() ? messageVector.size() : first+segSize;
		aligned_vector<int> messageSeg(&messageVector[first], &messageVector[last]);
		for(int i=0; i<segSize; ++i) {
			cryptVector[seg] += messageSeg[i] ? publicKeyVector[i] : 0;
		}	
	}
	
	//write encrypted message to file
	std::ofstream cryptFile (aCryptFile);
	if (cryptFile.is_open()) {
		for(int i=0; i<(int)cryptVector.size(); ++i) {
			cryptFile << cryptVector[i] << std::endl;
		}
		cryptFile.close();
	}
}

void Decrypt_BN(std::string aPrivateKeyFile, std::string aCryptFile) {

	//read private key from file
	bm::int512_t m = 0;
	bm::int512_t w = 0;
	aligned_vector<bm::int512_t> privateKeyVector;
	std::string line;
	std::ifstream privateKeyFile (aPrivateKeyFile);
	if (privateKeyFile.is_open()) {
		privateKeyFile >> m;
		privateKeyFile >> w;
		bm::int512_t privateKeySegment;
		while(privateKeyFile >> privateKeySegment) {
			privateKeyVector.push_back(privateKeySegment);
		}
		privateKeyFile.close();
    }
	
	//read encrypted message from file
    aligned_vector<bm::int512_t> cryptVector;
	std::ifstream cryptFile (aCryptFile);
	if (cryptFile.is_open()) {
		bm::int512_t cryptSegment;
		while(cryptFile >> cryptSegment) {
			cryptVector.push_back(cryptSegment);
		}
		cryptFile.close();
    }

	//get multiplicative inverse of w modulo m
	bm::int512_t w_inv = MultInverse_BN(w, m);
	
	int keyLength = (int) privateKeyVector.size();
	int cryptLength = (int) cryptVector.size();   	
   	aligned_vector<int> plainVector(keyLength*cryptLength, 0);
	for(int i=0; i<(int)cryptVector.size(); ++i) {
   	
   		//transform the dp back to the super increasing vector dp
   		bm::int512_t dp = cryptVector[i];
   		bm::int512_t dp_priv = (dp*w_inv) % m;
   		
   		//resolve simple knapsack problem the greedy way
   		bm::int512_t sum = 0;
   		for(int j=(int)privateKeyVector.size()-1; j >= 0; --j) {
   			if(privateKeyVector[j] + sum <= dp_priv) {
   				sum += privateKeyVector[j];
   				plainVector[i*keyLength+j] = 1;
   			}
   		}
    }
    
    std::string plainString = BitsToString(plainVector);
	std::cout << plainString << std::endl;
}

bm::int512_t MultInverse_BN(bm::int512_t aW, bm::int512_t aM)
{
	bm::int512_t b0 = aM, t, q;
	bm::int512_t x0 = 0, x1 = 1;
	if (aM == 1) return 1;
	while (aW > 1) {
		q = aW / aM;
		t = aM, aM = aW % aM, aW = (bm::int512_t)t;
		t = x0, x0 = x1 - q * x0, x1 = t;
	}
	if (x1 < 0) {
		x1 += b0;
	}
	return x1;
}

void Crack_BN(std::string aPublicKeyFile, std::string aCryptFile) {
	
	//read public key from file
	aligned_vector<bm::int512_t> publicKeyVector;
	std::string line;
	std::ifstream publicKeyFile (aPublicKeyFile);
	if (publicKeyFile.is_open()) {
		bm::int512_t publicKeySegment;
		while(publicKeyFile >> publicKeySegment) {
			publicKeyVector.push_back(publicKeySegment);
		}
		publicKeyFile.close();
    }
    
	//read encrypted message from file
    aligned_vector<bm::int512_t> cryptVector;
	std::ifstream cryptFile (aCryptFile);
	if (cryptFile.is_open()) {
		bm::int512_t cryptSegment;
		while(cryptFile >> cryptSegment) {
			cryptVector.push_back(cryptSegment);
		}
		cryptFile.close();
    }
    
    //start cracking
   	int keyLength = (int) publicKeyVector.size();
	int cryptLength = (int) cryptVector.size();   	
   	aligned_vector<int> plainVector(keyLength*cryptLength);
   	for(unsigned int i=0; i<cryptVector.size(); ++i) {    
    	ResolveDP_BN_P(publicKeyVector.data(), keyLength, cryptVector[i], plainVector.data(), i*keyLength);
    }
    
    std::string plainString = BitsToString(plainVector);
	std::cout << plainString << std::endl;
}

void ResolveDP_BN_P(bm::int512_t const* aKeyVector, int const aKeyLength, bm::int512_t const aDP, int * aPlainVector, int const aPlainVectorPosition) {

	//here is the reason why the key-length must not exceed a value of 64
	//both the omp framework and the bitshift operation are implemented for c++ integer types only
	//this could be resolved by chunking the work in 2^64 step pieces

	uint64_t n_max = pow(2, aKeyLength);
	#pragma omp parallel
	{
		#pragma omp for schedule(static)
		for(uint64_t n=0; n<n_max; ++n) {		
			bm::int512_t result=0;
			for(int i=0; i<aKeyLength; ++i) {
				result += ((n >> i) & 1) * aKeyVector[i];
			}
			if(aDP == result) {
				for(int i=0; i<aKeyLength; ++i) {
					aPlainVector[aPlainVectorPosition + i] = ((n >> i) & 1);
				}
				#pragma omp cancel for
			}
		}
	}
}












