// Write a C++ function named `countPairsAndLeftovers` that takes a `std::vector<int>` containing non-negative integers in the inclusive range [0, 100] and returns a `std::vector<int>` of exactly two elements: the first element is the total number of pairs (i.e., floor(value / 2) summed over all distinct values) that can be formed from identical numbers, and the second element is the total number of leftover single elements (i.e., the sum of `value % 2` for all distinct values). The function must be `const`-qualified, operate on the input by value, and use a fixed-size frequency array (not a hash map) because the value range is known and small. The input vector may be empty, in which case the result must be `{0, 0}`. The function must not modify the input or allocate dynamic memory except for the returned vector. The solution must be self-contained and not rely on any external class or global state.
The algorithm uses a frequency array of size 101 (indices 0 through 100) to count occurrences of each possible value in the input. Iterate through the input vector once, incrementing the frequency for each element. After counting, iterate through the entire frequency array. For each value with a count greater than zero, add `count / 2` (number of complete pairs) to the first result element and `count % 2` (leftover singles) to the second result element. Edge cases include an empty input (frequency array remains all zeros, result is `{0, 0}`), values at the boundary (0 and 100) which must be handled by the fixed-size array, and duplicate values where pairs and leftovers are correctly computed via integer division and modulo. Time complexity is O(n + 101) = O(n) where n is the input size, and space complexity is O(1) auxiliary (the frequency array is constant size) plus the O(1) space for the result vector.
#include <vector>
#include <array>

// Count the total number of pairs and leftover singles from integer values.
// Values must be in the inclusive range [0, 100].
std::vector<int> countPairsAndLeftovers(const std::vector<int>& input) {
    std::array<int, 101> frequency{};
    for (const int value : input) {
        ++frequency[value];
    }

    std::vector<int> result(2, 0);
    for (const int count : frequency) {
        if (count > 0) {
            result[0] += count / 2;
            result[1] += count % 2;
        }
    }
    return result;
}
#include <cassert>
#include <vector>

// Solution function declaration
std::vector<int> countPairsAndLeftovers(const std::vector<int>& input);

int main() {
    // Empty input should yield zero pairs and zero leftovers
    assert(countPairsAndLeftovers({}) == std::vector<int>({0, 0}));

    // Single value with even count: all are pairs
    assert(countPairsAndLeftovers({5, 5, 5, 5}) == std::vector<int>({2, 0}));

    // Single value with odd count: one leftover
    assert(countPairsAndLeftovers({3, 3, 3}) == std::vector<int>({1, 1}));

    // Multiple distinct values: 4 twos -> 2 pairs, 3 sevens -> 1 pair + 1 leftover, 1 zero -> 0 pair + 1 leftover
    assert(countPairsAndLeftovers({2, 2, 2, 2, 7, 7, 7, 0}) == std::vector<int>({3, 2}));

    // Values at boundaries 0 and 100
    assert(countPairsAndLeftovers({0, 0, 100, 100, 100}) == std::vector<int>({2, 1}));

    // All zeros and hundreds in large quantity
    std::vector<int> big_input(200, 100);
    big_input.insert(big_input.end(), 101, 0);
    assert(countPairsAndLeftovers(big_input) == std::vector<int>({150, 1}));

    // Values in non-sorted order with duplicates
    assert(countPairsAndLeftovers({1, 2, 1, 2, 1, 3}) == std::vector<int>({2, 2}));

    // Input with only one element: zero pairs, one leftover
    assert(countPairsAndLeftovers({42}) == std::vector<int>({0, 1}));
}
