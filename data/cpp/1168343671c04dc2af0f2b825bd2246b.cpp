// Write a C++ function `long long sumBitDifferences(const std::vector<int>& arr)` that, given a non-empty vector of 32-bit signed integers, returns the sum of absolute differences between the number of set bits (population count) at each bit position across all unordered pairs of distinct indices. More precisely, for every pair `(i, j)` with `i < j`, compute the Hamming distance between the binary representations of `arr[i]` and `arr[j]` (i.e., the number of bit positions where the two numbers differ). Sum these distances over all such pairs and return the total. The result may be large, so use a 64-bit integer return type. The function must handle negative numbers (which in two's complement have infinite leading 1s, but here we consider only the low 32 bits as per the usual bitwise representation) and duplicate values (which contribute zero difference). The input vector is guaranteed to contain at least one element.
#include <cassert>
#include <vector>
#include <cstdint>

// The solution function is declared above; this is the test harness.
long long sumBitDifferences(const std::vector<int>& arr); // forward declaration

int main() {
    // Single element: no pairs, result 0.
    assert(sumBitDifferences({42}) == 0);

    // Two identical numbers: XOR is 0, distance 0.
    assert(sumBitDifferences({7, 7}) == 0);

    // Classic example: 10 (1010) and 5 (0101) differ in all 4 bits → distance 4.
    assert(sumBitDifferences({10, 5}) == 4);

    // Three numbers: check pairwise distances.
    // 1 (0001), 2 (0010), 3 (0011)
    // (1,2): diff=3 (0011) → 2 bits
    // (1,3): diff=2 (0010) → 1 bit
    // (2,3): diff=1 (0001) → 1 bit
    // Total = 4.
    assert(sumBitDifferences({1, 2, 3}) == 4);

    // Negative numbers: -1 (32 ones) and 0 (32 zeros) → distance 32.
    assert(sumBitDifferences({-1, 0}) == 32);

    // Mixed: -1 and 1 (0001) differ in all but the lowest bit → distance 31.
    assert(sumBitDifferences({-1, 1}) == 31);

    // Multiple pairs with duplicates: {0,1,0} → (0,1)=1, (0,2)=0, (1,2)=1 → total 2.
    assert(sumBitDifferences({0, 1, 0}) == 2);

    // Larger vector: 0, 3, 5, 7
    // 0(000) 3(011) 5(101) 7(111) — using 3 bits for clarity
    // Pairs:
    // (0,3): diff=3 →2
    // (0,5): diff=5 →2
    // (0,7): diff=7 →3
    // (3,5): diff=6 →2
    // (3,7): diff=4 →1
    // (5,7): diff=2 →1
    // Sum = 2+2+3+2+1+1 = 11.
    assert(sumBitDifferences({0, 3, 5, 7}) == 11);

    // All zeros: no differences.
    assert(sumBitDifferences({0, 0, 0}) == 0);

    // All bits set in both numbers → identical, distance 0.
    assert(sumBitDifferences({0xFFFFFFFF, 0xFFFFFFFF}) == 0);

    return 0;
}
#include <vector>
#include <cstdint>
#include <bitset>

// Return the sum of Hamming distances over all unordered pairs (i < j).
long long sumBitDifferences(const std::vector<int>& arr) {
    const int n = static_cast<int>(arr.size());
    long long total = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int diff = arr[i] ^ arr[j];
            // Count set bits in the 32-bit representation.
            std::bitset<32> bs(diff);
            total += static_cast<long long>(bs.count());
        }
    }
    return total;
}
// The straightforward approach is to iterate over all unordered pairs `(i, j)` with `i < j`, compute the XOR of the two numbers, count the set bits in that XOR using `std::bitset<32>` or `__builtin_popcount`, and add that count to the total. This is correct because the Hamming distance between two bit strings equals the number of bits set in their bitwise XOR. The complexity is \(O(n^2)\) time, which is acceptable for small `n` (e.g., up to 10^3). For each pair, counting set bits takes \(O(1)\) (up to 32 operations). The space complexity is \(O(1)\) auxiliary, ignoring the input vector. Edge cases: an empty vector is not allowed per the spec, and a single element yields zero pairs so the result is 0. Negative numbers are handled naturally because `std::bitset<32>` of an `int` gives the two's complement representation of the low 32 bits, and XOR of two negative numbers correctly reflects differing bits in those 32 bits. Duplicate values produce XOR of zero, contributing zero. The function must use `const` references to avoid copying the vector. The return type is `long long` (at least 64-bit) to avoid overflow since the maximum Hamming distance per pair is 32, and the number of pairs is at most \(n(n-1)/2\), so for `n` up to ~2 million the sum could exceed `int` range.
