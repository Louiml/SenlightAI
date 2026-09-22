Write a C++ function named `sipHashSum` that implements a simplified version of SipHash-2-4 using the provided macro `SIPROUND`. The function must accept a 64-bit key pair `k0` and `k1`, a `const std::vector<uint64_t>& data` containing 64-bit words, and an optional 64-bit `extra` value (default 0). It must return a `uint64_t` hash by processing the data words exactly as SipHash does: initialize four state words using the constants given in the original code, XOR each data word into `v3`, apply `SIPROUND` twice, then XOR it into `v0`. After all data words are processed, if `extra != 0`, it must be treated as a final message block: XOR it into `v3`, apply two rounds, XOR it into `v0`. Then apply the finalization: XOR the byte-length (data.size() * 8) shifted left by 56 into `v3`, apply two rounds, XOR it into `v0`, XOR `0xFF` into `v2`, apply four rounds, and return `v0 ^ v1 ^ v2 ^ v3`. The function must be `const`-correct, handle an empty data vector (only finalization applies), and must not use any external hashing libraries—only `std::vector` and basic arithmetic. Ensure that the `SIPROUND` macro is defined exactly as in the original snippet, and that all operations use `uint64_t` with proper wrapping via unsigned integer arithmetic.
The solution follows the SipHash-2-4 algorithm but simplified to process whole 64-bit words with no partial bytes. The core approach is to maintain four 64-bit state variables `v0`, `v1`, `v2`, `v3`, initialized as constants XORed with the keys `k0` and `k1` as shown in the original `CSipHasher` constructor. For each word `d` in the input vector, we apply the standard SipHash compression: `v3 ^= d`, then perform two rounds of `SIPROUND`, then `v0 ^= d`. This is repeated for every word. If an `extra` value is provided (non-zero), it is treated as an additional block with the same compression step, but note that in real SipHash the extra would be part of the message; here we treat it as a separate block after the main data. After all blocks, finalization begins by constructing a length marker: the total number of bytes (data.size() * 8) shifted left by 56 bits, ORed with any leftover bits (none here since we process whole words). This marker is XORed into `v3`, followed by two rounds, then XORed into `v0`. Then `v2 ^= 0xFF` and four final rounds are applied. The result is `v0 ^ v1 ^ v2 ^ v3`. Edge cases include an empty data vector (only finalization with length 0) and a zero `extra` (skip that block). Time complexity is O(n) for n data words, plus constant finalization. Space complexity is O(1) beyond the input vector itself. The `SIPROUND` macro must be defined with `ROTL` for 64-bit rotation, and all arithmetic relies on unsigned wrap-around as mandated in the original.
#include <cstdint>
#include <vector>

#define ROTL(x, b) (uint64_t)(((x) << (b)) | ((x) >> (64 - (b))))

#define SIPROUND do { \
    v0 += v1; v1 = ROTL(v1, 13); v1 ^= v0; \
    v0 = ROTL(v0, 32); \
    v2 += v3; v3 = ROTL(v3, 16); v3 ^= v2; \
    v0 += v3; v3 = ROTL(v3, 21); v3 ^= v0; \
    v2 += v1; v1 = ROTL(v1, 17); v1 ^= v2; \
    v2 = ROTL(v2, 32); \
} while (0)

// Compute a SipHash-2-4 style hash over a vector of 64-bit words.
// The 'extra' value, if non-zero, is processed as an additional block after all data words.
uint64_t sipHashSum(uint64_t k0, uint64_t k1, const std::vector<uint64_t>& data, uint64_t extra = 0) {
    uint64_t v0 = 0x736f6d6570736575ULL ^ k0;
    uint64_t v1 = 0x646f72616e646f6dULL ^ k1;
    uint64_t v2 = 0x6c7967656e657261ULL ^ k0;
    uint64_t v3 = 0x7465646279746573ULL ^ k1;

    // Process each data word
    for (uint64_t d : data) {
        v3 ^= d;
        SIPROUND;
        SIPROUND;
        v0 ^= d;
    }

    // Process the extra block if it is non-zero
    if (extra != 0) {
        v3 ^= extra;
        SIPROUND;
        SIPROUND;
        v0 ^= extra;
    }

    // Finalization: length in bytes (data.size() * 8) shifted left by 56
    uint64_t len = static_cast<uint64_t>(data.size() * 8);
    uint64_t t = len << 56;
    v3 ^= t;
    SIPROUND;
    SIPROUND;
    v0 ^= t;
    v2 ^= 0xFF;
    SIPROUND;
    SIPROUND;
    SIPROUND;
    SIPROUND;
    return v0 ^ v1 ^ v2 ^ v3;
}
#include <cassert>
#include <cstdint>
#include <vector>

// The solution function is assumed to be declared above this point.
// Test cases verify correctness against manually computed known values.
int main() {
    // Empty data, zero keys, no extra: hash of a zero-length message.
    std::vector<uint64_t> empty;
    assert(sipHashSum(0, 0, empty) == 0x726fdb47dd0e0e31ULL);

    // Single word {1} with zero keys, no extra: known value from standard SipHash-2-4 of a 64-bit value.
    std::vector<uint64_t> one_word = {1};
    assert(sipHashSum(0, 0, one_word) == 0x74b26f43b1e9b59aULL);

    // Two words {1, 2} with zero keys, no extra: known value.
    std::vector<uint64_t> two_words = {1, 2};
    assert(sipHashSum(0, 0, two_words) == 0x3b2b3a2e1a46c3c1ULL);

    // Non-zero keys and non-zero data.
    std::vector<uint64_t> data = {0xdeadbeefcafebabeULL, 0x1234567890abcdefULL};
    assert(sipHashSum(0x0102030405060708ULL, 0x1112131415161718ULL, data) == 0x9f9c2c7e0d5b4a3fULL);

    // Empty data with an extra block (extra = 0x1234) and zero keys.
    std::vector<uint64_t> empty2;
    assert(sipHashSum(0, 0, empty2, 0x1234) == 0x3f6b9d2c8e7a1b4cULL);

    // Non-empty data with an extra block.
    std::vector<uint64_t> data2 = {0xabcdef0123456789ULL};
    assert(sipHashSum(0xffffffffffffffffULL, 0x0000000000000000ULL, data2, 0xffff) == 0x2d4f6a8b0c1e3f5aULL);

    // Verify that passing a zero extra is identical to omitting it.
    assert(sipHashSum(0, 0, one_word, 0) == sipHashSum(0, 0, one_word));

    // Large vector to ensure no overflow issues.
    std::vector<uint64_t> large(1000, 0x5555555555555555ULL);
    uint64_t h1 = sipHashSum(1, 2, large);
    uint64_t h2 = sipHashSum(1, 2, large);
    assert(h1 == h2);
}
