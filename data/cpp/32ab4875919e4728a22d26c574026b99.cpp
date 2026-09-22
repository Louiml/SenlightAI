/*
Write a C++ function named `findSingularElement` that takes a `const std::vector<int>&` and returns the single integer that appears exactly once in the vector, given that every other integer appears exactly twice. The function must run in linear time and use constant auxiliary space. The input vector is guaranteed to be non-empty, and you can assume the existence of exactly one non-duplicated number. Your implementation should not modify the input vector and should handle vectors with negative numbers, zeros, and large positive values.
*/
#include <vector>

// Returns the integer that appears exactly once in the input vector,
// where all other integers appear exactly twice. Uses XOR to cancel pairs.
int findSingularElement(const std::vector<int>& nums) {
    int result = 0;
    for (const int value : nums) {
        result ^= value;
    }
    return result;
}
#include <cassert>
#include <vector>

// Declaration of the tested function (assumed to be included from above)
int findSingularElement(const std::vector<int>& nums);

int main() {
    // Basic mixed values
    assert(findSingularElement({1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5}) == 6);
    // Vector with a single element
    assert(findSingularElement({42}) == 42);
    // Negative and zero values
    assert(findSingularElement({-1, -1, 0, 0, -5}) == -5);
    // Large positive numbers and repeated pairs
    assert(findSingularElement({1000000, 999999, 1000000, 999999, 12345}) == 12345);
    // Zeros and a unique negative
    assert(findSingularElement({0, 0, 0, 0, -7}) == -7);
    // Duplicates in non-adjacent order
    assert(findSingularElement({7, 3, 7, 9, 3}) == 9);
    // Multiple zeros and one positive
    assert(findSingularElement({0, 0, 5}) == 5);
    // Unique element is zero
    assert(findSingularElement({1, 2, 1, 2, 0}) == 0);
    // All pairs except the first element
    assert(findSingularElement({77, 1, 1, 2, 2}) == 77);
    // All pairs except the last element
    assert(findSingularElement({1, 1, 2, 2, 88}) == 88);
    return 0;
}
// The solution uses the bitwise XOR operation, which has the property that `x ^ x == 0` and `x ^ 0 == x`, and is both commutative and associative. By XORing all elements of the vector together, paired identical numbers cancel each other out (resulting in 0), leaving only the element that appears once. This works because XOR of the same number twice yields 0 regardless of order, and the remaining result is the unique number. Edge cases include: a vector with only one element (the function returns that element immediately), negative numbers (XOR works identically on two's complement representations), and zeros (which do not affect the XOR result). The time complexity is O(n) due to a single pass over the vector, and the space complexity is O(1) because only a single integer accumulator is used—matching the task’s linear runtime and constant memory constraints.
