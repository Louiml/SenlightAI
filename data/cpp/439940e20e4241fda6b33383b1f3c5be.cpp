// Write a C++ function that takes a non-empty array of integers and returns the unique number that appears exactly once, given that every other number in the array appears exactly three times. The function should be named `findUniqueNumber` and accept a pointer to the array (or a `std::vector<int>`) along with its size, returning the single integer that occurs only once. The array may contain negative numbers, zero, and large integer values, and you may assume the input is always valid (exactly one unique number, and every other number appears exactly three times). Do not use sorting as the primary approach; instead, use a bitwise method that counts the occurrence of each bit across all numbers and reduces the counts modulo 3 to recover the unique number's bits.

// The core idea is to leverage the fact that the frequency of every bit in the numbers that appear three times is a multiple of 3, while the bit contributions from the unique number (which appears once) will not cancel out. For each of the 32 bits (or 64 for larger ints), we count how many numbers in the array have that bit set. Since each repeated number contributes exactly 1 to each of its set bits for each of its 3 occurrences, the total count for any bit position across all numbers is `3 * (number of repeated numbers that have that bit) + (1 if the unique number has that bit else 0)`. Therefore, taking each bit count modulo 3 leaves exactly the bit value of the unique number. After processing all numbers and all bit positions, we reconstruct the unique number by setting bits where the count modulo 3 is 1. This works even with negative numbers because we treat the underlying binary representation using integer bit operations (e.g., using `(num >> i) & 1`), and we process all 64 bits to cover the full range of `long long` if needed, but for `int` we can process 32 bits. Edge cases include a single element in the array (the answer is that element), negative numbers (bitwise operations with `>>` on signed integers are implementation-defined for negative values, but when shifting right, most compilers do arithmetic shift; to be safe, we can process using unsigned integers or just iterate over fixed 32 bits using `(num & (1LL << i))` to avoid issues). For an array of size `n`, the algorithm runs in `O(n * b)` time where `b` is the fixed number of bits (e.g., 32 or 64), so effectively `O(n)`. Auxiliary space is `O(1)` if we use a fixed-size frequency array, or `O(b)` which is constant.

#include <vector>
#include <cstdint>

// Given an array where every number appears exactly three times except one unique number,
// return the unique number using bitwise counting modulo 3.
// The input array is provided as a const reference to a vector for safety and clarity.
// If you prefer a raw array, the function can be overloaded, but this version uses std::vector.
int findUniqueNumber(const std::vector<int>& arr) {
    // Use 64 bits to handle negative numbers and large values safely.
    int bitCount[64] = {0};

    // Count the number of times each bit is set across all numbers.
    for (int num : arr) {
        // Convert to unsigned to ensure well-defined right shifts.
        uint64_t value = static_cast<uint64_t>(num);
        for (int bit = 0; bit < 64; ++bit) {
            if (value & (1ULL << bit)) {
                ++bitCount[bit];
            }
        }
    }

    // Reduce each bit count modulo 3 and reconstruct the unique number.
    uint64_t result = 0;
    for (int bit = 0; bit < 64; ++bit) {
        bitCount[bit] %= 3;
        if (bitCount[bit] == 1) {
            result |= (1ULL << bit);
        }
    }

    // Cast back to signed int (the unique value fits within int range).
    return static_cast<int>(result);
}

#include <cassert>
#include <vector>

int findUniqueNumber(const std::vector<int>& arr); // declaration from solution

int main() {
    // Basic case with positive numbers
    assert(findUniqueNumber({1, 1, 1, 2, 2, 2, 3}) == 3);

    // Single element case
    assert(findUniqueNumber({7}) == 7);

    // Negative numbers
    assert(findUniqueNumber({-1, -1, -1, -2, -2, -2, -5}) == -5);

    // Mix of negative and positive, zero appears
    assert(findUniqueNumber({0, 1, 1, 1, -2, -2, -2}) == 0);

    // Larger values
    assert(findUniqueNumber({1000000, 1000000, 1000000, 999999, 999999, 999999, 42}) == 42);

    // Duplicate unique? Not allowed but check correctness for one unique
    assert(findUniqueNumber({5, 5, 5, 2}) == 2);

    // Multiple elements, unique at start
    assert(findUniqueNumber({10, 3, 3, 3, 4, 4, 4}) == 10);

    // Unique in middle, negative and large
    assert(findUniqueNumber({-2147483648, 2147483647, 2147483647, 2147483647, -2147483648, -2147483648, 123}) == 123);

    // All numbers of same value repeated, unique is different
    assert(findUniqueNumber({8, 8, 8, 9}) == 9);

    return 0;
}
