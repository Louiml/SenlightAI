Implement a C++ function named `murmurHash2` that takes a `const void*` key, an `int` length in bytes, and an `unsigned int` seed, and returns a 32-bit unsigned integer hash value. The function must exactly replicate the MurmurHash2 algorithm from the provided snippet, including the same mixing constants (`0x5bd1e995` and `24`), the same byte-order-dependent handling of 4-byte chunks (reading raw memory as `unsigned int`), the same tail-byte switch with fallthrough behavior, and the same final avalanche mixing steps. The function must be safe for any input length (including zero) and must not modify the input data. Use `const` correctly and include only the function definition with necessary headers; no `main` function.
#include <cassert>
#include <cstring>

// Declare the function (typically from a header, but here for testing)
unsigned int murmurHash2(const void* key, int len, unsigned int seed);

int main() {
    // Known hash values computed using the reference implementation
    // (example values from original MurmurHash2 test vectors)
    const char* empty = "";
    assert(murmurHash2(empty, 0, 0) == 0);

    const char* one = "a";
    assert(murmurHash2(one, 1, 0) == 1002485773u);

    const char* hello = "Hello, world!";
    assert(murmurHash2(hello, 13, 0) == 3728699735u);

    const char* repeated = "Test";
    assert(murmurHash2(repeated, 4, 12345) == 3460944938u);

    // Test aligned and unaligned pointers with identical content
    char buffer[16];
    strcpy(buffer, "abc");
    assert(murmurHash2(buffer, 3, 42) == murmurHash2(buffer + 1, 2, 42) ? false : true); // Different lengths, just ensure no crash
    // Better: identical content via copy
    char copy[8];
    strcpy(copy, "abc");
    assert(murmurHash2(buffer, 3, 42) == murmurHash2(copy, 3, 42));

    // Test length exactly 4 and 5
    const char* four = "abcd";
    assert(murmurHash2(four, 4, 7) == 3800951035u);
    const char* five = "abcde";
    assert(murmurHash2(five, 5, 7) == 260817430u);

    // Test zero seed vs nonzero seed
    const char* data = "seed";
    assert(murmurHash2(data, 4, 0) != murmurHash2(data, 4, 1));

    return 0;
}
#include <cstddef>

// MurmurHash2, by Austin Appleby. Returns a 32-bit hash of the key.
unsigned int murmurHash2(const void* key, int len, unsigned int seed) {
    const unsigned int m = 0x5bd1e995;
    const int r = 24;

    unsigned int h = seed ^ static_cast<unsigned int>(len);

    const unsigned char* data = static_cast<const unsigned char*>(key);

    while (len >= 4) {
        unsigned int k;
        // Read 4 bytes as a single unsigned int (endianness-dependent, matches reference)
        __builtin_memcpy(&k, data, sizeof(k));
        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    switch (len) {
        case 3:
            h ^= static_cast<unsigned int>(data[2]) << 16;
            // fall through
        case 2:
            h ^= static_cast<unsigned int>(data[1]) << 8;
            // fall through
        case 1:
            h ^= static_cast<unsigned int>(data[0]);
            h *= m;
            break;
        default:
            break;
    }

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}
// The MurmurHash2 algorithm processes the input in 4-byte blocks for efficiency, then handles any remaining 1–3 bytes individually. For each full 4-byte chunk, it reads the raw memory as an `unsigned int` (which is platform-dependent in endianness, but must match the reference behavior—typically little-endian on common systems), then applies: multiply by `m`, XOR with right-shifted 24 bits, multiply by `m` again. The hash state `h` is multiplied by `m` and XORed with the processed chunk. After the loop, the tail bytes are incorporated using a switch statement with intentional fallthrough: for 3 bytes, XOR with `data[2] << 16`; for 2, XOR with `data[1] << 8`; for 1, XOR with `data[0]`, and then multiply `h` by `m`. Finally, three avalanche operations are applied: `h ^= h >> 13`, `h *= m`, `h ^= h >> 15`. Edge cases: zero-length input returns `seed ^ 0` then the final mixes; lengths not divisible by 4 correctly handle the tail; the function must not dereference beyond the provided length. Time complexity is O(n) where n is the byte length, and space complexity is O(1) using only a few local variables.
