Write a C++ function named `filterLessThan` that takes a vector of integers and an integer threshold value `x`, and returns a new vector containing only those elements from the original vector that are strictly less than `x`, preserving their original relative order. The function must not modify the input vector, must be `const`-correct by taking the vector by `const` reference, and must handle edge cases such as an empty input vector or when no elements satisfy the condition (returning an empty vector). The solution should not rely on any external libraries beyond the standard library.
#include <cassert>
#include <vector>

// (Assume the filterLessThan function is defined above.)
int main() {
    // Basic filtering
    std::vector<int> v1 = {1, 5, 3, 8, 2};
    assert(filterLessThan(v1, 5) == std::vector<int>({1, 3, 2}));

    // All elements less than threshold
    std::vector<int> v2 = { -3, 0, 4 };
    assert(filterLessThan(v2, 10) == v2);

    // No elements less than threshold
    std::vector<int> v3 = {10, 20, 30};
    assert(filterLessThan(v3, 10).empty());

    // Empty input
    std::vector<int> v4;
    assert(filterLessThan(v4, 100).empty());

    // Negative numbers and threshold
    std::vector<int> v5 = {-5, -1, -10, 0, 7};
    assert(filterLessThan(v5, -1) == std::vector<int>({-5, -10}));

    // Duplicates preserved
    std::vector<int> v6 = {2, 2, 2, 3};
    assert(filterLessThan(v6, 3) == std::vector<int>({2, 2, 2}));

    // Threshold equal to some values (strict inequalities)
    std::vector<int> v7 = {5, 4, 3, 2, 1};
    assert(filterLessThan(v7, 3) == std::vector<int>({2, 1}));

    // Single element less than threshold
    std::vector<int> v8 = {8};
    assert(filterLessThan(v8, 9) == std::vector<int>({8}));

    // Single element not less than threshold
    std::vector<int> v9 = {9};
    assert(filterLessThan(v9, 9).empty());

    // Verify original vector unchanged
    std::vector<int> original = {1, 2, 3};
    std::vector<int> copy = original;
    filterLessThan(original, 2);
    assert(original == copy);

    return 0;
}
#include <vector>

// Return a new vector containing only elements strictly less than x,
// preserving the original order. The input vector is not modified.
std::vector<int> filterLessThan(const std::vector<int>& nums, int x) {
    std::vector<int> result;
    for (int value : nums) {
        if (value < x) {
            result.push_back(value);
        }
    }
    return result;
}
// The approach is straightforward: iterate through the input vector element by element. For each element, check if it is strictly less than the given threshold `x`. If it is, push that value into a result vector that we build incrementally. Since we only need a simple linear scan and append operations, the algorithm runs in O(n) time, where n is the number of elements in the input vector. The auxiliary space complexity is O(m) in the worst case for the result vector, where m is the number of elements that pass the filter (m ≤ n). If the input is empty or no element satisfies the condition, the result vector is naturally empty. Duplicates are preserved because we do not deduplicate. Negative numbers and zero are handled correctly because integer comparisons are straightforward. The original vector remains unchanged because we take it by const reference and only read from it. This is a basic filtering problem often seen in introductory programming exercises.
