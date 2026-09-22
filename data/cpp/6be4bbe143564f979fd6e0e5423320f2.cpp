// Write a standalone C++ function `extractFormatBits(const std::vector<std::vector<bool>>& matrix)` that, given a square binary matrix representing a QR code symbol (where `true` corresponds to a black module and `false` to a white module), returns a `std::pair<int, int>` containing the two 15-bit format information bit patterns read from the QR code: the first element is the top-left format info (bits read in the order specified by the original code: columns 0–5 at row 8, then column 7 at row 8, column 8 at row 8, column 8 at row 7, then columns 5–0 at row 8), and the second element is the top-right/bottom-left pattern (read from the rightmost column upward through column 8, then from row dimension-8 to dimension-1 at column 8). The matrix dimension is guaranteed to be odd, at least 21, and congruent to 1 modulo 4 (i.e., dimension = 4k+1, k≥5). The function must not decode or validate the format; it must only extract the raw bits. If the matrix dimension is invalid (less than 21 or not 1 mod 4), throw an exception. Use the same bit-shifting convention as the reference: starting from 0, each read bit shifts the current value left by 1 and ORs the bit (0 or 1) into the least significant position.
// The core task is to replicate the bit-extraction logic from `BitMatrixParser::readFormatInformation()` without performing format decoding. The algorithm proceeds in two independent passes over the matrix. In the first pass, we collect 15 bits from the top-left format region: the original code reads bits at coordinates (i,8) for i=0..5, then (7,8), (8,8), (8,7), then (8,j) for j=5 down to 0. That yields exactly 15 bits (6 + 1 + 1 + 1 + 6 = 15). The second pass collects the 15-bit top-right/bottom-left pattern: it first reads bits at (8, j) for j = dimension-1 down to dimension-7 (7 bits), then at (i, 8) for i = dimension-8 to dimension-1 (8 bits), totaling 15 bits. For every bit, we use the same `copyBit` semantics: if the matrix cell is `true`, we shift the accumulator left by 1 and OR with 1; otherwise just shift left. The order is crucial because the bits are read most-significant-first into the integer. Edge cases: the matrix dimension must be validated; if the dimension is less than 21 or not congruent to 1 mod 4, throw a `std::invalid_argument`. Since the dimension is guaranteed valid for normal QR code sizes, the coordinate access is in-bounds. Time complexity is O(1) because we always read exactly 30 bits (15 per format info), but if we consider matrix size, it's O(1) with respect to dimension since the accessed coordinates are fixed (near corners). Space complexity is O(1) beyond the input matrix (which is passed by const reference).
#include <vector>
#include <utility>
#include <stdexcept>

// Extracts the two 15-bit format information patterns from a QR code matrix.
// Returns a pair: first = top-left format info bits, second = top-right/bottom-left format info bits.
std::pair<int, int> extractFormatBits(const std::vector<std::vector<bool>>& matrix) {
    const size_t dimension = matrix.size();
    if (dimension < 21 || (dimension % 4) != 1) {
        throw std::invalid_argument("Dimension must be 1 mod 4 and >= 21");
    }

    // Define a lambda to copy a single bit into the accumulator, following the original shift semantics.
    auto copyBit = [&matrix](size_t x, size_t y, int versionBits) -> int {
        // The original code: bitMatrix_->get(x, y) ? (versionBits << 1) | 0x1 : versionBits << 1
        return matrix[y][x] ? (versionBits << 1) | 0x1 : versionBits << 1;
    };

    // Read top-left format info bits (15 bits)
    int formatInfoBits1 = 0;
    for (int i = 0; i < 6; i++) {
        formatInfoBits1 = copyBit(i, 8, formatInfoBits1);
    }
    // Skip a bit in the timing pattern? Coordinates: (7,8), (8,8), (8,7)
    formatInfoBits1 = copyBit(7, 8, formatInfoBits1);
    formatInfoBits1 = copyBit(8, 8, formatInfoBits1);
    formatInfoBits1 = copyBit(8, 7, formatInfoBits1);
    // Then j from 5 down to 0
    for (int j = 5; j >= 0; j--) {
        formatInfoBits1 = copyBit(8, j, formatInfoBits1);
    }

    // Read the top-right/bottom-left pattern (15 bits)
    int formatInfoBits2 = 0;
    const int jMin = static_cast<int>(dimension) - 7;
    for (int j = static_cast<int>(dimension) - 1; j >= jMin; j--) {
        formatInfoBits2 = copyBit(8, j, formatInfoBits2);
    }
    for (int i = static_cast<int>(dimension) - 8; i < static_cast<int>(dimension); i++) {
        formatInfoBits2 = copyBit(i, 8, formatInfoBits2);
    }

    return std::make_pair(formatInfoBits1, formatInfoBits2);
}
#include <cassert>
#include <vector>

// Declare the solution function (or include the header where it is defined)
std::pair<int, int> extractFormatBits(const std::vector<std::vector<bool>>& matrix);

int main() {
    // Test 1: 21x21 matrix with all false, should produce zeros because no bits set.
    {
        std::vector<std::vector<bool>> m(21, std::vector<bool>(21, false));
        auto result = extractFormatBits(m);
        assert(result.first == 0);
        assert(result.second == 0);
    }

    // Test 2: 21x21 matrix with all true, the bits read in order become all 1s.
    // For 15 bits, each shifted left and ORed with 1: result is 0x7FFF (32767).
    {
        std::vector<std::vector<bool>> m(21, std::vector<bool>(21, true));
        auto result = extractFormatBits(m);
        assert(result.first == 0x7FFF);
        assert(result.second == 0x7FFF);
    }

    // Test 3: 21x21 matrix with exactly one true at (0,8) (row y=8, column x=0).
    // For formatInfoBits1, the first bit read is that one, so the value is 1 << 14 = 16384.
    // FormatInfoBits2 remains 0 because none of its coordinates are (0,8).
    {
        std::vector<std::vector<bool>> m(21, std::vector<bool>(21, false));
        m[8][0] = true;
        auto result = extractFormatBits(m);
        assert(result.first == 16384);
        assert(result.second == 0);
    }

    // Test 4: 21x21, set a bit at (8,20) (row y=20, column x=8). This is the first bit of formatInfoBits2.
    {
        std::vector<std::vector<bool>> m(21, std::vector<bool>(21, false));
        m[20][8] = true; // Note: matrix[y][x]
        auto result = extractFormatBits(m);
        assert(result.first == 0);
        assert(result.second == 16384);
    }

    // Test 5: 21x21, set a bit at (8,0) (row y=0, column x=8). This is the last bit of formatInfoBits1.
    {
        std::vector<std::vector<bool>> m(21, std::vector<bool>(21, false));
        m[0][8] = true;
        auto result = extractFormatBits(m);
        assert(result.first == 1);
        assert(result.second == 0);
    }

    // Test 6: Invalid dimension 20 should throw.
    {
        bool threw = false;
        try {
            std::vector<std::vector<bool>> m(20, std::vector<bool>(20, false));
            extractFormatBits(m);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        assert(threw);
    }

    // Test 7: Invalid dimension 22 (not 1 mod 4) should throw.
    {
        bool threw = false;
        try {
            std::vector<std::vector<bool>> m(22, std::vector<bool>(22, false));
            extractFormatBits(m);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        assert(threw);
    }

    // Test 8: A 25x25 matrix (25 = 4*6+1) with all false, expect zeros.
    {
        std::vector<std::vector<bool>> m(25, std::vector<bool>(25, false));
        auto result = extractFormatBits(m);
        assert(result.first == 0);
        assert(result.second == 0);
    }

    // Test 9: A 25x25 matrix with a specific bit at (8,24) (row 24, col 8) affects first bit of second pattern.
    {
        std::vector<std::vector<bool>> m(25, std::vector<bool>(25, false));
        m[24][8] = true;
        auto result = extractFormatBits(m);
        assert(result.first == 0);
        assert(result.second == (1 << 14)); // 16384
    }

    // Test 10: A 21x21 matrix with a bit at (13,8) (row 8, col 13) — this is not in any format region, ignored.
    {
        std::vector<std::vector<bool>> m(21, std::vector<bool>(21, false));
        m[8][13] = true;
        auto result = extractFormatBits(m);
        assert(result.first == 0);
        assert(result.second == 0);
    }

    return 0;
}
