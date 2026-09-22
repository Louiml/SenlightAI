// Write a C++ function named `uniqueSortedValues` that accepts a vector of integers and returns a new vector containing the unique values from the input sorted in ascending order, using an `unordered_set` for deduplication and then sorting the resulting elements. The input vector may contain duplicates, negative numbers, zero, and positive numbers. The function must not modify the input vector. If the input is empty, return an empty vector. The output order is strictly ascending, and each value appears exactly once.
#include <cassert>
#include <vector>
#include <iostream>

// Assume the solution is declared above; here's a self-contained test main.
int main() {
    // Basic mixed values with duplicates
    std::vector<int> v1 = {3, 1, 2, 3, 1, 0, -1};
    assert(uniqueSortedValues(v1) == std::vector<int>({-1, 0, 1, 2, 3}));

    // All duplicate values
    std::vector<int> v2 = {7, 7, 7, 7};
    assert(uniqueSortedValues(v2) == std::vector<int>({7}));

    // Already sorted, no duplicates
    std::vector<int> v3 = {-5, -2, 0, 4, 9};
    assert(uniqueSortedValues(v3) == std::vector<int>({-5, -2, 0, 4, 9}));

    // Empty input
    std::vector<int> v4;
    assert(uniqueSortedValues(v4) == std::vector<int>());

    // Input with one element
    std::vector<int> v5 = {42};
    assert(uniqueSortedValues(v5) == std::vector<int>({42}));

    // Negative and zero duplicates
    std::vector<int> v6 = {0, -1, -1, 0, 3, 3, 3};
    assert(uniqueSortedValues(v6) == std::vector<int>({-1, 0, 3}));

    std::cout << "All tests passed." << std::endl;
    return 0;
}
#include <vector>
#include <unordered_set>
#include <algorithm>

// Return a sorted vector of unique values from the input vector.
std::vector<int> uniqueSortedValues(const std::vector<int>& input) {
    std::unordered_set<int> uniqueSet(input.begin(), input.end());
    std::vector<int> result(uniqueSet.begin(), uniqueSet.end());
    std::sort(result.begin(), result.end());
    return result;
}
// The problem requires deduplication and sorting. An `unordered_set<int>` is ideal for deduplication because it offers average O(1) insertion and lookup. After inserting all input elements into the set, the set will contain each distinct value exactly once, but in an arbitrary order (since it’s a hash set). To meet the sorting requirement, copy the set's contents into a vector, then sort that vector in ascending order using `std::sort`. Edge cases include an empty input (return empty vector) and inputs with all identical values (return a single-element vector). Time complexity: O(n) for deduplication (average case, assuming hash is good), plus O(m log m) for sorting, where m is the number of unique elements. In the worst case m = n, so overall O(n log n). Space complexity: O(n) for the set and the output vector.
