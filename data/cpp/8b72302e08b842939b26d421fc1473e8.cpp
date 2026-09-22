Write a C++ function named `computeX16RFamilyHash` that takes a hexadecimal string representing an 80-byte block header and an integer algorithm selector (1 or 2), and returns the hexadecimal string of the resulting 256-bit hash. For algorithm 1, apply the X16R hash function; for algorithm 2, apply the X16RV2 hash function. The function must parse the block header’s bytes 4–36 (inclusive) as the previous block hash (little-endian `uint256`) and pass that along with the raw header bytes to the corresponding hash function. If the algorithm selector is not 1 or 2, or the input hex is malformed (not a valid hex string or not exactly 80 bytes after parsing), return an empty string. The solution must not depend on external libraries other than standard C++ (e.g., no Bitcoin Core headers); you may implement your own minimal `uint256`-like wrapper or use `std::array<uint8_t,32>` and a placeholder hash implementation that produces a deterministic output based on XOR of header bytes and the previous block hash (to keep the task self-contained). The hash function should return a 64-character lowercase hex string. This task exercises byte parsing, hex conversion, header field extraction, and algorithm branching.

#include <cassert>

int main() {
    // Test with a valid 80-byte header: 160 hex characters (all zeros except a few)
    std::string headerHex(160, '0');
    headerHex[0] = 'a'; // make non-empty

    // Compute for algorithm 1 and 2, they should differ
    std::string hash1 = computeX16RFamilyHash(headerHex, 1);
    std::string hash2 = computeX16RFamilyHash(headerHex, 2);
    assert(hash1.size() == 64);
    assert(hash2.size() == 64);
    assert(hash1 != hash2);
    assert(hash1.length() == 64 && hash2.length() == 64);

    // Test invalid algorithm
    assert(computeX16RFamilyHash(headerHex, 3) == "");

    // Test invalid hex character
    std::string badHex = headerHex;
    badHex[0] = 'z';
    assert(computeX16RFamilyHash(badHex, 1) == "");

    // Test incorrect length (79 bytes)
    std::string shortHex = headerHex.substr(0, 158);
    assert(computeX16RFamilyHash(shortHex, 1) == "");

    // Test odd length hex
    assert(computeX16RFamilyHash("abc", 1) == "");

    // Test empty string
    assert(computeX16RFamilyHash("", 1) == "");

    // Test that previous block hash changes affect result
    std::string header2 = headerHex;
    header2[8] = 'b'; // change a byte in previous block hash region (bytes 4-35)
    std::string hash1b = computeX16RFamilyHash(header2, 1);
    assert(hash1 != hash1b);

    // Test determinism
    assert(computeX16RFamilyHash(headerHex, 1) == hash1);
    assert(computeX16RFamilyHash(headerHex, 2) == hash2);
}

#include <string>
#include <vector>
#include <cctype>
#include <array>
#include <stdexcept>

// Helper: convert a hex digit to its numeric value (0-15), or -1 if invalid.
int hexDigitValue(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

// Helper: convert a vector of bytes to a lowercase hex string.
std::string bytesToHex(const std::vector<unsigned char>& bytes) {
    static const char* hexChars = "0123456789abcdef";
    std::string result;
    result.reserve(bytes.size() * 2);
    for (unsigned char b : bytes) {
        result.push_back(hexChars[b >> 4]);
        result.push_back(hexChars[b & 0x0F]);
    }
    return result;
}

// Placeholder hash function that simulates X16R/X16RV2 output.
// Deterministic and based on header bytes and previous block hash.
std::array<unsigned char, 32> placeholderHash(
    const std::vector<unsigned char>& header, 
    const std::array<unsigned char, 32>& prevBlock, 
    int algo) 
{
    std::array<unsigned char, 32> result = prevBlock;
    // Mix header bytes into result
    for (size_t i = 0; i < header.size(); ++i) {
        result[i % 32] ^= header[i];
    }
    // For algorithm 2, apply an extra transformation to differentiate
    if (algo == 2) {
        for (size_t i = 0; i < 32; ++i) {
            result[i] = (result[i] << 1) | (result[i] >> 7); // rotate left by 1
        }
        // Additional mix with header reversed
        for (size_t i = 0; i < 32; ++i) {
            result[i] ^= header[header.size() - 1 - i];
        }
    }
    return result;
}

// Main solution function: compute hash hex string from block header hex and algorithm.
std::string computeX16RFamilyHash(const std::string& headerHex, int algorithm) {
    // Validate algorithm
    if (algorithm != 1 && algorithm != 2) return "";

    // Parse hex string
    std::vector<unsigned char> rawHeader;
    rawHeader.reserve(headerHex.size() / 2);
    int high = -1;
    for (char c : headerHex) {
        int val = hexDigitValue(c);
        if (val == -1) return ""; // invalid hex character
        if (high == -1) {
            high = val;
        } else {
            rawHeader.push_back(static_cast<unsigned char>((high << 4) | val));
            high = -1;
        }
    }
    // Must have even length and exactly 80 bytes
    if (high != -1 || rawHeader.size() != 80) return "";

    // Extract previous block hash (bytes 4..35 inclusive)
    std::array<unsigned char, 32> prevBlock{};
    for (int i = 0; i < 32; ++i) {
        prevBlock[i] = rawHeader[4 + i];
    }

    // Compute hash
    auto hashBytes = placeholderHash(rawHeader, prevBlock, algorithm);
    std::vector<unsigned char> hashVec(hashBytes.begin(), hashBytes.end());
    return bytesToHex(hashVec);
}

// The main approach is to parse the input hex string into a vector of bytes, verifying that the parsed size is exactly 80. Then extract bytes 4–35 (inclusive) as the previous block hash, storing them in a 32-byte array. Next, branch on the algorithm selector: if it is 1 or 2, call a placeholder hash function that simulates X16R or X16RV2 output. Since the real hash algorithms are complex and not available in standard C++, we define a deterministic placeholder that uses the header bytes and the previous block hash to produce a unique-looking 32-byte result. For example, for algorithm 1, compute a simple XOR of all header bytes with the previous block hash bytes cyclically; for algorithm 2, additionally rotate or invert some bytes to differentiate the two. Then convert the resulting 32 bytes to lowercase hex. Edge cases: empty or invalid hex (return empty string), wrong length (return empty string), invalid algorithm (return empty string). Complexity: hex parsing is O(n) where n is the hex string length (must be 160 chars for 80 bytes), hash computation is O(1) constant work (80 bytes), and hex output is O(32). Total time O(n) dominated by parsing, space O(1) beyond input/output.
