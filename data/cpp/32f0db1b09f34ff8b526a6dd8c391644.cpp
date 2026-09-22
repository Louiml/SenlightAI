Write a C++ function named `findSingleNumber` that takes a non-empty vector of integers, where every element appears exactly twice except for one element that appears only once, and returns that single element. The vector may contain duplicate values, negative numbers, and zero, but it is guaranteed that exactly one number appears an odd number of times (specifically exactly once, not three or five times — the input will always contain exactly one unique value and all others will appear exactly twice). The function should be deterministic and should not modify the input vector.
// The optimal solution uses the bitwise XOR operation. Since XORing a number with itself yields zero, and XORing with zero leaves the number unchanged, XORing all elements in the array cancels out every pair of identical numbers, leaving only the one number that appears once. This works because XOR is associative and commutative, so the order of processing does not matter. Edge cases: the array may contain negative numbers (XOR works on their two's-complement representation), zero, and many duplicates; the algorithm handles all of these correctly. If the array had only one element, that element is the answer. Time complexity is O(n) with a single pass over the array, and auxiliary space is O(1) because only one integer variable is used for accumulation. The solution does not rely on sorting or extra data structures, making it both time- and space-efficient.
#include <vector>

// Given a vector where each integer appears exactly twice except for one
// integer that appears once, return the integer that appears only once.
// Uses XOR cancellation: pairs of equal numbers cancel out, leaving the unique value.
int findSingleNumber(const std::vector<int>& arr) {
    int result = 0;
    for (const int value : arr) {
        result ^= value;  // XOR accumulates; duplicates cancel, unique remains
    }
    return result;
}
#include <cassert>
#include <vector>

// The solution function is declared in this test block.
int findSingleNumber(const std::vector<int>& arr);

int main() {
    // Basic case: unique number is in the middle
    std::vector<int> test1 = {2, 3, 5, 3, 2};
    assert(findSingleNumber(test1) == 5);

    // Unique number is the first element
    std::vector<int> test2 = {7, 4, 4, 9, 9, 1, 1};
    assert(findSingleNumber(test2) == 7);

    // Unique number is the last element
    std::vector<int> test3 = {4, 4, 3, 3, 8, 8, 11};
    assert(findSingleNumber(test3) == 11);

    // Single element array
    std::vector<int> test4 = {42};
    assert(findSingleNumber(test4) == 42);

    // Negative numbers and zero
    std::vector<int> test5 = {-3, -3, 0, 0, -7, -7, 5};
    assert(findSingleNumber(test5) == 5);

    // All duplicates except one negative unique value
    std::vector<int> test6 = {6, -10, 6, 6, 6, -10, -10};
    assert(findSingleNumber(test6) == 6);

    // Large numbers (still within int range)
    std::vector<int> test7 = {123456, -123456, 789012, 789012, 123456};
    assert(findSingleNumber(test7) == -123456);

    // Two elements: both are duplicates, but this should not happen; however, if it did, XOR gives zero.
    // Skipped because input guarantee is exactly one unique value.

    // Mixed sequence with duplicates scattered
    std::vector<int> test8 = {1, 2, 1, 3, 2, 4, 4, 5, 5};
    assert(findSingleNumber(test8) == 3);

    return 0;
}
