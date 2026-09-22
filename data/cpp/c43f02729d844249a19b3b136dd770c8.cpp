Write a C++ function `std::vector<int> stablePartitionByThreshold(const std::vector<int>& values, int threshold, bool keepGreater)` that takes a vector of integers, a threshold value, and a boolean flag. The function must return a new vector that contains all elements from the input that satisfy the filter condition, preserving their original relative order. If `keepGreater` is `true`, keep elements strictly greater than `threshold`; if `false`, keep elements strictly less than `threshold`. The input vector must not be modified. Handle empty input by returning an empty vector. Edge cases include negative numbers, duplicate values, and cases where no elements satisfy the condition. The function must be `const`-correct and use only standard library components.

// The solution iterates through the input vector once, applying the appropriate comparison based on the boolean flag. For `keepGreater == true`, we check `value > threshold`; for `false`, we check `value < threshold`. We append qualifying values to a result vector in the order they appear, which naturally preserves relative order and is stable. No sorting or rearrangement is needed. Time complexity is \(O(n)\) where \(n\) is the size of the input vector. Space complexity is \(O(m)\) for the result vector, where \(m \le n\) is the number of matching elements. Edge cases: empty input returns an empty result; if no elements match, an empty vector is returned; negative values and duplicates are handled naturally by the comparisons. The function takes the input vector by const reference to avoid copying and prevent modification.

#include <vector>

// Return a new vector containing elements from `values` that satisfy the filter.
// If keepGreater is true, keep elements > threshold; if false, keep elements < threshold.
// The original vector is not modified, and relative order of kept elements is preserved.
std::vector<int> stablePartitionByThreshold(const std::vector<int>& values, int threshold, bool keepGreater) {
    std::vector<int> result;
    for (int value : values) {
        if (keepGreater) {
            if (value > threshold) {
                result.push_back(value);
            }
        } else {
            if (value < threshold) {
                result.push_back(value);
            }
        }
    }
    return result;
}

#include <cassert>
#include <vector>

// Assume the solution function is declared above.

int main() {
    // Basic greater-than filtering
    std::vector<int> input1 = {1, 5, 3, 8, 2};
    std::vector<int> expected1 = {5, 8};
    assert(stablePartitionByThreshold(input1, 3, true) == expected1);

    // Basic less-than filtering
    std::vector<int> input2 = {1, 5, 3, 8, 2};
    std::vector<int> expected2 = {1, 3, 2};
    assert(stablePartitionByThreshold(input2, 4, false) == expected2);

    // Empty input
    std::vector<int> empty;
    assert(stablePartitionByThreshold(empty, 0, true).empty());
    assert(stablePartitionByThreshold(empty, 0, false).empty());

    // No elements satisfy condition
    std::vector<int> input3 = {1, 2, 3};
    assert(stablePartitionByThreshold(input3, 10, true).empty());
    assert(stablePartitionByThreshold(input3, 0, false).empty());

    // Duplicates and negative numbers
    std::vector<int> input4 = {-5, -1, -1, 0, 2, 2};
    std::vector<int> expected4 = {-1, -1, 2, 2};
    assert(stablePartitionByThreshold(input4, -5, true) == expected4);
    std::vector<int> expected5 = {-5, -1, -1};
    assert(stablePartitionByThreshold(input4, 0, false) == expected5);

    // Original input must not be modified
    std::vector<int> original = {1, 2, 3};
    std::vector<int> copy = original;
    stablePartitionByThreshold(original, 1, true);
    assert(original == copy);

    return 0;
}
