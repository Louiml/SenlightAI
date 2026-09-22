/*
Write a C++ function `int maxNestingEnvelopes(const std::vector<std::pair<int, int>>& envelopes)` that, given a list of envelopes where each envelope is represented by a pair `(width, height)`, returns the maximum number of envelopes that can be nested inside each other. An envelope A can fit inside envelope B only if both the width and height of A are strictly less than those of B (i.e., `A.first < B.first` and `A.second < B.second`). The input may be empty, may contain duplicate widths or heights, and may have envelopes that are identical or incomparable. The function must compute the maximum chain length of strictly increasing pairs, and must not modify the input vector (treat it as `const`). The solution should handle large inputs efficiently.
*/
#include <vector>
#include <algorithm>
#include <utility>

// Compute the maximum number of envelopes that can be nested inside each other.
// Envelope A fits inside B if A.first < B.first and A.second < B.second.
int maxNestingEnvelopes(const std::vector<std::pair<int, int>>& envelopes) {
    if (envelopes.empty()) {
        return 0;
    }

    // Make a mutable copy to sort, since input is const.
    std::vector<std::pair<int, int>> env(envelopes);
    std::sort(env.begin(), env.end(),
        [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            if (a.first == b.first) {
                return a.second > b.second; // descending height for equal widths
            }
            return a.first < b.first;      // ascending width
        });

    // Longest increasing subsequence on heights.
    std::vector<int> tails;
    for (const auto& e : env) {
        const int height = e.second;
        auto it = std::lower_bound(tails.begin(), tails.end(), height);
        if (it == tails.end()) {
            tails.push_back(height);
        } else {
            *it = height;
        }
    }

    return static_cast<int>(tails.size());
}
#include <cassert>
#include <vector>
#include <utility>

int maxNestingEnvelopes(const std::vector<std::pair<int, int>>& envelopes);

int main() {
    // Basic nesting chain.
    assert(maxNestingEnvelopes({{5,4},{6,4},{6,7},{2,3}}) == 3); // {2,3} -> {5,4} -> {6,7}

    // Empty input.
    assert(maxNestingEnvelopes({}) == 0);

    // Single envelope.
    assert(maxNestingEnvelopes({{1,1}}) == 1);

    // All identical - cannot nest.
    assert(maxNestingEnvelopes({{3,3},{3,3},{3,3}}) == 1);

    // Same widths but different heights - cannot nest same width.
    assert(maxNestingEnvelopes({{2,5},{2,3},{2,4}}) == 1);

    // Strictly increasing pairs.
    assert(maxNestingEnvelopes({{1,1},{2,2},{3,3},{4,4}}) == 4);

    // Incomparable pairs (width increases but height decreases) - only one chain possible.
    assert(maxNestingEnvelopes({{1,5},{2,4},{3,3},{4,2}}) == 1);

    // Duplicate widths with one valid extension after sorting.
    assert(maxNestingEnvelopes({{1,1},{2,2},{2,1},{3,3}}) == 3); // {1,1} -> {2,2} -> {3,3}

    // Large random-like case: verify known result.
    assert(maxNestingEnvelopes({{4,5},{4,6},{6,7},{2,3},{1,1}}) == 4); // {1,1},{2,3},{4,5},{6,7}

    // No input modification: check original order preserved (function takes const).
    std::vector<std::pair<int,int>> orig = {{3,4},{1,2},{2,3}};
    maxNestingEnvelopes(orig);
    assert(orig[0].first == 3 && orig[0].second == 4);
    assert(orig[1].first == 1 && orig[1].second == 2);
    assert(orig[2].first == 2 && orig[2].second == 3);

    return 0;
}
// The problem reduces to finding the length of the longest increasing subsequence (LIS) in 2D. Sort the envelopes by width in ascending order; for equal widths, sort heights in descending order. Sorting heights descending for equal widths ensures that envelopes with the same width cannot be nested (since width must be strictly less), and this prevents the LIS algorithm from incorrectly selecting two envelopes with the same width. After sorting, we only need the LIS of the heights sequence. For the LIS, we use the classic patience-sorting binary search approach: maintain a vector `tails` where `tails[i]` is the smallest possible tail height for an increasing subsequence of length `i+1`. Iterate through the heights; for each height, use `std::lower_bound` to find the first position in `tails` that is greater than or equal to the target, and either extend or replace. The size of `tails` at the end is the maximum number of envelopes that can be nested. Edge cases: empty input returns 0; single envelope returns 1; envelopes with equal dimensions are correctly excluded from nesting. Time complexity: sorting is O(n log n), LIS binary search per element is O(log n) when using `std::lower_bound` on a vector, so overall O(n log n). Space complexity: O(n) for the `tails` vector (but this is auxiliary, not modifying input). The approach handles duplicates correctly because equal widths are ordered descending, and equal heights (even with different widths) do not extend the LIS beyond what is valid.
