Write a standalone C++ function that, given a `std::vector<int>` of non-negative integer scores (each in the range 0 to 25), returns a new `std::vector<int>` containing the median and the two elements adjacent to the median in sorted order: specifically, the elements at positions `n/2 - 1`, `n/2`, and `n/2 + 1` (0-based) after sorting, where `n` is the vector size. If `n` is less than 3, return an empty vector. The function must not modify the input vector, must handle duplicate values correctly, and must not use any external libraries beyond the C++ standard library. The solution should be self-contained, with a clearly named free function and appropriate `const` correctness.
// The solution must first validate the input size: if the vector has fewer than 3 elements, return an empty vector immediately. For valid inputs, we need to find the three elements at the middle indices of the sorted sequence. The simplest approach is to copy the input vector, sort the copy, and then extract the elements at indices `n/2 - 1`, `n/2`, and `n/2 + 1` (note these indices are valid because `n/2 + 1 < n` when `n >= 3`). Sorting takes `O(n log n)` time and `O(n)` auxiliary space for the copy. Edge cases include duplicate values (which are handled naturally by sorting) and even/odd `n` (the median index `n/2` works for both). There is no risk of index overflow because we check `n >= 3` first. An alternative more efficient approach could use `std::nth_element` to find the three relevant elements in `O(n)` expected time, but sorting is simpler and clearer for this task; complexity remains acceptable.
#include <vector>
#include <algorithm>

// Given a vector of scores, return a vector containing the three elements
// at positions n/2-1, n/2, n/2+1 of the sorted sequence, or an empty vector
// if the input has fewer than 3 elements. The input vector is not modified.
std::vector<int> medianNeighbors(const std::vector<int>& scores) {
    const std::size_t n = scores.size();
    if (n < 3) {
        return {};
    }

    // Work on a copy to avoid modifying the caller's data.
    std::vector<int> sorted = scores;
    std::sort(sorted.begin(), sorted.end());

    const std::size_t middle = n / 2;
    return {
        sorted[middle - 1],
        sorted[middle],
        sorted[middle + 1]
    };
}
#include <cassert>
#include <vector>

// Solution function declaration (as above) would be included here.

int main() {
    // Basic odd-length case
    std::vector<int> scores1 = {5, 3, 8, 1, 9};
    assert(medianNeighbors(scores1) == std::vector<int>({3, 5, 8}));

    // Even-length case: n=6, middle index = 3, neighbors at 2,3,4
    std::vector<int> scores2 = {10, 2, 7, 4, 11, 5};
    assert(medianNeighbors(scores2) == std::vector<int>({4, 5, 7}));

    // All duplicates
    std::vector<int> scores3 = {7, 7, 7, 7, 7};
    assert(medianNeighbors(scores3) == std::vector<int>({7, 7, 7}));

    // Input with exactly 3 elements
    std::vector<int> scores4 = {1, 3, 2};
    assert(medianNeighbors(scores4) == std::vector<int>({1, 2, 3}));

    // Input with exactly 2 elements -> empty
    std::vector<int> scores5 = {4, 6};
    assert(medianNeighbors(scores5).empty());

    // Input with exactly 1 element -> empty
    std::vector<int> scores6 = {42};
    assert(medianNeighbors(scores6).empty());

    // Input with n=4: middle index=2, neighbors at 1,2,3
    std::vector<int> scores7 = {0, 25, 10, 15};
    assert(medianNeighbors(scores7) == std::vector<int>({10, 15, 25}));

    // Ensure input is not modified
    std::vector<int> original = {5, 3, 8, 1, 9};
    std::vector<int> copy = original;
    (void)medianNeighbors(original);
    assert(original == copy);

    // Edge with all same values and n=3
    std::vector<int> scores8 = {2, 2, 2};
    assert(medianNeighbors(scores8) == std::vector<int>({2, 2, 2}));

    return 0;
}
