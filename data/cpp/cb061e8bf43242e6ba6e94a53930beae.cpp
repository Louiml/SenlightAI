// Write a C++ function named `findSingleNumbers` that accepts a vector of integers (where every integer appears exactly twice except for two distinct integers that appear exactly once) and returns a vector containing those two unique integers in any order. The input vector is non-empty, and the two unique integers are guaranteed to be distinct. The function must be `const`-qualified where appropriate and handle edge cases such as duplicate values appearing consecutively or spaced apart, and negative numbers. The function should not modify the input vector.

The provided snippet uses an unordered_map to count occurrences of each number, then collects keys whose count equals 1. This straightforward approach works but requires extra O(n) space. A more elegant solution uses bitwise XOR: XORing all numbers cancels out pairs, leaving the XOR of the two unique numbers. Since the two numbers are distinct, this XOR has at least one set bit; we can isolate the rightmost set bit to partition the array into two groups: numbers with that bit set, and numbers without. XORing each group separately yields the two unique numbers. Edge cases include numbers that are negative (bitwise operations work on two's complement representation), duplicates appearing anywhere, and the guarantee of exactly two unique numbers. Time complexity is O(n) and space complexity is O(1) for the XOR approach, versus O(n) time and O(n) space for the map-based approach. The solution must return a vector of size 2.

#include <vector>
#include <numeric>

// Return the two numbers that appear exactly once in the input vector.
// The input vector must have exactly two elements that appear once,
// and all other elements must appear exactly twice.
std::vector<int> findSingleNumbers(const std::vector<int>& nums) {
    // XOR all elements; pairs cancel out, leaving xor of the two unique numbers.
    int xors = 0;
    for (int num : nums) {
        xors ^= num;
    }

    // Isolate the rightmost set bit (since the two unique numbers differ in this bit).
    int rightmost_bit = xors & -xors;

    // Partition numbers by the rightmost bit and XOR each group.
    int first = 0, second = 0;
    for (int num : nums) {
        if (num & rightmost_bit) {
            first ^= num;
        } else {
            second ^= num;
        }
    }

    return {first, second};
}

#include <vector>
#include <cassert>
#include <algorithm>

int main() {
    // Basic case
    std::vector<int> nums1 = {1, 2, 1, 3, 2, 5};
    auto res1 = findSingleNumbers(nums1);
    assert(res1.size() == 2);
    assert((res1[0] == 3 && res1[1] == 5) || (res1[0] == 5 && res1[1] == 3));

    // Duplicates not adjacent
    std::vector<int> nums2 = {4, 7, 4, 9, 7, 2};
    auto res2 = findSingleNumbers(nums2);
    assert(res2.size() == 2);
    assert((res2[0] == 9 && res2[1] == 2) || (res2[0] == 2 && res2[1] == 9));

    // Negative numbers
    std::vector<int> nums3 = {-1, -2, -1, -3, -2, -4};
    auto res3 = findSingleNumbers(nums3);
    assert(res3.size() == 2);
    assert((res3[0] == -3 && res3[1] == -4) || (res3[0] == -4 && res3[1] == -3));

    // Large numbers
    std::vector<int> nums4 = {100000, 200000, 100000, 300000, 200000, 400000};
    auto res4 = findSingleNumbers(nums4);
    assert(res4.size() == 2);
    assert((res4[0] == 300000 && res4[1] == 400000) || (res4[0] == 400000 && res4[1] == 300000));

    // Only two numbers (both unique)
    std::vector<int> nums5 = {7, 9};
    auto res5 = findSingleNumbers(nums5);
    assert(res5.size() == 2);
    assert((res5[0] == 7 && res5[1] == 9) || (res5[0] == 9 && res5[1] == 7));

    // Mixed with zeros
    std::vector<int> nums6 = {0, 1, 1, 2, 2, 3};
    auto res6 = findSingleNumbers(nums6);
    assert(res6.size() == 2);
    assert((res6[0] == 0 && res6[1] == 3) || (res6[0] == 3 && res6[1] == 0));

    return 0;
}
