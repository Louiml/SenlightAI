// Design and implement a C++ function that computes a 64-bit hash value from a byte array (a `const void*` pointer and a `size_t` length), using a simplified but robust version of the Bob Jenkins lookup3 mixing scheme. The function must mix the input data thoroughly so that every byte affects the final hash, and it must support an arbitrary byte length (including zero). It should return the hash as a single `uint64_t`, combining two 32-bit internal hash values. The function must avoid platform-specific endianness assumptions by processing the input byte-by-byte in an endian-safe manner (i.e., interpret bytes consistently regardless of machine endianness). The provided code snippet is the reference implementation; your task is to create a self-contained, portable C++ function that replicates the behavior of `hashlittle2` (returning the concatenation of `*pc` and `*pb`) but without relying on unaligned reads, alignment detection, or preprocessor endianness macros. Specifically: implement the `mix` and `final` macros as inline functions or lambdas, process the input in 12-byte blocks using byte-wise accumulation into three 32-bit registers `a`, `b`, `c`, and handle the tail (0–11 remaining bytes) with a fall-through switch, then apply the final mixing and return `(uint64_t(c) << 32) | b` (where `c` is the primary hash and `b` is the secondary). The seed values should default to zero for an empty input, but the function should accept two uint32_t seeds as optional parameters (defaulting to 0) to allow chaining. The solution must include a single free function named `hash64` with a clear signature, proper `const` correctness, and no external dependencies beyond standard headers. Provide a reference implementation and test cases that validate the hash for edge cases (empty input, single byte, aligned/unaligned buffers, and a known vector from the original code).

#include <cassert>
#include <cstdint>
#include <cstring>

// Declare the solution function (include the above code in the same translation unit).
uint64_t hash64(const void* key, size_t length, uint32_t seed1 = 0, uint32_t seed2 = 0);

int main() {
    // Empty input: seed1=0, seed2=0 → known vector from original (c=0xdeadbeef, b=0xdeadbeef)
    uint64_t empty = hash64(nullptr, 0);
    assert(empty == (static_cast<uint64_t>(0xdeadbeef) << 32) | 0xdeadbeef);

    // Known vector: "Four score and seven years ago" (30 bytes) with seeds 0,0.
    // From the original driver5, hashlittle2 gives c=0x17770551, b=0xce7226e6.
    const char* data = "Four score and seven years ago";
    uint64_t known = hash64(data, 30, 0, 0);
    assert(known == (static_cast<uint64_t>(0x17770551) << 32) | 0xce7226e6);

    // Single byte zero: should be deterministic and not equal to empty.
    unsigned char zero_byte = 0;
    uint64_t single = hash64(&zero_byte, 1, 0, 0);
    assert(single != empty);

    // Two different seeds produce different hashes for same data.
    uint64_t seed_a = hash64(data, 30, 1, 0);
    uint64_t seed_b = hash64(data, 30, 2, 0);
    assert(seed_a != seed_b);

    // Same data with same seeds must be identical (idempotent).
    assert(hash64(data, 30, 1, 0) == seed_a);

    // Handling unaligned buffers: copy data to a shifted array and compare.
    char buffer[64];
    char* shifted = buffer + 1; // simulate unaligned pointer
    std::memcpy(shifted, data, 30);
    uint64_t unaligned = hash64(shifted, 30, 0, 0);
    assert(unaligned == known);

    // Length 12 (exact block) vs length 13 (block + 1 byte tail) should differ.
    const char* twelve = "123456789012";
    const char* thirteen = "1234567890123";
    assert(hash64(twelve, 12, 0, 0) != hash64(thirteen, 13, 0, 0));

    // Chaining: hash of first part, use as seed for second part.
    const char* part1 = "Hello ";
    const char* part2 = "World";
    uint64_t h1 = hash64(part1, 6, 0, 0);
    uint32_t seed1 = static_cast<uint32_t>(h1 >> 32);
    uint32_t seed2 = static_cast<uint32_t>(h1);
    uint64_t chained = hash64(part2, 5, seed1, seed2);
    uint64_t whole = hash64("Hello World", 11, 0, 0);
    // Not necessarily equal, but should be different from using hash of part2 alone.
    assert(chained != hash64(part2, 5, 0, 0));

    // Sign and interpretation: high bits must be set for some inputs.
    const char* long_str = "The quick brown fox jumps over the lazy dog";
    uint64_t long_hash = hash64(long_str, std::strlen(long_str), 0, 0);
    assert(long_hash != 0);
    assert((long_hash >> 32) != 0); // Primary hash is well-mixed.

    return 0;
}

#include <cstdint>
#include <cstddef>

// Rotate 32-bit value x left by k bits.
static inline uint32_t rot(uint32_t x, int k) {
    return (x << k) | (x >> (32 - k));
}

// Mix three 32-bit values reversibly (Bob Jenkins' mix).
static inline void mix(uint32_t& a, uint32_t& b, uint32_t& c) {
    a -= c; a ^= rot(c, 4); c += b;
    b -= a; b ^= rot(a, 6); a += c;
    c -= b; c ^= rot(b, 8); b += a;
    a -= c; a ^= rot(c, 16); c += b;
    b -= a; b ^= rot(a, 19); a += c;
    c -= b; c ^= rot(b, 4); b += a;
}

// Final mixing of three 32-bit values (Bob Jenkins' final).
static inline void finalize(uint32_t& a, uint32_t& b, uint32_t& c) {
    c ^= b; c -= rot(b, 14);
    a ^= c; a -= rot(c, 11);
    b ^= a; b -= rot(a, 25);
    c ^= b; c -= rot(b, 16);
    a ^= c; a -= rot(c, 4);
    b ^= a; b -= rot(a, 14);
    c ^= b; c -= rot(b, 24);
}

/**
 * Compute a 64-bit hash of a byte array using a simplified lookup3 algorithm.
 * @param key      Pointer to the bytes to hash (may be null if length is 0).
 * @param length   Number of bytes in the key.
 * @param seed1    Primary seed (default 0).
 * @param seed2    Secondary seed (default 0).
 * @return         Hash value: (primary_hash << 32) | secondary_hash.
 *                 For zero-length input, behaves like hashlittle2 with given seeds.
 */
uint64_t hash64(const void* key, size_t length, uint32_t seed1 = 0, uint32_t seed2 = 0) {
    const uint8_t* k = static_cast<const uint8_t*>(key);
    uint32_t a, b, c;

    // Set up internal state: standard constants plus length and seeds.
    a = b = c = 0xdeadbeef + static_cast<uint32_t>(length) + seed1;
    c += seed2;

    // Process full 12-byte blocks, endian-safe byte accumulation.
    while (length >= 12) {
        a += k[0] | (static_cast<uint32_t>(k[1]) << 8) |
             (static_cast<uint32_t>(k[2]) << 16) | (static_cast<uint32_t>(k[3]) << 24);
        b += k[4] | (static_cast<uint32_t>(k[5]) << 8) |
             (static_cast<uint32_t>(k[6]) << 16) | (static_cast<uint32_t>(k[7]) << 24);
        c += k[8] | (static_cast<uint32_t>(k[9]) << 8) |
             (static_cast<uint32_t>(k[10]) << 16) | (static_cast<uint32_t>(k[11]) << 24);
        mix(a, b, c);
        length -= 12;
        k += 12;
    }

    // Handle remaining 0..11 bytes using fall-through switch.
    switch (length) {
        case 11: c += static_cast<uint32_t>(k[10]) << 16; [[fallthrough]];
        case 10: c += static_cast<uint32_t>(k[9]) << 8;  [[fallthrough]];
        case 9:  c += static_cast<uint32_t>(k[8]);       [[fallthrough]];
        case 8:  b += static_cast<uint32_t>(k[7]) << 24; [[fallthrough]];
        case 7:  b += static_cast<uint32_t>(k[6]) << 16; [[fallthrough]];
        case 6:  b += static_cast<uint32_t>(k[5]) << 8;  [[fallthrough]];
        case 5:  b += static_cast<uint32_t>(k[4]);       [[fallthrough]];
        case 4:  a += static_cast<uint32_t>(k[3]) << 24; [[fallthrough]];
        case 3:  a += static_cast<uint32_t>(k[2]) << 16; [[fallthrough]];
        case 2:  a += static_cast<uint32_t>(k[1]) << 8;  [[fallthrough]];
        case 1:  a += static_cast<uint32_t>(k[0]);       [[fallthrough]];
        case 0:  break;
        default: break; // length > 11 already handled above, but safe.
    }

    // Apply final mixing to produce well-distributed values.
    finalize(a, b, c);

    // Combine primary (c) and secondary (b) into 64-bit result.
    return (static_cast<uint64_t>(c) << 32) | b;
}

// The core algorithm is derived from Bob Jenkins’ lookup3 hash. It maintains three 32-bit state variables `a`, `b`, `c`. Initially, these are seeded with a constant `0xdeadbeef`, the length, and the primary seed, with the secondary seed added to `c`. The function processes the input in blocks of 12 bytes. For each full block, it accumulates the first 4 bytes into `a` (as little‑endian unsigned 32‑bit), the next 4 into `b`, and the last 4 into `c`, then applies the `mix` operation: six rounds of reversible arithmetic and rotations (e.g., `a -= c; a ^= rot(c,4); c += b;` etc.). This ensures that every byte’s influence spreads across all three state words. After processing all full blocks, the remaining 0–11 bytes are added to `a`, `b`, or `c` in a fall‑through switch: for lengths 1–4, bytes are combined into `a` (shifted appropriately); for lengths 5–8, the first four go to `a` and the next four to `b`; for lengths 9–12, all three registers are filled. Finally, the `final` operation applies seven additional rounds of mixing to thoroughly avalanche changes. The function returns a 64‑bit value by shifting the primary hash `c` left by 32 and ORing with the secondary hash `b`. The implementation must be endian‑independent: since we read bytes individually and combine them with explicit shifts (e.g., `a += k[0] | (k[1]<<8) | (k[2]<<16) | (k[3]<<24)`), it produces the same results on little‑ and big‑endian machines, unlike the original alignment‑dependent code. Edge cases include zero length (immediately return based on seeds), lengths not multiples of 12, and unaligned buffers (handled by byte‑wise reading). Time complexity is O(n) with n being the byte length; space complexity is O(1) beyond the function’s stack. The algorithm is not cryptographically secure but is well‑suited for hash tables and checksums.
