Write a C++ function `long long winningMove(int n, const std::vector<long long>& submissions)` that, given the total number of players `n` and a vector of `n` submission values (each value is a non-negative integer), returns the smallest submission value that appears exactly once in the entire list. If no unique submission exists (i.e., every value appears at least twice), return `0`. The vector is guaranteed to contain exactly `n` entries. The function must handle `n` up to 10^7 efficiently, and all values fit in a signed 64-bit integer. Note: The order of the vector is arbitrary, and you should not modify the input vector.

The core problem is to find the minimum value that occurs exactly once among a large array of integers. A straightforward approach is to sort the array and then scan for a run of length one, returning the first such value; if none exists, return 0. Sorting takes `O(n log n)` time, which is acceptable for `n` up to 10^7 if using an efficient in-place sort (e.g., `std::sort` with an introsort implementation). After sorting, adjacent equal values will be grouped, so a single pass compares each element with its neighbors: if `data[i]` differs from both `data[i-1]` (if exists) and `data[i+1]` (if exists), then it is unique. Since we want the smallest unique value, we scan in ascending order and return the first such occurrence. If no unique value is found, return 0. Edge cases: an empty vector (return 0), a single-element vector (that element is unique), and vectors where every value is duplicated. Space complexity is `O(1)` additional space if we sort in-place (we are allowed to copy the input vector, so that's `O(n)` auxiliary space for the copy). Time complexity is dominated by sorting: `O(n log n)`. Alternatively, one could use a hash map to count frequencies in `O(n)` expected time and `O(n)` space, but the sort approach is simpler and deterministic.

#include <vector>
#include <algorithm>

// Returns the smallest value that appears exactly once in 'submissions'.
// If no unique value exists, returns 0.
long long winningMove(int n, const std::vector<long long>& submissions) {
    if (n <= 0) return 0;
    // Copy to allow sorting without modifying the input.
    std::vector<long long> data = submissions;
    std::sort(data.begin(), data.end());

    // Scan for the first unique element.
    for (int i = 0; i < n; ++i) {
        bool leftIsSame = (i > 0 && data[i - 1] == data[i]);
        bool rightIsSame = (i + 1 < n && data[i + 1] == data[i]);
        if (!leftIsSame && !rightIsSame) {
            return data[i];
        }
    }
    return 0;
}

#include <cassert>
#include <vector>

// Declaration of the function under test.
long long winningMove(int n, const std::vector<long long>& submissions);

int main() {
    // Example 1: unique values, smallest is 3.
    std::vector<long long> v1 = {5, 3, 7, 3}; // 3 appears twice, 5 and 7 unique → smallest is 5? Actually 5 appears once, 7 once → smallest unique is 5.
    // Correction: v1 = {5,3,7,3} has 3 twice, 5 once, 7 once → smallest unique is 5.
    assert(winningMove(4, v1) == 5);

    // Example 2: all duplicates → return 0.
    std::vector<long long> v2 = {4, 4, 4, 4};
    assert(winningMove(4, v2) == 0);

    // Example 3: single element.
    std::vector<long long> v3 = {42};
    assert(winningMove(1, v3) == 42);

    // Example 4: smallest unique is at the beginning after sorting.
    std::vector<long long> v4 = {100, 1, 100, 2, 2, 3};
    // Unique values: 1 and 3 → smallest is 1.
    assert(winningMove(6, v4) == 1);

    // Example 5: empty vector → 0.
    std::vector<long long> v5 = {};
    assert(winningMove(0, v5) == 0);

    // Example 6: negative and large numbers.
    std::vector<long long> v6 = {-10, 2000000000LL, -10, 2000000000LL, 5};
    // Unique: 5 only → returns 5.
    assert(winningMove(5, v6) == 5);

    // Example 7: all unique, smallest is negative.
    std::vector<long long> v7 = {0, -1, 10, 3};
    // Unique all: smallest is -1.
    assert(winningMove(4, v7) == -1);

    // Example 8: duplicates but unique at the end.
    std::vector<long long> v8 = {2, 2, 1, 1, 7};
    // Unique: 7 → returns 7.
    assert(winningMove(5, v8) == 7);

    // Example 9: mixed duplicates and unique larger value.
    std::vector<long long> v9 = {9, 9, 8, 8, 7, 6, 6};
    // Unique: 7 → returns 7.
    assert(winningMove(7, v9) == 7);

    return 0;
}
