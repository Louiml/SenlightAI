// Write a C++ function `vector<vector<int>> findPairsWithSum(const vector<int>& arr, int target)` that takes a vector of integers (which may be unsorted and may contain duplicates) and a target sum `target`, and returns a vector of all unique pairs of indices `(i, j)` with `i < j` such that `arr[i] + arr[j] == target`. For each valid pair, store the smaller value first and the larger value second in a two‑element inner vector. If both values are equal (e.g., `[3,3]` with target 6), store them as two equal numbers. The outer vector must be sorted in lexicographic (dictionary) order: first by the first element of the pair, then by the second element. If no pairs exist, return an empty vector. Your function must be `const`‑correct (take the input vector by `const&`), and you may use only the C++ standard library. Assume the input vector is non‑empty.

#include <cassert>
#include <vector>

int main() {
    // Basic unsorted with duplicates
    std::vector<int> a1 = {5, 1, 2, 3, 4, 1};
    std::vector<std::vector<int>> r1 = findPairsWithSum(a1, 5);
    std::vector<std::vector<int>> e1 = {{1,4}, {1,4}, {2,3}};
    assert(r1 == e1);

    // All equal values
    std::vector<int> a2 = {3, 3, 3};
    std::vector<std::vector<int>> r2 = findPairsWithSum(a2, 6);
    std::vector<std::vector<int>> e2 = {{3,3}, {3,3}, {3,3}};
    assert(r2 == e2);

    // No valid pairs
    std::vector<int> a3 = {1, 2, 3};
    std::vector<std::vector<int>> r3 = findPairsWithSum(a3, 100);
    assert(r3.empty());

    // Negative numbers
    std::vector<int> a4 = {-3, 0, 3, 5, -1};
    std::vector<std::vector<int>> r4 = findPairsWithSum(a4, 2);
    std::vector<std::vector<int>> e4 = {{-3,5}, {-1,3}};
    assert(r4 == e4);

    // Single element
    std::vector<int> a5 = {7};
    std::vector<std::vector<int>> r5 = findPairsWithSum(a5, 14);
    assert(r5.empty());

    // Target zero with positive and corresponding negative
    std::vector<int> a6 = {2, -2, 0, 4, -4};
    std::vector<std::vector<int>> r6 = findPairsWithSum(a6, 0);
    std::vector<std::vector<int>> e6 = {{-2,2}, {-4,4}};
    assert(r6 == e6);

    return 0;
}

#include <vector>
#include <algorithm>

// Return all unique index pairs (i<j) whose values sum to target,
// sorted lexicographically, with each pair stored as {smaller, larger}.
std::vector<std::vector<int>> findPairsWithSum(const std::vector<int>& arr, int target) {
    std::vector<std::vector<int>> result;
    const int n = static_cast<int>(arr.size());

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (arr[i] + arr[j] == target) {
                result.push_back({std::min(arr[i], arr[j]), std::max(arr[i], arr[j])});
            }
        }
    }

    std::sort(result.begin(), result.end());
    return result;
}

// The simplest correct approach is to use two nested loops. For each index `i` from `0` to `n-1`, iterate over all `j` from `i+1` to `n-1`. When `arr[i] + arr[j] == target`, create an inner vector containing `min(arr[i], arr[j])` and `max(arr[i], arr[j])`, and push it into the answer. After all pairs are collected, sort the outer vector lexicographically using `std::sort` (default comparator works for vectors of ints). Important edge cases: duplicate values in the input should produce multiple pairs, because each distinct combination of indices counts (e.g., `[1,1,1]` with target 2 gives three pairs). Pairs where both numbers are equal (like `[3,3]` when target 6) are handled correctly because `min` and `max` both return the same value. The sorting step ensures the output is deterministic and matches the requirement. Time complexity is `O(n²)` for the nested loops plus `O(m log m)` for sorting, where `m` is the number of valid pairs (at most `n(n-1)/2`). Space complexity is `O(m)` for the result, not counting the input.
