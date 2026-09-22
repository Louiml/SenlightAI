/*
Write a C++ function named `processLimits` that takes a vector of integers representing user-entered limits and returns a vector containing only the limits that are strictly greater than their position index (0-based) in the input vector. For example, if the input vector contains `{5, 2, 10, 3}`, the output should be `{5, 10}` because 5>0, 2 is not >1, 10>2, 3 is not >3. The function should handle an empty input vector by returning an empty vector, and should not modify the input. Use `const` where appropriate.
*/
#include <vector>

// Return a vector of elements from the input that are strictly greater than their index.
std::vector<int> processLimits(const std::vector<int>& limits) {
    std::vector<int> result;
    for (std::size_t i = 0; i < limits.size(); ++i) {
        if (limits[i] > static_cast<int>(i)) {
            result.push_back(limits[i]);
        }
    }
    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Empty input returns empty output.
    assert(processLimits({}) == std::vector<int>({}));

    // Standard case with mixed values.
    std::vector<int> input1 = {5, 2, 10, 3};
    std::vector<int> expected1 = {5, 10};
    assert(processLimits(input1) == expected1);

    // All elements satisfy condition (all positive and index is small).
    std::vector<int> input2 = {1, 2, 3, 4};
    assert(processLimits(input2) == input2);

    // No element satisfies condition (all indices are greater or equal to values).
    std::vector<int> input3 = {0, 1, 2, 3};
    assert(processLimits(input3) == std::vector<int>({}));

    // Negative values and zeros.
    std::vector<int> input4 = {-5, 3, 0, 4};
    std::vector<int> expected4 = {3, 4}; // -5<0, 3>1, 0<2, 4>3
    assert(processLimits(input4) == expected4);

    // Single element.
    std::vector<int> input5 = {0};
    assert(processLimits(input5) == std::vector<int>({}));

    std::vector<int> input6 = {10};
    assert(processLimits(input6) == std::vector<int>({10}));
}
// The main algorithm iterates through the input vector using an index variable (starting from 0). For each position `i`, if the element `vec[i]` is greater than `i`, we append that element to a result vector. This is a simple linear scan with a single conditional push-back. Edge cases: empty input returns an empty result; negative values and zero are handled naturally by the strict `>` comparison, so only positive values greater than their index will be included. The time complexity is O(n) where n is the size of the input vector, and space complexity is O(m) where m is the number of elements that satisfy the condition (which is at most n).
