#Purpose of the Code
The code is designed to compute the SHA-256 hash of the contents of a file. SHA-256 is a cryptographic hash function that takes an input (in this case, the contents of a file) and produces a fixed-size (256-bit) hash value. This hash value is unique to the input data, meaning even a small change in the input will produce a completely different hash.

#How the Code Works
1. Read the File:
    * The program reads the contents of a file specified by the user (via a file path).
    * If the file cannot be opened, it displays an error message and exits.
2. SHA-256 Hashing:
    * The file's contents are processed using the SHA-256 algorithm, which involves:
        * Padding the input data to meet the block size requirements (64 bytes).
        * Breaking the data into 512-bit (64-byte) chunks.
        * Applying a series of mathematical operations (bitwise operations, modular addition, etc.) to each chunk to compute the hash.
3. Output the Hash:
    * The final 256-bit hash is converted into a 64-character hexadecimal string and displayed to the user.

#Key Components of the Code 
1. SHA-256 Constants:
    * The K array contains 64 constant values used in the SHA-256 algorithm. These are derived from the fractional parts of the cube roots of the first 64 prime numbers.
2. Right Rotation (rightrotr):
    * A helper function that performs a circular right shift (rotation) on a 32-bit integer. This is used extensively in the SHA-256 algorithm.
3. SHA-256 Transform Function (sha256_transform):
    * This is the core of the SHA-256 algorithm. It processes a 512-bit (64-byte) chunk of data and updates the internal state (state[8]) of the hash computation.
4. SHA-256 Hashing Function (sha256):
    * This function prepares the input data (padding, appending length) and processes it in 512-bit chunks using the sha256_transform function.
    * It returns the final hash as a 64-character hexadecimal string.
5. File Reading (read_file):
    * Reads the contents of a file into a string. If the file cannot be opened, it returns an empty optional (nullopt).
6. Main Function:
    * Takes a file path as input, reads the file, computes its SHA-256 hash, and prints the result.


#Why SHA-256? 
SHA-256 is widely used for:
* Data Integrity Verification: Ensuring that data has not been tampered with.
* Digital Signatures: Verifying the authenticity of data.
* Blockchain Technology: Used in cryptocurrencies like Bitcoin for mining and transaction verification.


#Why This Code is Useful
* It provides a way to verify the integrity of a file. If the file is modified, the hash will change.
* It can be used in security applications, such as verifying downloaded files or ensuring data consistency.
