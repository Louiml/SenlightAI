// Write a C++ function `vector<int> computeSubsetSums(const vector<int>& nums)` that takes a non-empty vector of integers and returns a vector containing all possible subset sums, where each subset sum is the total of the elements chosen from a subset of the original array. The result may contain duplicate values if different subsets yield the same sum. The order of the returned sums does not matter for correctness, but the function must include the sum of the empty subset (which is 0) and the sum of the full set. The input vector may contain negative numbers, zeros, and duplicates. Assume the input size is at most 20 so that generating all 2^n subsets is feasible. The function must not modify the input vector.
#include <cassert>
#include <vector>
#include <algorithm>
// The function is declared above; here we test it.
int main() {
    // Test 1: Simple positive numbers
    {
        std::vector<int> input = {1, 2};
        std::vector<int> expected = {0, 1, 2, 3}; // subsets: {}, {1}, {2}, {1,2}
        std::vector<int> result = computeSubsetSums(input);
        std::sort(result.begin(), result.end());
        std::sort(expected.begin(), expected.end());
        assert(result == expected);
    }
    // Test 2: Includes negative numbers
    {
        std::vector<int> input = {-1, 2};
        // Subsets: {} ->0, {-1}->-1, {2}->2, {-1,2}->1
        std::vector<int> expected = {0, -1, 2, 1};
        std::vector<int> result = computeSubsetSums(input);
        std::sort(result.begin(), result.end());
        std::sort(expected.begin(), expected.end());
        assert(result == expected);
    }
    // Test 3: Single element
    {
        std::vector<int> input = {5};
        std::vector<int> expected = {0, 5};
        std::vector<int> result = computeSubsetSums(input);
        std::sort(result.begin(), result.end());
        std::sort(expected.begin(), expected.end());
        assert(result == expected);
    }
    // Test 4: Zeros cause duplicate sums
    {
        std::vector<int> input = {0, 1};
        // Subsets: {}, {0}, {1}, {0,1} -> sums 0,0,1,1
        std::vector<int> result = computeSubsetSums(input);
        std::sort(result.begin(), result.end());
        std::vector<int> expected = {0, 0, 1, 1};
        assert(result == expected);
    }
    // Test 5: All equal elements
    {
        std::vector<int> input = {2, 2};
        // Subsets: {} ->0, {2}->2, {2}->2, {2,2}->4
        std::vector<int> result = computeSubsetSums(input);
        std::sort(result.begin(), result.end());
        std::vector<int> expected = {0, 2, 2, 4};
        assert(result == expected);
    }
    // Test 6: Empty input (edge case, but the task says non-empty; still test for completeness)
    {
        std::vector<int> input = {};
        std::vector<int> result = computeSubsetSums(input);
        std::vector<int> expected = {0};
        assert(result == expected);
    }
    return 0;
}
#include <vector>
#include <cstddef>

// Compute all possible subset sums from the input array.
// The result includes the empty subset sum (0) and may contain duplicates.
std::vector<int> computeSubsetSums(const std::vector<int>& nums) {
    std::vector<int> result;
    // Recursive helper to explore subsets starting from a given index.
    // The current sum is kept in `currentSum`.
    // Note: nums is const reference, we only read from it.
    std::function<void(size_t, int)> dfs = [&](size_t index, int currentSum) {
        // Base case: all elements have been considered.
        if (index == nums.size()) {
            result.push_back(currentSum);
            return;
        }
        // Include the current element in the subset sum.
        dfs(index + 1, currentSum + nums[index]);
        // Exclude the current element from the subset sum.
        dfs(index + 1, currentSum);
    };
    dfs(0, 0);
    return result;
}
// The core idea is to recursively explore every subset by deciding for each element whether to include it in the current subset sum or exclude it. Starting at index 0 with an initial temporary sum of 0, at each index we branch into two recursive calls: one that adds the current element to the sum and moves to the next index, and one that skips the current element and moves to the next index. When the index reaches the size of the array, we push the current accumulated sum into the result vector. This depth-first exploration guarantees every combination is visited exactly once. Edge cases include negative numbers (they correctly reduce the sum when included), zeros (inclusion or exclusion yields the same sum, so duplicates appear), and duplicate elements (they naturally produce repeated sums, which are acceptable). The time complexity is O(2^n) because there are 2^n subsets and each subset is generated once; the auxiliary space is O(n) for the recursion stack plus O(2^n) for the result vector. The solution is straightforward and does not require sorting or deduplication.
