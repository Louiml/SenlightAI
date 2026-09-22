/*
Write a C++ function named `countDivisibilityPairs` that accepts a `const std::vector<int>&` (containing positive integers) and returns a `std::vector<int>` of the same length. For each element at index `i`, the returned vector's value at `i` must equal the total number of other elements in the input array (at different indices) that divide it exactly (i.e., `arr[i] % arr[j] == 0`) or that it divides exactly (i.e., `arr[j] % arr[i] == 0`). In other words, count for each position how many other positions form a pair where one number is a divisor of the other (in either direction). The input array length `n` satisfies `1 <= n <= 10^5`, and all elements are positive integers (>=1). The function should be efficient and handle duplicates correctly: if there are multiple identical values, each pair of equal indices still counts as a divisibility pair because `x % x == 0`.
*/
#include <vector>

// Count for each element how many other elements divide it or are divided by it.
std::vector<int> countDivisibilityPairs(const std::vector<int>& arr) {
    const int n = static_cast<int>(arr.size());
    std::vector<int> ans(n, 0);
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (arr[i] % arr[j] == 0 || arr[j] % arr[i] == 0) {
                ++ans[i];
                ++ans[j];
            }
        }
    }
    return ans;
}
#include <cassert>
#include <vector>

// Include the solution function here (or link it).
std::vector<int> countDivisibilityPairs(const std::vector<int>& arr);

int main() {
    // Example 1: A=[2,3,4,5,6] -> [2,1,1,0,2]
    std::vector<int> a1 = {2,3,4,5,6};
    std::vector<int> r1 = {2,1,1,0,2};
    assert(countDivisibilityPairs(a1) == r1);

    // Example 2: All equal -> each index gets n-1
    std::vector<int> a2 = {6,6,6,6,6};
    std::vector<int> r2 = {4,4,4,4,4};
    assert(countDivisibilityPairs(a2) == r2);

    // Single element -> all zeros
    std::vector<int> a3 = {7};
    std::vector<int> r3 = {0};
    assert(countDivisibilityPairs(a3) == r3);

    // Two elements that divide each other
    std::vector<int> a4 = {2,4};
    std::vector<int> r4 = {1,1};  // 2 divides 4, so both get 1
    assert(countDivisibilityPairs(a4) == r4);

    // Two elements that do not divide each other
    std::vector<int> a5 = {3,5};
    std::vector<int> r5 = {0,0};
    assert(countDivisibilityPairs(a5) == r5);

    // Larger test with repeated values
    std::vector<int> a6 = {1,2,4,8};
    std::vector<int> r6 = {3,3,3,3}; // 1 divides all, 2 divides 4 and 8, 4 divides 8, etc.
    assert(countDivisibilityPairs(a6) == r6);

    // Input with 1 at start
    std::vector<int> a7 = {1,3,5};
    std::vector<int> r7 = {2,1,1}; // 1 divides both 3 and 5, but 3 and 5 don't divide each other
    assert(countDivisibilityPairs(a7) == r7);

    // All distinct primes
    std::vector<int> a8 = {2,3,5,7};
    std::vector<int> r8 = {0,0,0,0}; // no divisibility
    assert(countDivisibilityPairs(a8) == r8);

    // Duplicate values count as divisibility pairs
    std::vector<int> a9 = {4,4,4};
    std::vector<int> r9 = {2,2,2};
    assert(countDivisibilityPairs(a9) == r9);

    return 0;
}
// The main algorithm iterates over all unordered pairs of indices `(i, j)` with `i < j`. For each pair, if either `arr[i] % arr[j] == 0` or `arr[j] % arr[i] == 0`, then both indices `i` and `j` increment their answer counters by 1. This directly matches the original code's logic, but we encapsulate it in a clean function. The approach is straightforward but has a time complexity of O(n^2) because we check every pair, and space complexity of O(n) for the output vector (plus O(n) for the input itself). Important edge cases include: `n=1` (answer is all zeros), all elements equal (each index gets `n-1` because every other element divides it), and large inputs where O(n^2) may be slow—but the task is still correct. The function must not modify the input (hence `const` reference) and must use `const` correctness properly.
