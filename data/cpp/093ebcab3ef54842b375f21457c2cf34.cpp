// Write a C++ function that accepts two vectors of integers representing ORB descriptor bytes (each descriptor is exactly 32 bytes, stored as 8 integers since each `int` packs 4 bytes), and returns the Hamming distance between them, computed by XOR-ing corresponding integers and counting the set bits using the bit-counting technique shown in the snippet. The function must return an `int` and handle any input vectors that are exactly 8 elements long; if the vectors are not equal in size or not 8 elements, return -1 as an error indicator. The function should be named `orbDescriptorDistance` and be `const`-correct, taking the vectors by `const std::vector<int>&`.
The core algorithm mirrors the `DescriptorDistance` from the snippet, which processes 8 packed 32-bit integers. For each pair of corresponding integers from the two vectors, we compute `v = a ^ b`. Then we count set bits in `v` using the standard parallel bit-count method: 
1. `v = v - ((v >> 1) & 0x55555555)` subtracts pairs of bits.
2. `v = (v & 0x33333333) + ((v >> 2) & 0x33333333)` sums adjacent pairs into nibbles.
3. `dist += (((v + (v >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24` accumulates the total count.

We repeat this for all 8 integers. Edge cases: if either vector has a size other than 8, or sizes differ, return -1 to signal invalid input. We assume the integers are non-negative (as descriptors are), but the bit count works for any bit pattern. The time complexity is O(1) since we always process exactly 8 integers; space complexity is O(1) auxiliary. The bit-counting method is branchless and efficient, using only a few arithmetic operations per integer.
#include <vector>
#include <cstdint>

// Compute Hamming distance between two 32-byte ORB descriptors.
// Each descriptor is represented as 8 ints (each int packs 4 bytes).
// Returns -1 if input is invalid (size != 8 or sizes differ).
int orbDescriptorDistance(const std::vector<int>& a, const std::vector<int>& b) {
    // Validate input: both must be exactly 8 integers (32 bytes).
    if (a.size() != 8 || b.size() != 8) {
        return -1;
    }

    int dist = 0;
    const int* pa = a.data();
    const int* pb = b.data();

    for (int i = 0; i < 8; ++i, ++pa, ++pb) {
        unsigned int v = static_cast<unsigned int>(*pa) ^ static_cast<unsigned int>(*pb);
        // Parallel bit count (from Stanford bithacks).
        v = v - ((v >> 1) & 0x55555555);
        v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
        dist += static_cast<int>((((v + (v >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24);
    }

    return dist;
}
#include <cassert>
#include <vector>

// Declaration of the function under test (already defined above).
int orbDescriptorDistance(const std::vector<int>& a, const std::vector<int>& b);

int main() {
    // Identical descriptors: distance 0.
    std::vector<int> d1 = {0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> d2 = {0, 0, 0, 0, 0, 0, 0, 0};
    assert(orbDescriptorDistance(d1, d2) == 0);

    // All bits differ in each int: each int has 32 bits set, total 8*32=256.
    std::vector<int> d3 = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
    std::vector<int> d4 = {0, 0, 0, 0, 0, 0, 0, 0};
    assert(orbDescriptorDistance(d3, d4) == 256);

    // Single bit difference: 0x01 vs 0x00 gives 1, other ints same.
    std::vector<int> d5 = {0x01, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> d6 = {0x00, 0, 0, 0, 0, 0, 0, 0};
    assert(orbDescriptorDistance(d5, d6) == 1);

    // Multiple bits in one int and one bit in another: 0x0F has 4 bits, 0x80 has 1.
    std::vector<int> d7 = {0x0F, 0x80, 0, 0, 0, 0, 0, 0};
    std::vector<int> d8 = {0x00, 0x00, 0, 0, 0, 0, 0, 0};
    assert(orbDescriptorDistance(d7, d8) == 5);

    // Identical non-zero pattern: distance 0.
    std::vector<int> d9 = {0xAAAA5555, 0x12345678, 0xFFFFFFFF, 0x00000001, 0x80000000, 0x0F0F0F0F, 0x77777777, 0x01010101};
    std::vector<int> d10 = d9; // copy
    assert(orbDescriptorDistance(d9, d10) == 0);

    // Difference in the last int: 0x00 vs 0xFF -> 8 bits.
    std::vector<int> d11 = {0, 0, 0, 0, 0, 0, 0, 0x00};
    std::vector<int> d12 = {0, 0, 0, 0, 0, 0, 0, 0xFF};
    assert(orbDescriptorDistance(d11, d12) == 8);

    // Invalid: wrong size.
    std::vector<int> short_vec = {1, 2, 3};
    std::vector<int> valid_vec = {0, 0, 0, 0, 0, 0, 0, 0};
    assert(orbDescriptorDistance(short_vec, valid_vec) == -1);
    assert(orbDescriptorDistance(valid_vec, short_vec) == -1);

    // Invalid: mismatched sizes (both wrong but different).
    std::vector<int> vec7 = {1,2,3,4,5,6,7};
    std::vector<int> vec9 = {1,2,3,4,5,6,7,8,9};
    assert(orbDescriptorDistance(vec7, vec9) == -1);

    return 0;
}
