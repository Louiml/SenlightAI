Write a C++ function `countAdjacentDifferences` that takes a vector of integers and returns the number of indices that are "marked" according to the following rule: an index `i` is marked if either `a[i] != a[i+1]` (for `i < n-1`) or `a[i] != a[i-1]` (for `i > 0`). In other words, for every adjacent pair of elements that differ, both positions in that pair are marked. The function should return the count of all distinct marked indices. The input vector will have at least one element. The function must be `const`-correct (take the vector by `const&`), and no modifications to the input are allowed.

#include <cassert>
#include <vector>

int countAdjacentDifferences(const std::vector<int>& values); // declaration

int main() {
    // Single element: no pairs, returns 0
    assert(countAdjacentDifferences({7}) == 0);

    // Two identical elements: no differences, returns 0
    assert(countAdjacentDifferences({3, 3}) == 0);

    // Two different elements: both marked, returns 2
    assert(countAdjacentDifferences({3, 4}) == 2);

    // Three different elements: all marked, returns 3
    assert(countAdjacentDifferences({1, 2, 3}) == 3);

    // Middle element duplicates neighbors -> only ends marked
    // [1,2,1]: index0 differs from 1 (marked), index1 differs from both (marked), index2 differs (marked) -> all 3
    assert(countAdjacentDifferences({1, 2, 1}) == 3);

    // [1,2,2,3]: index0 marked; index1 differs from left (marked); index2 differs from right (marked); index3 marked -> all 4
    assert(countAdjacentDifferences({1, 2, 2, 3}) == 4);

    // [1,1,1,2]: only last two differ, index2 and index3 marked -> 2
    assert(countAdjacentDifferences({1, 1, 1, 2}) == 2);

    // [1,2,2,2]: only first two differ, index0 and index1 marked -> 2
    assert(countAdjacentDifferences({1, 2, 2, 2}) == 2);

    // Alternating pattern: every index is adjacent to a differing neighbor -> all marked
    assert(countAdjacentDifferences({1, 2, 1, 2}) == 4);

    // Large all same: 0
    assert(countAdjacentDifferences({5, 5, 5, 5}) == 0);

    return 0;
}

#include <vector>

// Returns the number of indices that are adjacent to at least one element with a different value.
// For a vector v of size n:
// - index 0 is marked if v[0] != v[1] (if n > 1)
// - index n-1 is marked if v[n-2] != v[n-1] (if n > 1)
// - every other index i is marked if v[i] != v[i-1] or v[i] != v[i+1]
int countAdjacentDifferences(const std::vector<int>& values) {
    const int n = static_cast<int>(values.size());
    if (n <= 1) return 0; // no adjacent pairs exist

    int count = 0;

    // Check first element
    if (values[0] != values[1]) ++count;

    // Check middle elements
    for (int i = 1; i < n - 1; ++i) {
        if (values[i] != values[i-1] || values[i] != values[i+1]) {
            ++count;
        }
    }

    // Check last element
    if (values[n-2] != values[n-1]) ++count;

    return count;
}

// The core idea is to iterate over all adjacent pairs in the vector. Whenever two consecutive elements are different, both indices are considered "interesting" and should be counted. A naive approach could use a separate boolean vector (like the snippet does) to avoid double-counting an index that may be part of two different differing pairs (e.g., the middle element in `[1,2,1]` is adjacent to both `1` and `2`, but should only be counted once). However, a more efficient direct approach: instead of storing a boolean array, we can simply iterate and check each index individually. For the first element (index 0), it is marked if it differs from index 1. For the last element (index n-1), it is marked if it differs from index n-2. For any middle element, it is marked if it differs from either its left neighbor or its right neighbor. This avoids extra memory. Alternatively, the boolean array method is equally correct, but the direct method is more space-efficient. The main edge cases are: a vector with one element (returns 0, since there are no adjacent pairs), a vector with all identical elements (returns 0), and a vector where every adjacent pair differs (e.g., `[1,2,3]` — index 0 marked, index 1 marked twice but counted once, index 2 marked, so total 3). Time complexity is O(n) because we examine each element’s adjacent relationships once. Space complexity is O(1) using the direct approach (or O(n) if using a boolean array, but we choose O(1)).
