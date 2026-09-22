Write a C++ function that takes a non-empty sorted array of integers (sorted in ascending order) and a target integer value. The function must find two numbers in the array that sum to exactly the target and return their 1-based indices as a vector of two integers. Because the array is sorted, you should solve the problem efficiently without using extra space. If no such pair exists, return an empty vector. The input array is guaranteed to contain exactly one valid solution if it exists, but you must still handle the no-solution case gracefully.

// The optimal approach leverages the sorted order via the two-pointer technique. Initialize `left` to the first index (0) and `right` to the last index (size-1). At each step, compute the sum of `numbers[left]` and `numbers[right]`. If the sum equals the target, return `{left+1, right+1}` because the problem expects 1-based indices. If the sum is less than the target, the only way to increase the sum is to move the left pointer forward (`left++`), since the array is sorted and moving right backward would decrease the sum further. Conversely, if the sum is greater than the target, move the right pointer backward (`right--`) to reduce the sum. Continue until `left >= right`. If no pair is found, return an empty vector. Edge cases include: the array having only two elements, the target being the sum of the two endpoints, and the pair being adjacent (e.g., indices 1 and 2). Time complexity is O(n) because each pointer moves at most n steps total. Space complexity is O(1) auxiliary, not counting the output vector.

#include <vector>

// Given a sorted ascending vector of integers and a target, return 1-based indices
// of the two numbers that sum to target. Returns an empty vector if no such pair exists.
std::vector<int> findTwoSumSorted(const std::vector<int>& numbers, int target) {
    int left = 0;
    int right = static_cast<int>(numbers.size()) - 1;

    while (left < right) {
        int sum = numbers[left] + numbers[right];
        if (sum == target) {
            return {left + 1, right + 1};
        } else if (sum < target) {
            ++left;
        } else {
            --right;
        }
    }

    return {};
}

#include <cassert>
#include <vector>

int main() {
    // Basic case with pair in the middle
    std::vector<int> v1 = {2, 7, 11, 15};
    assert(findTwoSumSorted(v1, 9) == std::vector<int>({1, 2}));

    // Pair at the ends
    std::vector<int> v2 = {1, 3, 5, 7};
    assert(findTwoSumSorted(v2, 8) == std::vector<int>({1, 4}));

    // Adjacent pair
    std::vector<int> v3 = {1, 2, 3, 4};
    assert(findTwoSumSorted(v3, 3) == std::vector<int>({1, 2}));

    // Negative numbers
    std::vector<int> v4 = {-3, -1, 0, 2};
    assert(findTwoSumSorted(v4, -1) == std::vector<int>({2, 4}));

    // Duplicate values (but indices distinct)
    std::vector<int> v5 = {1, 1, 2};
    assert(findTwoSumSorted(v5, 2) == std::vector<int>({1, 2}));

    // No valid pair
    std::vector<int> v6 = {1, 2, 3};
    assert(findTwoSumSorted(v6, 10).empty());

    // Minimum size (two elements that match)
    std::vector<int> v7 = {5, 5};
    assert(findTwoSumSorted(v7, 10) == std::vector<int>({1, 2}));

    // Larger array, pair not at extremes
    std::vector<int> v8 = {2, 3, 4, 5, 8, 9, 12, 15};
    assert(findTwoSumSorted(v8, 20) == std::vector<int>({5, 8}));

    // Zero and positive
    std::vector<int> v9 = {0, 3, 7};
    assert(findTwoSumSorted(v9, 7) == std::vector<int>({1, 3}));

    // Large numbers and target
    std::vector<int> v10 = {100, 200, 300, 400};
    assert(findTwoSumSorted(v10, 600) == std::vector<int>({2, 4}));

    return 0;
}
