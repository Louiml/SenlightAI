Write a standalone C++ function named `countDistinctElements` that takes a `std::vector<int>` as input and returns the number of distinct (unique) integers present in it. The function should handle empty vectors by returning 0, and should correctly account for negative numbers, zeros, and duplicate values that may appear in any order. The input vector is passed by const reference, and the function must not modify the input. The task is focused purely on counting distinct elements efficiently and correctly.
The most straightforward approach is to use a hash-based container, such as `std::unordered_set<int>`, to store each unique value as we iterate through the vector. Inserting each element into the set automatically discards duplicates, so the size of the set at the end gives the count of distinct integers. Alternatively, a `std::map` could be used, but the unordered version provides average O(1) insertion and O(n) total time, which is optimal for this problem. Edge cases include an empty vector (should return 0), a vector with all identical elements (should return 1), and vectors containing negative numbers or zeros, which are handled naturally by the set. The time complexity is O(n) on average, and the space complexity is O(k) where k is the number of distinct elements (at most n). This solution is simple, robust, and does not require sorting or additional pre-processing.
#include <vector>
#include <unordered_set>

// Count the number of distinct integers in the input vector.
// Returns 0 for an empty vector, otherwise the count of unique elements.
int countDistinctElements(const std::vector<int>& nums) {
    std::unordered_set<int> uniqueElements;
    for (int value : nums) {
        uniqueElements.insert(value);
    }
    return static_cast<int>(uniqueElements.size());
}
#include <cassert>
#include <vector>

int countDistinctElements(const std::vector<int>& nums); // declaration

int main() {
    // Basic cases
    assert(countDistinctElements({}) == 0);
    assert(countDistinctElements({1, 2, 3, 4}) == 4);
    assert(countDistinctElements({1, 1, 1, 1}) == 1);
    assert(countDistinctElements({1, -1, 0, 2, -1}) == 4);

    // With negative and zero
    assert(countDistinctElements({-5, -5, -1, 0}) == 3);

    // Large vector with many duplicates
    std::vector<int> large(10000, 42);
    assert(countDistinctElements(large) == 1);

    // Mixed duplicates in non-sorted order
    assert(countDistinctElements({3, 1, 2, 1, 3, 2, 4}) == 4);

    // Single element
    assert(countDistinctElements({7}) == 1);

    // All distinct but large spread
    std::vector<int> spread = {1000000, -1000000, 0, 1, -1};
    assert(countDistinctElements(spread) == 5);

    return 0;
}
