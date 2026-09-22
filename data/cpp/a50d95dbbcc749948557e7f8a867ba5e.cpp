/*
Write a C++ function that takes a non-empty vector of integers where every number appears exactly twice except for two distinct numbers that appear once, and returns a vector containing those two unique numbers. The order of the two returned numbers does not matter, and the input vector may contain negative numbers, zeros, and duplicate values (with the exception of the two singletons). For example, given `{1, 2, 1, 3, 2, 5}` the function should return `{3, 5}` in any order. The function must be `const`-correct and should not modify the input vector.
*/

#include <vector>

// Returns the two numbers that appear only once in the input vector.
// Every other number appears exactly twice.
std::vector<int> findTwoUniqueNumbers(const std::vector<int>& nums) {
    int xor_all = 0;
    for (int num : nums) {
        xor_all ^= num;
    }

    // xor_all holds x ^ y. Since x != y, xor_all has at least one set bit.
    // Isolate the lowest set bit.
    int lowest_bit = xor_all & -xor_all;

    int first = 0;
    int second = 0;
    for (int num : nums) {
        if (num & lowest_bit) {
            first ^= num;
        } else {
            second ^= num;
        }
    }

    return {first, second};
}

#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    {
        std::vector<int> input = {1, 2, 1, 3, 2, 5};
        std::vector<int> result = findTwoUniqueNumbers(input);
        std::sort(result.begin(), result.end());
        assert(result == std::vector<int>({3, 5}));
    }
    {
        std::vector<int> input = {0, -1, -1, 0, 42, 7, 42, 9};
        std::vector<int> result = findTwoUniqueNumbers(input);
        std::sort(result.begin(), result.end());
        assert(result == std::vector<int>({7, 9}));
    }
    {
        std::vector<int> input = {-3, -3, -2, -2, 100, -100};
        std::vector<int> result = findTwoUniqueNumbers(input);
        std::sort(result.begin(), result.end());
        assert(result == std::vector<int>({-100, 100}));
    }
    {
        std::vector<int> input = {5, 5, 4, 4, 10, 11};
        std::vector<int> result = findTwoUniqueNumbers(input);
        std::sort(result.begin(), result.end());
        assert(result == std::vector<int>({10, 11}));
    }
    {
        std::vector<int> input = {0, 1, 2, 0, 1, 2, 3, 4};
        std::vector<int> result = findTwoUniqueNumbers(input);
        std::sort(result.begin(), result.end());
        assert(result == std::vector<int>({3, 4}));
    }
    {
        std::vector<int> input = {2147483647, -2147483648, 2147483647, 5, 5, -2147483648, 123, 456};
        std::vector<int> result = findTwoUniqueNumbers(input);
        std::sort(result.begin(), result.end());
        assert(result == std::vector<int>({123, 456}));
    }
    {
        std::vector<int> input = {1, 1, 2, 2, 3, 3, 4, 4, 5, 6};
        std::vector<int> result = findTwoUniqueNumbers(input);
        std::sort(result.begin(), result.end());
        assert(result == std::vector<int>({5, 6}));
    }
}

// The core idea is to use XOR properties. XORing all numbers together cancels out pairs (since `a ^ a = 0`) and leaves `x ^ y`, where `x` and `y` are the two unique numbers. Since `x != y`, the XOR result has at least one set bit. That set bit indicates a position where `x` and `y` differ. We can isolate that bit (e.g., the lowest set bit) and partition the original numbers into two groups: those with that bit set and those with it clear. Because every duplicate pair has identical bits, each pair falls entirely into one group, and the two unique numbers fall into different groups. XORing each group separately yields `x` and `y`. Edge cases: the input must contain at least two distinct numbers, but there is no size restriction beyond that; zeros and negatives work because XOR and bitwise operations are defined for signed integers (using two’s complement representation). Time complexity is O(n) with a single pass for XOR, a loop over at most 31 bits, and a second pass for partitioning. Space complexity is O(1) auxiliary, ignoring the output vector.
