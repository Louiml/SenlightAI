Write a C++ function named `findTwoSumIndices` that takes a vector of integers and a target integer as input, and returns a vector of two integers representing the 1-based indices of the two numbers in the vector that sum to the target. The two indices must be returned in ascending order. You may assume each input has exactly one solution, and the vector contains at least two distinct positions (the same element cannot be used twice). The solution must use a hash map to achieve better than quadratic time complexity. The function should be `const`-correct (i.e., accept the input vector by const reference) and handle cases where the target or numbers are negative or zero.

// The core idea is to use a hash map (in C++, `std::unordered_map`) to store the complement of each number as we iterate. For each element `numbers[i]`, we check if `numbers[i]` already exists in the map. If it does, that means we have previously seen the value `target - numbers[i]` at some earlier index `j`, and we can return `{j+1, i+1}` (since indices are 1-based). Otherwise, we store the complement `target - numbers[i]` with its current index `i`. This works because we assume exactly one solution exists, so we won't overwrite a needed complement. Edge cases: numbers can be negative, zero, or large; the map handles all integer values. If the solution pair is found when `i` is the second element, the earlier index is guaranteed smaller, so we push the earlier index first. Time complexity is O(n) because each lookup and insertion in the unordered map averages O(1). Space complexity is O(n) for storing up to n complement entries in the map.

#include <vector>
#include <unordered_map>

// Given a vector of integers and a target sum, return the 1-based indices
// of the two numbers that add up to the target. Indices are in ascending order.
std::vector<int> findTwoSumIndices(const std::vector<int>& numbers, int target) {
    std::unordered_map<int, int> complementIndex; // value -> index
    std::vector<int> result;

    for (size_t i = 0; i < numbers.size(); ++i) {
        int current = numbers[i];
        auto it = complementIndex.find(current);
        if (it != complementIndex.end()) {
            // Found the complement earlier
            result.push_back(it->second + 1); // earlier index (1-based)
            result.push_back(i + 1);          // current index (1-based)
            break; // exactly one solution, so we can stop
        } else {
            // Store the complement needed for current value
            complementIndex[target - current] = i;
        }
    }
    return result;
}

#include <cassert>
#include <vector>

// Assume the solution function is defined above

int main() {
    // Example from the snippet
    std::vector<int> v1 = {2, 7, 11, 15};
    assert(findTwoSumIndices(v1, 9) == std::vector<int>({1, 2}));

    // Negative numbers
    std::vector<int> v2 = {-3, 4, 3, 90};
    assert(findTwoSumIndices(v2, 0) == std::vector<int>({1, 3}));

    // Zero target with zeros
    std::vector<int> v3 = {0, 4, 3, 0};
    assert(findTwoSumIndices(v3, 0) == std::vector<int>({1, 4}));

    // Large numbers
    std::vector<int> v4 = {1000000, 2000000, 3000000};
    assert(findTwoSumIndices(v4, 5000000) == std::vector<int>({2, 3}));

    // Duplicate values but different positions
    std::vector<int> v5 = {1, 5, 5, 11};
    assert(findTwoSumIndices(v5, 10) == std::vector<int>({2, 3}));

    // Target negative
    std::vector<int> v6 = {-10, -20, 30, 5};
    assert(findTwoSumIndices(v6, -30) == std::vector<int>({1, 2}));

    // Two elements only
    std::vector<int> v7 = {-1, 1};
    assert(findTwoSumIndices(v7, 0) == std::vector<int>({1, 2}));

    // Mixed with zero and negatives
    std::vector<int> v8 = {3, -2, 8, 0, -1};
    assert(findTwoSumIndices(v8, -2) == std::vector<int>({2, 5}));

    // Larger vector
    std::vector<int> v9 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    assert(findTwoSumIndices(v9, 19) == std::vector<int>({9, 10}));

    // Single pair with target 0 but non-zero elements
    std::vector<int> v10 = {7, -7, 3};
    assert(findTwoSumIndices(v10, 0) == std::vector<int>({1, 2}));

    return 0;
}
