Write a C++ function `countUnmatchedPairs` that takes a vector of integers `tbl`, an integer `s`, and returns the number of pairs `(i, j)` with `0 <= i < j < tbl.size()` such that `tbl[i] + tbl[j] == s`. The input may contain duplicate values and the vector is not necessarily sorted. The function must handle up to `10^5` elements and values up to `10^5`. Return the count as a `long long` (since the maximum pairs can be large). Use a hash map (e.g., `std::unordered_map`) to efficiently count frequencies. The function should be `const`-correct with respect to its parameters.
The naive approach checks all pairs, which is `O(n^2)`. Instead, we can use a frequency map: iterate through the vector once, for each element `x`, check how many times the complement `s - x` has been seen before; add that count to the answer. Then increment the frequency of `x`. This works because we only count pairs where the second element appears after the first, avoiding double counting. For example, if `s = 6` and the vector is `[3, 3, 3]`, the first `3` sees zero occurrences of `3`, the second sees one, the third sees two → total 3 pairs. Edge cases: `s` can be any integer, and `x` can be negative? The problem states values are up to `10^5`, but may include negatives? Not specified, but we should handle any integer. The modulo? Not needed. Complexity: `O(n)` time, `O(n)` space for the map, where `n` is the vector size. The answer may exceed `int` range, so use `long long` (max pairs for `n=1e5` is about `5e9`, within `long long`).
#include <vector>
#include <unordered_map>

// Count pairs (i,j) with i<j such that tbl[i] + tbl[j] == s.
long long countUnmatchedPairs(const std::vector<int>& tbl, int s) {
    std::unordered_map<int, long long> freq;
    long long result = 0;
    for (int x : tbl) {
        int complement = s - x;
        auto it = freq.find(complement);
        if (it != freq.end()) {
            result += it->second;
        }
        freq[x]++;
    }
    return result;
}
#include <cassert>
#include <vector>

// Assume solution function is declared above.
long long countUnmatchedPairs(const std::vector<int>& tbl, int s);

int main() {
    // Basic cases
    assert(countUnmatchedPairs({1, 2, 3, 4}, 5) == 2); // (1,4) and (2,3)
    assert(countUnmatchedPairs({5, 5, 5}, 10) == 3);
    assert(countUnmatchedPairs({1, 1, 1, 1}, 2) == 6);
    // Duplicate complements with multiple pairs
    assert(countUnmatchedPairs({0, 0, 0, 0}, 0) == 6);
    // Singles
    assert(countUnmatchedPairs({7}, 7) == 0);
    // No matches
    assert(countUnmatchedPairs({1, 2, 3}, 100) == 0);
    // Negative numbers
    assert(countUnmatchedPairs({-1, 2, -3, 4, 3}, 1) == 2); // (-1,2) and (-3,4)
    // Large vector (test performance)
    std::vector<int> big(100000, 1);
    assert(countUnmatchedPairs(big, 2) == 4999950000LL);
    return 0;
}
