#include <iostream> // i/o operations
#include <iomanip> //formatting output into hexadecial
#include <sstream> // string stream operations
#include <string>  // string operations
#include <vector> // for dynamic arrays.
#include <cstdint> // fixed width intergers
#include <fstream> // file input and output
using namespace std;

// array of round constants(Fractional parts of cube roots from first 64 prime numbers)

const uint32_t K[64] = 
{
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};
//stage -3
// right rotation Function.

inline uint32_t rightrotr(uint32_t x, uint32_t n) 
{
    return (x >> n) | (x << (32 - n));
}

//stage - 4
// SHA-256 compression function

void sha256_transform(uint32_t state[8], const uint8_t data[64]) 
{
    uint32_t a, b, c, d, e, f, g, h, t1, t2, m[64];
    const uint8_t *p = data;

    for (int i = 0; i < 16; ++i) 
    {
        m[i] = (uint32_t)p[0] << 24 | (uint32_t)p[1] << 16 | (uint32_t)p[2] << 8 | (uint32_t)p[3];
        p += 4;
    }

    for (int i = 16; i < 64; ++i) 
    {
        uint32_t s0 = rightrotr(m[i-15], 7) ^ rightrotr(m[i-15], 18) ^ (m[i-15] >> 3);
        uint32_t s1 = rightrotr(m[i-2], 17) ^ rightrotr(m[i-2], 19) ^ (m[i-2] >> 10);
        m[i] = m[i-16] + s0 + m[i-7] + s1;
    }

// intialize working variables
    a = state[0]; 
    b = state[1]; 
    c = state[2]; 
    d = state[3];
    e = state[4]; 
    f = state[5]; 
    g = state[6]; 
    h = state[7];

    for (int i = 0; i < 64; ++i)
     {
        uint32_t S1 = rightrotr(e, 6) ^ rightrotr(e, 11) ^ rightrotr(e, 25);
        uint32_t ch = (e & f) ^ (~e & g);
        uint32_t temp1 = h + S1 + ch + K[i] + m[i];
        uint32_t S0 = rightrotr(a, 2) ^ rightrotr(a, 13) ^ rightrotr(a, 22);
        uint32_t majority = (a & b) ^ (a & c) ^ (b & c);
        uint32_t temp2 = S0 + majority;

        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }

    state[0] += a;
    state[1] += b; 
    state[2] += c; 
    state[3] += d;
    state[4] += e; 
    state[5] += f; 
    state[6] += g; 
    state[7] += h;
}

// SHA-256 hashing function

string sha256(const string& input)
 {
    uint32_t state[8] =
     {
        0x6a09e667, 
        0xbb67ae85, 
        0x3c6ef372, 
        0xa54ff53a, 
        0x510e527f, 
        0x9b05688c, 
        0x1f83d9ab, 
        0x5be0cd19  
    };
     

    uint64_t bitlen = input.size() * 8;
    uint64_t bitlen_be = __builtin_bswap64(bitlen);

    vector<uint8_t> data(input.begin(), input.end());
    data.push_back(0x80);

    while ((data.size() + 8) % 64 != 0)
    {
        data.push_back(0x00);
    }

    for (int i = 0; i < 8; ++i) 
    {
        data.push_back((bitlen_be >> (56 - i * 8)) & 0xff);
    }

    for (size_t i = 0; i < data.size(); i += 64) 
    {
        sha256_transform(state, &data[i]);
    }

    stringstream ss;
    for (int i = 0; i < 8; ++i) 
    {
        ss <<hex <<setfill('0') <<setw(8) << __builtin_bswap32(state[i]);
    }

    return ss.str();
}

// stage - 2 Function to read text from a file
string read_file(const string& filepath)
{
    ifstream file(filepath);
    if (!file.is_open()) 
    {
        cerr << "Failed to open file: " << filepath << endl;
        return "";
    }
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    return content;
}

//stage -1 
int main() 
{
    string filepath = "/workspaces/adv_alg-Final/Final/Mark.txt"; 
    string text = read_file(filepath);

    if (text.empty()) 
    {
        cerr << "Failed to read text from file." << endl;
        return 1;
    }

    string hash = sha256(text);
    cout << "SHA-256 hash of the book of Mark: " << hash << endl;

    return 0;
}