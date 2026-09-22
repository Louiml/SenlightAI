// Create a standalone C++ function named `computeAdler32` that implements the Adler-32 checksum algorithm exactly as shown in the provided code snippet. The function must take a pointer to an array of unsigned characters (the data buffer), an integer specifying the number of bytes in the buffer, and an integer parameter `features` that is ignored (for API compatibility). The function must return an unsigned integer containing the Adler-32 checksum, with the lower 16 bits holding `sum1` and the upper 16 bits holding `sum2` after the final modulo operation. The implementation must follow the same logic: use the modulo constant `BASE = 65521`, process data in blocks of `NMAX = 5552` bytes for efficiency, then handle remaining data in chunks of 16 bytes and then byte-by-byte. Handle the edge case of an empty buffer (n = 0) correctly, returning the checksum of an empty input (which is 1, since sum1 starts at 1 and sum2 starts at 0, and after modulo both remain as is). Ensure the function is self-contained, const-correct, and does not rely on any external code apart from standard C++ headers.
// The Adler-32 algorithm maintains two unsigned 16-bit accumulators, `sum1` and `sum2`. `sum1` initially equals 1, and `sum2` initially equals 0. For each byte `b` in the input, `sum1` is incremented by `b` (modulo 65521), and `sum2` is incremented by the new `sum1` (modulo 65521). At the end, the checksum is `(sum2 << 16) | sum1`. The efficient implementation reduces the frequency of modulo operations by accumulating sums over blocks of at most `NMAX = 5552` bytes, because 5552 * 255 = 1,415,760, which is less than the maximum value before overflow when multiplied by 16 (the largest possible `sum1` after 5552 bytes is 5552*255 + 1 = 1,415,761, and adding that to `sum2` over 5552 iterations would overflow 32-bit unsigned integer if we didn't modulo periodically). The algorithm processes `NMAX` bytes in blocks, applying modulo to both `sum1` and `sum2` after each block. Then it processes any remaining bytes in chunks of 16 (using macros that expand to sequential additions), and finally byte-by-byte. Edge cases: empty input (n=0) returns 1 because the loops are skipped and the final mod leaves sum1=1, sum2=0, so the result is 1. The `features` parameter is unused, so it is simply ignored. Time complexity is O(n) where n is the number of bytes, with constant space usage O(1). The implementation uses macros to unroll the inner loops for performance, matching the original snippet.
#include <cstdint>
#include <cstddef>

// Adler-32 checksum computation. Returns checksum as (sum2 << 16) | sum1.
// The 'features' parameter is ignored; it exists for API compatibility.
unsigned int computeAdler32(const unsigned char* data, int length, int /*features*/) {
    const unsigned int BASE = 65521u;  // Largest prime less than 2^16
    const int NMAX = 5552;             // Max bytes before needing modulo to avoid overflow

    unsigned int sum1 = 1;
    unsigned int sum2 = 0;

    // Process in blocks of NMAX bytes for efficiency
    while (length >= NMAX) {
        // Process 16 bytes at a time in this block
        for (int i = 0; i < NMAX / 16; ++i) {
            // Unroll 16-byte processing
            for (int j = 0; j < 16; ++j) {
                sum1 += data[j];
                sum2 += sum1;
            }
            data += 16;
        }
        // Reduce modulo after each block
        sum1 %= BASE;
        sum2 %= BASE;
        length -= NMAX;
    }

    // Process remaining full 16-byte chunks
    while (length >= 16) {
        for (int j = 0; j < 16; ++j) {
            sum1 += data[j];
            sum2 += sum1;
        }
        data += 16;
        length -= 16;
    }

    // Process remaining bytes one by one
    while (length > 0) {
        sum1 += data[0];
        sum2 += sum1;
        data += 1;
        length -= 1;
    }

    // Final modulo reduction
    sum1 %= BASE;
    sum2 %= BASE;

    return sum1 | (sum2 << 16);
}
#include <cassert>
#include <vector>

int main() {
    // Test empty input
    assert(computeAdler32(nullptr, 0, 0) == 1u);

    // Test single byte
    unsigned char single = 'A'; // 65
    // Expected: sum1 = 1+65=66, sum2 = 0+66=66, result = (66<<16)|66 = 0x00420042
    assert(computeAdler32(&single, 1, 0) == 0x00420042u);

    // Test a short string "abc"
    unsigned char abc[] = {'a', 'b', 'c'}; // 97, 98, 99
    // sum1: 1+97=98, +98=196, +99=295
    // sum2: 98, +196=294, +295=589
    // result = (589<<16)|295 = 0x024D0127 (since 589=0x24D, 295=0x127)
    assert(computeAdler32(abc, 3, 0) == 0x024D0127u);

    // Test a longer buffer to exercise the NMAX block path
    std::vector<unsigned char> large(NMAX + 10, 0xFF); // 255 each
    // We'll compare against a simple direct implementation for correctness
    unsigned int s1 = 1, s2 = 0;
    for (size_t i = 0; i < large.size(); ++i) {
        s1 = (s1 + large[i]) % BASE;
        s2 = (s2 + s1) % BASE;
    }
    unsigned int expected = s1 | (s2 << 16);
    assert(computeAdler32(large.data(), static_cast<int>(large.size()), 1) == expected);

    // Test with data of length exactly NMAX
    std::vector<unsigned char> block(NMAX, 0x01); // 1 each
    s1 = 1; s2 = 0;
    for (size_t i = 0; i < block.size(); ++i) {
        s1 = (s1 + block[i]) % BASE;
        s2 = (s2 + s1) % BASE;
    }
    expected = s1 | (s2 << 16);
    assert(computeAdler32(block.data(), NMAX, 0) == expected);

    // Test with all zeros (length 5)
    unsigned char zeros[5] = {0, 0, 0, 0, 0};
    // sum1 = 1 (since adding 0s), sum2 = 1 (since 0+1 per iteration? Let's compute: start s1=1,s2=0
    // After first 0: s1=1, s2=1; second: s1=1, s2=2; third: s1=1, s2=3; fourth: s1=1, s2=4; fifth: s1=1, s2=5
    // result = (5<<16)|1 = 0x00050001
    assert(computeAdler32(zeros, 5, 0) == 0x00050001u);

    return 0;
}
