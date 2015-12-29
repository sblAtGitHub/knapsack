#Comments on the work in progress

**Structure of the project**
* all functions needed are declared in `knapsackLib.hpp` and implemented in `knapsackLib.cpp`
* some functions show up in different versions for 64-bit integers and BigNums, these functions are indicated by postfixes _64 respectively _BN
* `main.cpp` holds as a test-container right now, there might be a GUI later
* unit-tests are declared in `test.hpp` and implemented in `test.cpp`
* the whole work is in an early stage, so there is no exception handling or detailed documentation yet
  
**Functions implemented so far**  
* `bool IsCoPrime (uint64_t aX, uint64_t aY)`
    * Will be needed to create the key pair and the trapdoor values.
* `std::vector<int> StringToBits(std::string aString)`
    * The original message needs to be transformed in a vector of bits to be one part of the dot product. The method is based on ASCII characters, so one character takes 8 ints (0 or 1) in the resulting vector. The order of the bits is most significant first.
* `std::string BitsToString(std::vector<int> aBits)`
    * The counterpart to `StringToBits`...
* `void IncrementVector(std::vector<int>& aVector)`
    * To break the encryption, all combinations of the input vector have to be tried out. To get all this combinations, the vector is interpreted as a representation of a binary number and iteratively increased by 1.
* `uint64_t GetDotProduct_64(std::vector<int>& aBits, std::vector<uint64_t>& aKey)`
    * Computes the dot product of the given vectors, which basicly means to sum up the elements of the key-vector where the corresponding element of the bit-vector is 1.
* `std::vector<uint64_t> GetVector_64(int aSize)`
    * Creates a random encryption vector (the public key). It's not possible to implement the method of key-creation published by Merkle and Hellman with 64-bit integers. So here just random numbers are used.
* `std::vector<uint64_t> Encrypt_64(std::vector<int>& aMessage, std::vector<uint64_t>& aKey)`
    * Encrypts the given message, which is just to compute the dot product of the given message and key.
* `std::vector<int> Break_64(std::vector<uint64_t>& aCrypt, std::vector<uint64_t>& aKey)`
    * Tries to break the encryption in a brute-force like way. All possible message-vectors are combined with the key to find the one which results in the given cryptotext. This is a valid method for small key lengths but grows exponentially in time with increasing length of the key.
    * This method is subject for parallelization and vectorization to use the full capabilities of the given parallel computing environment of the university lab.
