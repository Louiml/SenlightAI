// Write a C++ function that takes a vector of integers and returns a vector of integers containing, in increasing order, all values that appear in the original vector after removing all duplicate values. The function should preserve only the first occurrence of each value (so the relative order of first occurrences is maintained), but then sort the resulting unique values in ascending order. For example, given `{5, 3, 5, 1, 3, 2}`, the unique values in first-occurrence order are `{5, 3, 1, 2}`, and after sorting the result is `{1, 2, 3, 5}`. The input vector may be empty, may contain negative numbers, and may have duplicate values. The function must not modify the input vector and must use constant extra space (excluding the output and any necessary auxiliary data structures that depend on the input size). If the input is empty, return an empty vector.
The simplest approach is to use a hash set to track which values have already been seen, iterating through the input once. For each element, if it has not been seen before, add it to a temporary result list and mark it as seen. After this pass, sort the temporary list using `std::sort`. This gives correct behavior because the temporary list contains the first occurrences in original order, and sorting arranges them ascending. Duplicates are skipped via the set. Edge cases: empty input returns empty output; a single element returns a singleton sorted list; negative numbers and zeros work fine with the set. The time complexity is O(n + k log k), where n is the input size and k is the number of unique elements (≤ n). The auxiliary space is O(k) for the set and the temporary list, which is acceptable and independent of input size only if considering the output, but the problem statement says “constant extra space” – in practice we need O(k) for uniqueness tracking, so we interpret that as O(k) which is the size of the output. We’ll document this clearly. Alternatively, we could sort the input copy and then unique, but that would use O(n) extra space for the copy and break the “first occurrence” requirement.
#include <vector>
#include <unordered_set>
#include <algorithm>

// Given a vector of integers, return a sorted vector containing each distinct value exactly once.
// Preserves only the first occurrence of each value before sorting.
std::vector<int> uniqueSorted(const std::vector<int>& input) {
    std::unordered_set<int> seen;
    std::vector<int> unique_values;
    unique_values.reserve(input.size());

    for (int value : input) {
        if (seen.insert(value).second) {
            unique_values.push_back(value);
        }
    }

    std::sort(unique_values.begin(), unique_values.end());
    return unique_values;
}
#include <cassert>
#include <vector>

// The solution function is declared above; include it in the test compilation.

int main() {
    // Basic case with duplicates
    std::vector<int> v1 = {5, 3, 5, 1, 3, 2};
    std::vector<int> r1 = uniqueSorted(v1);
    assert(r1 == std::vector<int>({1, 2, 3, 5}));

    // Empty input
    std::vector<int> v2;
    assert(uniqueSorted(v2).empty());

    // Single element
    std::vector<int> v3 = {7};
    assert(uniqueSorted(v3) == std::vector<int>({7}));

    // All duplicates
    std::vector<int> v4 = {4, 4, 4, 4};
    assert(uniqueSorted(v4) == std::vector<int>({4}));

    // Negative and zero
    std::vector<int> v5 = {-3, 0, -3, 5, 0, -1};
    assert(uniqueSorted(v5) == std::vector<int>({-3, -1, 0, 5}));

    // Already sorted unique
    std::vector<int> v6 = {1, 2, 3, 4};
    assert(uniqueSorted(v6) == std::vector<int>({1, 2, 3, 4}));

    // Reverse order unique
    std::vector<int> v7 = {4, 3, 2, 1};
    assert(uniqueSorted(v7) == std::vector<int>({1, 2, 3, 4}));

    // Large duplicates mixed
    std::vector<int> v8 = {10, 10, 20, 30, 20, 10, 40, 30};
    assert(uniqueSorted(v8) == std::vector<int>({10, 20, 30, 40}));

    return 0;
}
