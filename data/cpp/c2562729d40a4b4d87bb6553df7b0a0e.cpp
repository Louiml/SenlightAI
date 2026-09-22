Write a C++ function that takes a vector of integers and returns a vector of integers containing the starting indices of all maximal increasing runs (strictly increasing contiguous subarrays) of length at least 2. If no such run exists, return an empty vector. For example, given `{1, 2, 3, 1, 2, 5, 0}`, the maximal increasing runs are `[0..2]` (values 1,2,3) and `[3..5]` (values 1,2,5), so the function returns `{0, 3}`. Note that overlapping or shorter runs (like `[1..2]` or `[0..1]`) should not be included; only the start index of each maximal run that spans at least two elements.

// The algorithm scans the array once, maintaining the start index of the current potential run. For each position `i` from 1 to n-1, if `v[i] > v[i-1]`, the run continues; otherwise, if the current run has length at least 2 (i.e., `i - start >= 2`), we record `start`. Then we reset `start = i` for the next potential run. At the end, after the loop, we need to check the last run similarly. Important edge cases: empty array (return empty), single-element array (no run of length 2), all increasing (the whole array is one run from index 0), all decreasing (no runs), and equal adjacent elements (break the run because it is not strictly increasing). Time complexity is O(n) with O(1) auxiliary space (excluding output vector).

#include <vector>

// Return starting indices of all maximal strictly increasing runs of length at least 2.
std::vector<int> increasingRunStarts(const std::vector<int>& values) {
    std::vector<int> result;
    const int n = static_cast<int>(values.size());
    if (n < 2) return result;

    int start = 0;
    for (int i = 1; i < n; ++i) {
        if (values[i] > values[i - 1]) {
            // Current run continues.
        } else {
            // Run ended at i-1.
            if (i - start >= 2) {
                result.push_back(start);
            }
            start = i; // start new potential run at current index
        }
    }
    // Check the final run after loop.
    if (n - start >= 2) {
        result.push_back(start);
    }
    return result;
}

#include <cassert>
#include <vector>

// Function declaration from solution
std::vector<int> increasingRunStarts(const std::vector<int>& values);

int main() {
    assert((increasingRunStarts({1, 2, 3, 1, 2, 5, 0}) == std::vector<int>{0, 3}));
    assert((increasingRunStarts({1, 2, 3}) == std::vector<int>{0}));
    assert((increasingRunStarts({3, 2, 1}) == std::vector<int>{}));
    assert((increasingRunStarts({1, 1, 1}) == std::vector<int>{}));
    assert((increasingRunStarts({1, 2, 2, 3}) == std::vector<int>{0, 2}));
    assert((increasingRunStarts({}) == std::vector<int>{}));
    assert((increasingRunStarts({5}) == std::vector<int>{}));
    assert((increasingRunStarts({1, 3, 2, 4, 6, 5}) == std::vector<int>{0, 2}));
    assert((increasingRunStarts({1, 2, 3, 4}) == std::vector<int>{0}));
    assert((increasingRunStarts({4, 3, 2, 1, 0, 1, 2}) == std::vector<int>{4}));
    return 0;
}
