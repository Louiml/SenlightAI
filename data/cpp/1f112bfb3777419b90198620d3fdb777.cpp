// Write a C++ function named `shellSortWithSteps` that takes a vector of integers (with possible duplicate values) and a vector of positive integer gaps (the "delta" sequence), and returns a new vector containing the sorted result after applying the Shell sort algorithm. The function must perform the sorting by using the provided gaps in order, and for each gap, it must do an insertion sort on elements spaced by that gap. The returned vector must be fully sorted in non-decreasing order. Assume the gaps are valid (positive and decreasing) and the last gap is always 1 to guarantee full sorting. The function should not modify the input vector.

The solution implements Shell sort, which is an optimization over insertion sort by allowing exchanges of far-apart elements. For each gap `dk` from the provided gap sequence, we perform a "gap insertion sort": we start at index `dk` (0-based) and compare each element with those `dk` positions before it, shifting larger elements forward by `dk` positions until the correct spot is found. Using a sentinel copy of the current element simplifies the shifting loop. After processing all gaps, the vector is fully sorted because the last gap is 1, which is equivalent to a standard insertion sort. Important edge cases include: an empty input vector (return empty), a vector with one element (already sorted), and duplicate values (they remain stable in relative order among equal keys, though stability is not strictly required). The algorithm's time complexity is approximately \(O(n^{3/2})\) for the common gap sequence like powers of two minus one, but in general depends on the gap sequence; the worst-case can be \(O(n^2)\) for poorly chosen gaps. Space complexity is \(O(1)\) auxiliary, excluding the output vector that must be created as a copy.

#include <vector>

// Perform Shell sort on a copy of the input vector using the given gap sequence.
// Returns a new vector containing the sorted elements.
std::vector<int> shellSortWithSteps(const std::vector<int>& input, const std::vector<int>& gaps) {
    std::vector<int> result = input; // work on a copy
    
    int n = static_cast<int>(result.size());
    for (int dk : gaps) {
        // Insertion sort with step dk
        for (int i = dk; i < n; ++i) {
            int key = result[i];
            int j = i - dk;
            // Shift elements that are greater than key, dk positions to the right
            while (j >= 0 && result[j] > key) {
                result[j + dk] = result[j];
                j -= dk;
            }
            result[j + dk] = key;
        }
    }
    return result;
}

#include <cassert>
#include <vector>

// Function declaration (or include the solution header)
std::vector<int> shellSortWithSteps(const std::vector<int>& input, const std::vector<int>& gaps);

int main() {
    // Basic sorting with gaps {5,3,1}
    std::vector<int> v1 = {49, 38, 65, 97, 76, 13, 27, 49, 55, 4};
    std::vector<int> gaps1 = {5, 3, 1};
    std::vector<int> sorted1 = shellSortWithSteps(v1, gaps1);
    assert(sorted1 == (std::vector<int>{4, 13, 27, 38, 49, 49, 55, 65, 76, 97}));

    // Empty input
    std::vector<int> v2 = {};
    std::vector<int> gaps2 = {1};
    assert(shellSortWithSteps(v2, gaps2).empty());

    // Single element
    std::vector<int> v3 = {42};
    std::vector<int> gaps3 = {1};
    assert(shellSortWithSteps(v3, gaps3) == (std::vector<int>{42}));

    // Already sorted
    std::vector<int> v4 = {1, 2, 3, 4, 5};
    std::vector<int> gaps4 = {3, 1};
    assert(shellSortWithSteps(v4, gaps4) == (std::vector<int>{1, 2, 3, 4, 5}));

    // Reverse sorted
    std::vector<int> v5 = {5, 4, 3, 2, 1};
    std::vector<int> gaps5 = {2, 1};
    assert(shellSortWithSteps(v5, gaps5) == (std::vector<int>{1, 2, 3, 4, 5}));

    // Duplicate values
    std::vector<int> v6 = {3, 1, 3, 2, 3};
    std::vector<int> gaps6 = {2, 1};
    assert(shellSortWithSteps(v6, gaps6) == (std::vector<int>{1, 2, 3, 3, 3}));

    // Negative numbers
    std::vector<int> v7 = {-5, -1, -10, 0, 2};
    std::vector<int> gaps7 = {3, 1};
    assert(shellSortWithSteps(v7, gaps7) == (std::vector<int>{-10, -5, -1, 0, 2}));

    // Input not modified
    std::vector<int> original = {3, 1, 2};
    std::vector<int> copy = original;
    shellSortWithSteps(original, {1});
    assert(original == copy);

    // Larger gap sequence (e.g., gaps not ending with 1 in test, but function assumes 1 eventually)
    std::vector<int> v8 = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    std::vector<int> gaps8 = {4, 2, 1};
    assert(shellSortWithSteps(v8, gaps8) == (std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9}));

    // All equal
    std::vector<int> v9 = {7, 7, 7, 7};
    std::vector<int> gaps9 = {2, 1};
    assert(shellSortWithSteps(v9, gaps9) == (std::vector<int>{7, 7, 7, 7}));
}
