// Write a standalone C++ function `int maxNestedPairs(vector<vector<int>>& pairs)` that, given a list of pairs where each pair is `{width, height}`, returns the maximum number of pairs that can be nested inside each other. A pair `(w1, h1)` can be placed inside another pair `(w2, h2)` only if both `w1 < w2` and `h1 < h2` (strict inequality). You may reorder the list freely, but each pair can be used at most once, and nesting is transitive (i.e., if A fits in B and B fits in C, then A fits in C). The function should handle an empty input (return 0) and pairs with duplicate widths or heights correctly.

The problem is a classic longest increasing subsequence (LIS) variant in 2D. First, sort all pairs by width in ascending order; if two pairs have the same width, sort them by height in **descending** order. The descending height for equal widths is critical: it prevents two pairs with the same width from being counted as nested (since strict `w1 < w2` is required, equal widths must not be considered in the increasing sequence). After sorting, we need to find the length of the longest strictly increasing subsequence of heights. A straightforward O(n²) dynamic programming approach works: initialize `dp[i] = 1` for all i, then for each i, scan all j < i, and if `heights[i] > heights[j]`, update `dp[i] = max(dp[i], dp[j] + 1)`. The answer is the maximum value in `dp`. Edge cases: empty input returns 0; single pair returns 1; duplicate heights across different widths are allowed (since widths differ, nesting is possible only if both dimensions strictly increase). The time complexity is O(n log n) for sorting plus O(n²) for the DP, overall O(n²). Space complexity is O(n) for the DP array and the sorted copy (if we mutate the input, we save extra space). Using the descending order for equal widths is essential to avoid incorrectly counting pairs with the same width as nesting.

#include <vector>
#include <algorithm>
#include <cstddef>

// Returns the maximum number of pairs that can be nested inside each other.
// Each pair is {width, height}. Nesting requires both width and height to be strictly greater.
int maxNestedPairs(std::vector<std::vector<int>>& pairs) {
    if (pairs.empty()) return 0;

    // Sort by width ascending; for equal width, sort by height descending
    // to prevent counting same-width pairs in the increasing sequence.
    std::sort(pairs.begin(), pairs.end(),
              [](const std::vector<int>& a, const std::vector<int>& b) {
                  if (a[0] != b[0]) return a[0] < b[0];
                  return a[1] > b[1];
              });

    const std::size_t n = pairs.size();
    std::vector<int> dp(n, 1);  // dp[i] = longest chain ending at index i
    int maxLen = 1;

    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < i; ++j) {
            if (pairs[i][0] > pairs[j][0] && pairs[i][1] > pairs[j][1]) {
                dp[i] = std::max(dp[i], dp[j] + 1);
            }
        }
        maxLen = std::max(maxLen, dp[i]);
    }
    return maxLen;
}

#include <cassert>
#include <vector>

// Function declaration (in practice, include the header or paste the solution above)
int maxNestedPairs(std::vector<std::vector<int>>& pairs);

int main() {
    // Basic cases
    std::vector<std::vector<int>> v1 = {{5,4},{6,4},{6,7},{2,3}};
    assert(maxNestedPairs(v1) == 3);  // [2,3] -> [5,4] -> [6,7]

    std::vector<std::vector<int>> v2 = {{1,1},{1,1},{1,1}};
    assert(maxNestedPairs(v2) == 1);  // all identical, can only take one

    std::vector<std::vector<int>> v3 = {};
    assert(maxNestedPairs(v3) == 0);  // empty

    std::vector<std::vector<int>> v4 = {{3,4}};
    assert(maxNestedPairs(v4) == 1);  // single

    // Edge case: same width, different heights cannot nest together
    std::vector<std::vector<int>> v5 = {{2,3},{2,4},{2,5}};
    assert(maxNestedPairs(v5) == 1);  // width equal, strict inequality fails

    // Edge case: same height, different widths also cannot nest
    std::vector<std::vector<int>> v6 = {{1,5},{2,5},{3,5}};
    assert(maxNestedPairs(v6) == 1);  // height equal, strict inequality fails

    // Mixed case with duplicates and correct nesting
    std::vector<std::vector<int>> v7 = {{1,2},{2,3},{2,4},{3,5},{4,5}};
    // Longest chain: [1,2] -> [2,3] -> [3,5] (or [1,2] -> [2,4] -> [3,5]) length 3
    assert(maxNestedPairs(v7) == 3);

    return 0;
}
