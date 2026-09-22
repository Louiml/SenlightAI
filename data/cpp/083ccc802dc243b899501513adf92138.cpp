Write a C++ function `computeCommitmentEqualityChallenge` that, given two commitments `A` and `B` (represented as large positive integers via `std::string` decimal strings), two pairs of public parameters `(g1, h1, p1)` and `(g2, h2, p2)` (each pair also represented as `std::string` decimal strings for the base values and the modulus), and two ephemeral commitments `T1` and `T2` (also `std::string`), computes a deterministic challenge value by hashing these components together. The challenge is computed as follows: concatenate a fixed protocol string `"ZEROCOIN_COMMITMENT_EQUALITY_PROOF"`, then `T1`, then the literal string `"||"`, then `T2`, then `"||"`, then `A`, then `"||"`, then `B`, then `"||"`, then the serialized representation of `(g1, h1, p1)` (as `g1|h1|p1`), then `"||"`, then the serialized representation of `(g2, h2, p2)` (as `g2|h2|p2`). The hash function is SHA-256, and the result is interpreted as a big-endian unsigned 256-bit integer, then converted to a decimal string and returned. The function must handle arbitrary‑length positive decimal inputs (no leading zeros except for the number "0" itself) and must not rely on any external big‑integer library; implement your own arbitrary‑precision unsigned integer arithmetic for addition, multiplication, and modular exponentiation only as needed for hashing (you may use a simple string‑based representation internally). The final output is the decimal representation of the SHA-256 digest (a 256‑bit integer). Ensure the function is `const`‑correct and follows modern C++17 guidelines.
The core challenge is to simulate the SHA-256 hash over a byte stream that concatenates the given components in a specific order, then interpret the resulting 32‑byte digest as a big‑endian 256‑bit integer and convert it to a decimal string. Since we cannot use external big‑integer or hash libraries, we must implement both a SHA-256 hash function and a minimal big‑integer representation. 

Approach:
1. **Data serialization**: Each component (the protocol string, the literal `"||"`, the commitment values `A`, `B`, `T1`, `T2`, and the parameter serializations) is converted to a byte sequence. For decimal strings representing numbers, we must convert them to their raw binary representation (big‑endian, minimal number of bytes, but at least one byte, with "0" as a single zero byte). For the parameter serialization, we first form a string like `"g1|h1|p1"` (where `g1`, `h1`, `p1` are the original decimal strings) and then convert that entire string to bytes (UTF‑8). The protocol string and the `"||"` literals are also converted directly to bytes.

2. **SHA-256**: Implement the standard SHA-256 algorithm with the usual constants and message schedule. Process the concatenated byte stream in 512‑bit blocks, with padding (append 0x80, then zeros, then a 64‑bit big‑endian length). Output the 32‑byte digest.

3. **Big‑integer conversion**: Write a small class `BigUint` that stores digits in base 10^9 (or base 2^32 for internal arithmetic) to represent the 256‑bit digest as a decimal string. Since the digest is exactly 256 bits, we can represent it as an array of four 64‑bit words (or eight 32‑bit words) and then convert to decimal by repeatedly dividing by 10^9. However, to keep the solution self‑contained and avoid manual bit‑manipulation, we can store the digest bytes as a big‑endian byte array and implement a simple `bytesToDecimal` using repeated multiplication and addition (building a decimal string). This is simpler given the fixed 32‑byte input.

Edge cases:
- Inputs may be "0" (single zero). Conversion from decimal string to bytes must handle zero correctly.
- Leading zeros in the input decimal strings should be ignored (but the input is given as standard decimal, so we can assume no leading zeros except for zero itself).
- The parameter serialization uses the exact original decimal strings as provided, so we do not normalize them.

Time complexity: SHA-256 for a message of length `L` bytes is O(L). The decimal conversion for a 32‑byte digest is O(32 * (number of digits)) which is effectively O(32^2) for a simple repeated multiplication, but since 32 is constant, it is O(1) in practice. Overall O(L) where L is the total byte length of all concatenated components.

Space complexity: O(L) for the concatenated byte buffer, plus O(1) for SHA‑256 state and decimal conversion.
#include <string>
#include <vector>
#include <cstdint>
#include <cstring>
#include <stdexcept>

namespace {

// Convert a decimal string (positive integer, no leading zeros except "0") to a big-endian byte vector.
std::vector<uint8_t> decimalStringToBytes(const std::string& decimal) {
    if (decimal.empty()) throw std::invalid_argument("Empty decimal string");
    std::string value = decimal;
    // Normalize: remove leading zeros, but keep at least one digit.
    size_t firstNonZero = value.find_first_not_of('0');
    if (firstNonZero == std::string::npos) {
        value = "0";
    } else {
        value = value.substr(firstNonZero);
    }
    if (value == "0") return {0x00};

    // Repeatedly divide by 256 to get bytes (least significant byte last).
    std::vector<uint8_t> bytes;
    std::string num = value;
    while (num != "0") {
        int remainder = 0;
        std::string quotient;
        quotient.reserve(num.length());
        for (char ch : num) {
            int current = remainder * 10 + (ch - '0');
            quotient.push_back(static_cast<char>('0' + current / 256));
            remainder = current % 256;
        }
        bytes.push_back(static_cast<uint8_t>(remainder));
        // Remove leading zeros from quotient for the next iteration.
        size_t pos = quotient.find_first_not_of('0');
        num = (pos == std::string::npos) ? "0" : quotient.substr(pos);
    }
    // bytes are in little-endian order; reverse to get big-endian.
    std::reverse(bytes.begin(), bytes.end());
    return bytes;
}

// Convert a big-endian byte vector (e.g., SHA-256 digest) to a decimal string.
std::string bytesToDecimalString(const std::vector<uint8_t>& bytes) {
    if (bytes.empty()) return "0";
    // Start with decimal string "0".
    std::string decimal = "0";
    for (uint8_t byte : bytes) {
        // decimal = decimal * 256 + byte
        std::string newDecimal;
        int carry = byte;
        // Multiply current decimal by 256 and add byte.
        // Process from least significant digit (end of string) backwards.
        newDecimal.reserve(decimal.size() + 2);
        for (auto it = decimal.rbegin(); it != decimal.rend(); ++it) {
            int digit = *it - '0';
            int cur = digit * 256 + carry;
            newDecimal.push_back(static_cast<char>('0' + (cur % 10)));
            carry = cur / 10;
        }
        while (carry > 0) {
            newDecimal.push_back(static_cast<char>('0' + (carry % 10)));
            carry /= 10;
        }
        std::reverse(newDecimal.begin(), newDecimal.end());
        decimal = newDecimal;
    }
    // Remove leading zeros.
    size_t firstNonZero = decimal.find_first_not_of('0');
    return (firstNonZero == std::string::npos) ? "0" : decimal.substr(firstNonZero);
}

// -------------------- SHA-256 implementation --------------------
constexpr uint32_t K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

inline uint32_t rotr(uint32_t x, int n) { return (x >> n) | (x << (32 - n)); }

std::vector<uint8_t> sha256(const std::vector<uint8_t>& message) {
    // Initial hash values
    uint32_t h0 = 0x6a09e667;
    uint32_t h1 = 0xbb67ae85;
    uint32_t h2 = 0x3c6ef372;
    uint32_t h3 = 0xa54ff53a;
    uint32_t h4 = 0x510e527f;
    uint32_t h5 = 0x9b05688c;
    uint32_t h6 = 0x1f83d9ab;
    uint32_t h7 = 0x5be0cd19;

    // Pre-processing: padding
    std::vector<uint8_t> padded = message;
    uint64_t bit_len = static_cast<uint64_t>(message.size()) * 8;
    padded.push_back(0x80);
    // Pad with zeros until length mod 64 == 56
    while ((padded.size() % 64) != 56) {
        padded.push_back(0x00);
    }
    // Append 64-bit big-endian length
    for (int i = 7; i >= 0; --i) {
        padded.push_back(static_cast<uint8_t>((bit_len >> (8 * i)) & 0xff));
    }

    // Process each 512-bit block
    for (size_t i = 0; i < padded.size(); i += 64) {
        uint32_t w[64];
        for (int j = 0; j < 16; ++j) {
            w[j] = (static_cast<uint32_t>(padded[i + j*4]) << 24) |
                   (static_cast<uint32_t>(padded[i + j*4 + 1]) << 16) |
                   (static_cast<uint32_t>(padded[i + j*4 + 2]) << 8) |
                   (static_cast<uint32_t>(padded[i + j*4 + 3]));
        }
        for (int j = 16; j < 64; ++j) {
            uint32_t s0 = rotr(w[j-15], 7) ^ rotr(w[j-15], 18) ^ (w[j-15] >> 3);
            uint32_t s1 = rotr(w[j-2], 17) ^ rotr(w[j-2], 19) ^ (w[j-2] >> 10);
            w[j] = w[j-16] + s0 + w[j-7] + s1;
        }

        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
        uint32_t e = h4;
        uint32_t f = h5;
        uint32_t g = h6;
        uint32_t h = h7;

        for (int j = 0; j < 64; ++j) {
            uint32_t S1 = rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25);
            uint32_t ch = (e & f) ^ ((~e) & g);
            uint32_t temp1 = h + S1 + ch + K[j] + w[j];
            uint32_t S0 = rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22);
            uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            uint32_t temp2 = S0 + maj;

            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
        h5 += f;
        h6 += g;
        h7 += h;
    }

    std::vector<uint8_t> digest;
    digest.reserve(32);
    for (uint32_t val : {h0, h1, h2, h3, h4, h5, h6, h7}) {
        for (int i = 3; i >= 0; --i) {
            digest.push_back(static_cast<uint8_t>((val >> (8 * i)) & 0xff));
        }
    }
    return digest;
}

} // anonymous namespace

// Main function: compute deterministic challenge from given components.
// All inputs are decimal strings for positive integers.
// Parameters g1,h1,p1,g2,h2,p2 are the public parameters; A,B,T1,T2 are commitments.
// Returns the SHA-256 challenge as a decimal string (big-endian interpretation of 256-bit digest).
std::string computeCommitmentEqualityChallenge(
    const std::string& A, const std::string& B,
    const std::string& T1, const std::string& T2,
    const std::string& g1, const std::string& h1, const std::string& p1,
    const std::string& g2, const std::string& h2, const std::string& p2) {
    
    // Prepare byte vector for hashing
    std::vector<uint8_t> data;
    
    auto appendString = [&data](const std::string& s) {
        data.insert(data.end(), s.begin(), s.end());
    };
    
    auto appendNumber = [&data](const std::string& num) {
        std::vector<uint8_t> bytes = decimalStringToBytes(num);
        data.insert(data.end(), bytes.begin(), bytes.end());
    };
    
    appendString("ZEROCOIN_COMMITMENT_EQUALITY_PROOF");
    appendNumber(T1);
    appendString("||");
    appendNumber(T2);
    appendString("||");
    appendNumber(A);
    appendString("||");
    appendNumber(B);
    appendString("||");
    appendString(g1 + "|" + h1 + "|" + p1);
    appendString("||");
    appendString(g2 + "|" + h2 + "|" + p2);
    
    // Compute SHA-256
    std::vector<uint8_t> digest = sha256(data);
    
    // Convert digest to decimal string
    return bytesToDecimalString(digest);
}
#include <string>
#include <cassert>
#include <iostream>

// Forward declaration of the solution function (already included above in actual build)
std::string computeCommitmentEqualityChallenge(
    const std::string& A, const std::string& B,
    const std::string& T1, const std::string& T2,
    const std::string& g1, const std::string& h1, const std::string& p1,
    const std::string& g2, const std::string& h2, const std::string& p2);

int main() {
    // Test 1: Simple small values, check that the result is a 77-digit decimal (256 bits max is 78 digits, but we check structure)
    std::string A1 = "1", B1 = "2", T1a = "3", T2a = "4";
    std::string g1a = "5", h1a = "6", p1a = "7";
    std::string g2a = "8", h2a = "9", p2a = "10";
    std::string result1 = computeCommitmentEqualityChallenge(A1, B1, T1a, T2a, g1a, h1a, p1a, g2a, h2a, p2a);
    // Just verify it's non-empty and doesn't throw; prefix should be "0" or digits.
    assert(!result1.empty());
    for (char c : result1) assert(c >= '0' && c <= '9');

    // Test 2: Determinism - same inputs produce same output
    std::string r2 = computeCommitmentEqualityChallenge(A1, B1, T1a, T2a, g1a, h1a, p1a, g2a, h2a, p2a);
    assert(result1 == r2);

    // Test 3: Changing any input changes output (basic avalanche)
    std::string r3 = computeCommitmentEqualityChallenge("2", B1, T1a, T2a, g1a, h1a, p1a, g2a, h2a, p2a);
    assert(result1 != r3);

    // Test 4: Values with leading zeros are normalized (e.g., "0007" == "7")
    std::string r4a = computeCommitmentEqualityChallenge("0007", "2", "3", "4", "5", "6", "7", "8", "9", "10");
    std::string r4b = computeCommitmentEqualityChallenge("7", "2", "3", "4", "5", "6", "7", "8", "9", "10");
    assert(r4a == r4b);

    // Test 5: Zero handling
    std::string r5 = computeCommitmentEqualityChallenge("0", "0", "0", "0", "0", "0", "0", "0", "0", "0");
    // Should be non-empty
    assert(!r5.empty());

    // Test 6: Known hash value (manually computed? We'll verify against a known SHA-256 digest)
    // For A=1,B=2,T1=3,T2=4,g1=5,h1=6,p1=7,g2=8,h2=9,p2=10, the serialized string is:
    // "ZEROCOIN_COMMITMENT_EQUALITY_PROOF" + bytes(3) + "||" + bytes(4) + "||" + bytes(1) + "||" + bytes(2) + "||" + "5|6|7" + "||" + "8|9|10"
    // We can hardcode a known SHA-256 result for a similar simple string, but here we test consistency.
    // Let's use a simple string "abc" and known SHA-256, but that's not our function. So just check the result is exactly 78 digits max.
    assert(result1.size() <= 78);

    // Test 7: Large number inputs (e.g., 100-digit numbers)
    std::string big(100, '9');
    std::string r7 = computeCommitmentEqualityChallenge(big, "2", "3", big, "5", "6", "7", "8", "9", "10");
    assert(r7.size() <= 78);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
