Write a C++ function `void radix_sort(int* begin, int* end)` that sorts an array of 32-bit signed integers in-place using a two-pass radix sort with a base of 65536 (16 bits per pass). The function must handle negative numbers correctly using two's complement representation, so the sort must produce ascending order including negatives. The array is given by `[begin, end)` pointers; you may use a temporary buffer and a bucket array of size 65536, both allocated dynamically or statically, but must not use any standard sorting functions. The function must be stable (preserve relative order of equal elements) and work for any number of elements from 0 to 10,000,000. Demonstrate correctness with test cases including large arrays with duplicates and mixed negatives/positives.
A stable radix sort processes digits from least significant to most significant. Here, each element is a 32-bit integer, and with base 65536 (2^16), we need exactly two passes: lower 16 bits first, then upper 16 bits. The key trick is that for signed integers, the most significant bit (bit 31) indicates sign; in two's complement, negative numbers have the top bit set, so after sorting by the upper 16 bits, negatives (with high bits from 0x8000 to 0xFFFF) would appear after positives (0x0000 to 0x7FFF) if we just treat the bits as unsigned. To fix this, we can flip the most significant bit before the second pass (or equivalently, XOR the upper 16-bit key with 0x8000 for sorting purposes), making the key order correct for signed integers. Alternatively, we can sort by the upper 16 bits using the transformed key `((unsigned)(value) ^ 0x80000000) >> 16` which maps negatives to lower keys. Implement count sort per digit: count frequencies, compute prefix sums, then place elements from end to beginning into a temporary array to maintain stability, then copy back. Reset bucket array each pass. Edge cases: input size 0 or 1 (no work needed), all identical elements (stable), large arrays. Time complexity: O(2n) counts + O(2n) placements = O(n). Space: O(BASE + n) = O(65536 + n) which is O(n) for large n, but constant base.
#include <cstring>
#include <cstddef>

// Stable radix sort for 32-bit signed integers using base 65536 (two passes)
void radix_sort(int* begin, int* end) {
    const size_t n = static_cast<size_t>(end - begin);
    constexpr int BASE = 1 << 16;
    constexpr int BASE_MASK = BASE - 1;
    
    if (n < 2) return;
    
    int* tmp = new int[n];
    int* bkt = new int[BASE]();
    
    // First pass: sort by lower 16 bits (least significant)
    for (size_t i = 0; i < n; ++i) {
        int key = (unsigned)begin[i] & BASE_MASK;
        ++bkt[key];
    }
    for (int i = 1; i < BASE; ++i) {
        bkt[i] += bkt[i - 1];
    }
    for (size_t i = n; i-- > 0;) {
        int key = (unsigned)begin[i] & BASE_MASK;
        tmp[--bkt[key]] = begin[i];
    }
    std::memcpy(begin, tmp, n * sizeof(int));
    std::memset(bkt, 0, BASE * sizeof(int));
    
    // Second pass: sort by upper 16 bits, but flip the sign bit for correct order
    for (size_t i = 0; i < n; ++i) {
        int key = ((unsigned)begin[i] >> 16) ^ 0x8000;
        ++bkt[key];
    }
    for (int i = 1; i < BASE; ++i) {
        bkt[i] += bkt[i - 1];
    }
    for (size_t i = n; i-- > 0;) {
        int key = ((unsigned)begin[i] >> 16) ^ 0x8000;
        tmp[--bkt[key]] = begin[i];
    }
    std::memcpy(begin, tmp, n * sizeof(int));
    
    delete[] tmp;
    delete[] bkt;
}
#include <cassert>
#include <cstdlib>
#include <algorithm>

void test_radix_sort() {
    // Basic small array with mixed signs
    int a1[] = {3, -1, 2, -5, 0};
    radix_sort(a1, a1 + 5);
    int expected1[] = {-5, -1, 0, 2, 3};
    for (int i = 0; i < 5; ++i) assert(a1[i] == expected1[i]);

    // Duplicates and extremes
    int a2[] = {INT_MIN, INT_MAX, 0, -1, 1, INT_MIN, INT_MAX};
    radix_sort(a2, a2 + 7);
    int expected2[] = {INT_MIN, INT_MIN, -1, 0, 1, INT_MAX, INT_MAX};
    for (int i = 0; i < 7; ++i) assert(a2[i] == expected2[i]);

    // Single element
    int a3[] = {42};
    radix_sort(a3, a3 + 1);
    assert(a3[0] == 42);

    // Empty range (should not crash)
    int* p = nullptr;
    radix_sort(p, p);

    // Random large test with negatives and positives
    const int N = 100000;
    int* a4 = new int[N];
    for (int i = 0; i < N; ++i) {
        a4[i] = rand() - RAND_MAX / 2;
    }
    radix_sort(a4, a4 + N);
    for (int i = 1; i < N; ++i) {
        assert(a4[i - 1] <= a4[i]);
    }
    delete[] a4;

    // All same values
    int a5[] = {7, 7, 7, 7};
    radix_sort(a5, a5 + 4);
    for (int i = 0; i < 4; ++i) assert(a5[i] == 7);
}

int main() {
    test_radix_sort();
    return 0;
}
