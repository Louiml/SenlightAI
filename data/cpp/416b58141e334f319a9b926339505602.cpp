Write a C++ function named `expandEachElement` that takes a non-empty vector of integers and returns a new vector where each integer `x` from the input is expanded into `x` consecutive copies of `x`, preserving the original order. For example, given `{2, 3, 1}`, the output should be `{2, 2, 3, 3, 3, 1}`. Handle both positive and negative numbers appropriately: a positive `x` repeats it `x` times, a negative `x` (say `-2`) should repeat the original negative value `-2` exactly `-x` (i.e., 2) times, and `0` contributes nothing. The input vector will contain at least one element, and the total output size may be large, so consider efficiency.
The solution processes each element in the input vector sequentially. For each value `v`, if `v == 0`, skip it. Otherwise, determine the repeat count as `abs(v)` (or explicitly handle sign with a conditional). For a negative value, we still insert the original negative value into the output `abs(v)` times. This can be done by looping from `0` to `abs(v)-1` and pushing `v` each time. The main complexity is the repeated insertion; using `push_back` is efficient. Edge cases: input with only zeros produces an empty output vector; a mix of signs is handled uniformly. Time complexity is O(S) where S is the sum of absolute values of input elements, because each output element is produced exactly once. Space complexity is O(S) for the output vector (plus O(1) auxiliary space). We must avoid using `insert` at the beginning (as in the snippet) because that would be O(n) per insertion; instead, we build the result with `push_back` which is amortized O(1) per element.
#include <vector>
#include <cstdlib> // for std::abs

// Expands each integer x into |x| copies of x. Zero contributes nothing.
// Preserves the original order of input elements.
std::vector<int> expandEachElement(const std::vector<int>& arr) {
    std::vector<int> result;
    result.reserve(arr.size()); // optional, but helps avoid reallocations

    for (int value : arr) {
        int count = std::abs(value);
        for (int i = 0; i < count; ++i) {
            result.push_back(value);
        }
    }
    return result;
}
#include <cassert>
#include <vector>

// (The solution function above is assumed to be included.)

int main() {
    // Basic positive case
    std::vector<int> input1 = {2, 3, 1};
    std::vector<int> expected1 = {2, 2, 3, 3, 3, 1};
    assert(expandEachElement(input1) == expected1);

    // Mixed signs
    std::vector<int> input2 = {-2, 3, 0, -1};
    std::vector<int> expected2 = {-2, -2, 3, 3, 3, -1};
    assert(expandEachElement(input2) == expected2);

    // All zeros gives empty output
    std::vector<int> input3 = {0, 0, 0};
    std::vector<int> expected3 = {};
    assert(expandEachElement(input3) == expected3);

    // Single negative value
    std::vector<int> input4 = {-4};
    std::vector<int> expected4 = {-4, -4, -4, -4};
    assert(expandEachElement(input4) == expected4);

    // Single positive value
    std::vector<int> input5 = {5};
    std::vector<int> expected5 = {5, 5, 5, 5, 5};
    assert(expandEachElement(input5) == expected5);

    // Large repeats
    std::vector<int> input6 = {1000, -2, 1};
    std::vector<int> expected6;
    for (int i = 0; i < 1000; ++i) expected6.push_back(1000);
    expected6.push_back(-2);
    expected6.push_back(-2);
    expected6.push_back(1);
    assert(expandEachElement(input6) == expected6);

    return 0;
}
