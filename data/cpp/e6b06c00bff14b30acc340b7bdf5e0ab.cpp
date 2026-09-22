// Write a C++ function that takes a non-empty vector of integers and returns a new vector containing the prefix sums (running sums) of the input, where each element at index `i` in the result is the sum of all elements from index `0` through `i` in the original vector. The input vector must not be modified, and the function should handle both small and large inputs efficiently. For example, given `[3, 1, 2]`, the result should be `[3, 4, 6]`.
The solution iterates through the input vector exactly once, maintaining a running total. For the first element, the running sum equals that element itself; for every subsequent element, the prefix sum at position `i` is `running_total + nums[i]`, and we update the running total accordingly. The main edge case is an empty input (though the problem states non-empty, we can guard against it by returning an empty vector). Another edge case is a single-element vector, where the output is just that element. The algorithm runs in O(n) time, where n is the number of elements, because it makes a single pass. It uses O(n) auxiliary space for the result vector (which is required by the return type); ignoring the output storage, the algorithm itself uses O(1) extra space.
#include <vector>

// Return the prefix sums of the input vector.
// Example: runningSum({3, 1, 2}) -> {3, 4, 6}
std::vector<int> computePrefixSums(const std::vector<int>& nums) {
    if (nums.empty()) {
        return {};
    }

    std::vector<int> prefixSums;
    prefixSums.reserve(nums.size());

    int runningTotal = 0;
    for (int value : nums) {
        runningTotal += value;
        prefixSums.push_back(runningTotal);
    }

    return prefixSums;
}
#include <cassert>
#include <vector>

// The solution function is provided above; this main function tests it.
int main() {
    // Basic case
    std::vector<int> input1 = {3, 1, 2};
    std::vector<int> expected1 = {3, 4, 6};
    assert(computePrefixSums(input1) == expected1);

    // Single element
    std::vector<int> input2 = {5};
    std::vector<int> expected2 = {5};
    assert(computePrefixSums(input2) == expected2);

    // Negative numbers
    std::vector<int> input3 = {-2, -1, -3};
    std::vector<int> expected3 = {-2, -3, -6};
    assert(computePrefixSums(input3) == expected3);

    // Mixed positive and negative
    std::vector<int> input4 = {1, -1, 1, -1};
    std::vector<int> expected4 = {1, 0, 1, 0};
    assert(computePrefixSums(input4) == expected4);

    // Larger vector with zeros
    std::vector<int> input5 = {0, 0, 0};
    std::vector<int> expected5 = {0, 0, 0};
    assert(computePrefixSums(input5) == expected5);

    // Multiple elements with repeated values
    std::vector<int> input6 = {4, 4, 4};
    std::vector<int> expected6 = {4, 8, 12};
    assert(computePrefixSums(input6) == expected6);

    // Input vector is not modified
    std::vector<int> input7 = {2, 3, 4};
    std::vector<int> original = input7;
    computePrefixSums(input7);
    assert(input7 == original);

    // Empty input (optional; problem says non-empty, but robust)
    std::vector<int> input8 = {};
    std::vector<int> expected8 = {};
    assert(computePrefixSums(input8) == expected8);
}
