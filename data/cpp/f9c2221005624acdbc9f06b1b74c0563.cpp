// Write a C++ free function `maxEnvelopes` that accepts a non-empty vector of pairs of positive integers, where each pair represents the width and height of an envelope, and returns the maximum number of envelopes that can be nested inside one another. Envelope A can fit inside envelope B if and only if both width and height of A are strictly smaller than those of B (i.e., `A.w < B.w && A.h < B.h`). You may reorder the envelopes arbitrarily. The function should handle duplicate dimensions correctly—envelopes with identical width or height cannot nest into each other. Your solution must run in O(n log n) time and O(n) auxiliary space.

The core idea is to transform the two-dimensional nesting problem into a one-dimensional longest increasing subsequence (LIS) problem. First, sort the envelopes by width ascending; when widths are equal, sort by height descending. Why descending height for equal widths? This ensures that when we later apply an LIS algorithm on heights, envelopes with the same width cannot be placed in the same increasing sequence (because their heights are in decreasing order, so they can never be part of a strictly increasing subsequence together). After sorting, we extract the heights array and compute the length of its strictly increasing subsequence using a classic patience-sorting approach: maintain a vector `d` where `d[k]` is the smallest possible tail value of an increasing subsequence of length `k+1`. For each height `x`, if `x > d.back()`, we append `x`; otherwise, we use binary search to find the first element in `d` that is `>= x` and replace it with `x`. The size of `d` at the end is the answer.

Edge cases: If `envelopes` is empty (but the problem specifies non-empty, still handle it gracefully by returning 0). If all envelopes have identical dimensions, the sorted heights will be non-increasing, and `d` will end with size 1. Because the sorting rule handles equal widths correctly, no false nesting is counted. Time complexity is O(n log n) due to sorting and each binary search; space complexity is O(n) for the `d` array and sorting overhead (which uses O(n) extra space).

#include <vector>
#include <algorithm>

// Returns the maximum number of envelopes that can be nested.
// Each envelope is represented as {width, height}.
int maxEnvelopes(std::vector<std::vector<int>>& envelopes) {
    if (envelopes.empty()) return 0;

    // Sort by width ascending, and if equal, by height descending.
    std::sort(envelopes.begin(), envelopes.end(), [](const auto& a, const auto& b) {
        return a[0] < b[0] || (a[0] == b[0] && a[1] > b[1]);
    });

    // Compute length of longest strictly increasing subsequence on heights.
    std::vector<int> d;
    d.push_back(envelopes[0][1]);
    for (size_t i = 1; i < envelopes.size(); ++i) {
        int h = envelopes[i][1];
        if (h > d.back()) {
            d.push_back(h);
        } else {
            auto it = std::lower_bound(d.begin(), d.end(), h);
            *it = h;
        }
    }
    return static_cast<int>(d.size());
}

#include <cassert>
#include <vector>

int maxEnvelopes(std::vector<std::vector<int>>& envelopes);

int main() {
    std::vector<std::vector<int>> e1 = {{5,4},{6,4},{6,7},{2,3}};
    assert(maxEnvelopes(e1) == 3);  // {2,3} -> {5,4} -> {6,7}

    std::vector<std::vector<int>> e2 = {{1,1},{1,1},{1,1}};
    assert(maxEnvelopes(e2) == 1);  // All identical, can't nest

    std::vector<std::vector<int>> e3 = {{10,8},{8,10},{9,9}};
    assert(maxEnvelopes(e3) == 1);  // No strict pair fits

    std::vector<std::vector<int>> e4 = {{1,2},{2,1}};
    assert(maxEnvelopes(e4) == 1);  // One is wider, other is taller

    std::vector<std::vector<int>> e5 = {{1,1},{2,2},{3,3},{4,4}};
    assert(maxEnvelopes(e5) == 4);  // Perfect chain

    std::vector<std::vector<int>> e6 = {{30,50},{12,2},{30,60},{3,4},{12,3}};
    assert(maxEnvelopes(e6) == 3);  // {3,4} -> {12,3}? Wait: {3,4}->{12,3}? No, heights not increasing. Actually {3,4}->{12,2}? No. Let's test carefully: Sorted: (3,4),(12,3),(12,2),(30,50),(30,60). Heights: 4,3,2,50,60. Longest inc subseq: 4,50,60 length 3. So answer 3.
    assert(maxEnvelopes(e6) == 3);

    std::vector<std::vector<int>> e7 = {{1,1}};
    assert(maxEnvelopes(e7) == 1);  // Single envelope

    std::vector<std::vector<int>> e8 = {{2,2},{1,3},{3,1}};
    assert(maxEnvelopes(e8) == 1);  // No strict pair fits? Check: (1,3) and (2,2): 1<2 but 3>2, no. (2,2) and (3,1): 2<3 but 2>1, no. (1,3) and (3,1): 1<3 but 3>1, no. So 1.

    std::vector<std::vector<int>> e9 = {{5,4},{6,4},{6,7},{2,3},{1,2}};
    assert(maxEnvelopes(e9) == 4);  // {1,2}->{2,3}->{5,4}->{6,7}

    return 0;
}
