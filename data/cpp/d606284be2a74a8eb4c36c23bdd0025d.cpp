// Given a list of \( n \) intervals represented as pairs of integers \((l_i, r_i)\), write a C++ function that returns the indices (1-based) of all intervals that intersect with every other interval in the list. Two intervals \( (l_a, r_a) \) and \( (l_b, r_b) \) are said to intersect if \( l_a < r_b \) and \( l_b < r_a \). The input intervals are all valid with \( l_i < r_i \), but may not be sorted. If the total number of intersecting pairs among all intervals is \( S \), an interval qualifies if it participates in exactly \( S \) intersecting pairs (i.e., it intersects with every other interval). The function should return a vector of 1-based indices in increasing order. If no such interval exists, return an empty vector. Note that the total number of intersecting pairs can be up to \( \binom{n}{2} \), and \( n \) can be up to 5000.

The problem asks to find all intervals that intersect with every other interval. A direct brute-force approach checks all pairs of intervals. For each pair that intersects, we increment a counter for both intervals (recording their participation count) and also increment a global counter of total intersecting pairs \( S \). After processing all pairs, any interval with participation count equal to \( S \) must have intersected with every other interval (because each other interval contributes exactly one intersection with it, and there are \( n-1 \) others, but note that \( S \) is the total count of intersecting pairs across all intervals). However, careful: if an interval intersects with all other \( n-1 \) intervals, its participation count is \( n-1 \). So the condition \( s[i] == sum \) where \( sum \) is the total number of intersecting pairs is only valid if the interval also participates in every pair that exists. But if an interval intersects all others, then every pair involving that interval is counted, and there are \( n-1 \) such pairs, so \( s[i] = n-1 \). But \( sum \) is the total number of intersecting pairs in the whole set, which may be larger than \( n-1 \) if other intervals also intersect among themselves. So matching \( s[i] \) to the total sum is not generally correct. Actually the condition in the given snippet is `s[i] == sum` which would require each qualifying interval to participate in every intersecting pair in the entire graph, meaning essentially that every pair of intervals intersects (i.e., the graph is a complete graph). That is a misinterpretation. The correct condition is that an interval intersects with every other interval, i.e., its degree in the intersection graph is \( n-1 \). So the algorithm should count for each interval the number of other intervals it intersects with. Then an interval qualifies if that count equals \( n-1 \). Edge case: when \( n = 1 \), every interval intersects vacuously with all others, so that one interval qualifies. Time complexity: \( O(n^2) \) to check all pairs, space \( O(n) \) for counters. Alternatively, we could use a sweep-line method but \( n \leq 5000 \) makes \( O(n^2) \) acceptable.

#include <vector>
#include <utility>

// Returns 1-based indices of intervals that intersect with every other interval.
std::vector<int> universalIntervals(const std::vector<std::pair<int, int>>& intervals) {
    const int n = static_cast<int>(intervals.size());
    std::vector<int> degree(n, 0);

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            const int li = intervals[i].first;
            const int ri = intervals[i].second;
            const int lj = intervals[j].first;
            const int rj = intervals[j].second;
            if (li < rj && lj < ri) {
                degree[i]++;
                degree[j]++;
            }
        }
    }

    std::vector<int> result;
    for (int i = 0; i < n; ++i) {
        if (degree[i] == n - 1) {
            result.push_back(i + 1);
        }
    }
    return result;
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be declared above.
int main() {
    // Example 1: All three intervals mutually intersect (complete graph).
    std::vector<std::pair<int, int>> intervals1 = {{1, 10}, {2, 3}, {4, 5}};
    std::vector<int> expected1 = {1, 2, 3};
    assert(universalIntervals(intervals1) == expected1);

    // Example 2: One interval intersects all others, others do not intersect each other.
    std::vector<std::pair<int, int>> intervals2 = {{1, 10}, {2, 3}, {4, 5}};
    // Already covered; use a new case.
    std::vector<std::pair<int, int>> intervals2b = {{1, 4}, {2, 5}, {3, 6}};
    std::vector<int> expected2b = {1, 2, 3}; // All pairwise intersect? Check: (1,4) & (2,5): 1<5 && 2<4 true; (1,4)&(3,6): 1<6 && 3<4 true; (2,5)&(3,6): 2<6 && 3<5 true. yes.
    assert(universalIntervals(intervals2b) == expected2b);

    // Example 3: No interval intersects all others.
    std::vector<std::pair<int, int>> intervals3 = {{1, 2}, {3, 4}, {5, 6}};
    std::vector<int> expected3 = {};
    assert(universalIntervals(intervals3) == expected3);

    // Example 4: Single interval (vacuous truth).
    std::vector<std::pair<int, int>> intervals4 = {{1, 2}};
    std::vector<int> expected4 = {1};
    assert(universalIntervals(intervals4) == expected4);

    // Example 5: One interval contains all others but they are disjoint from each other.
    // Interval 1: (1,10) intersects all. Intervals 2: (2,3), 3: (4,5), 4: (6,7)
    std::vector<std::pair<int, int>> intervals5 = {{1, 10}, {2, 3}, {4, 5}, {6, 7}};
    std::vector<int> expected5 = {1};
    assert(universalIntervals(intervals5) == expected5);

    // Example 6: Disjoint intervals, none universal.
    std::vector<std::pair<int, int>> intervals6 = {{1, 2}, {3, 4}, {5, 6}, {7, 8}};
    assert(universalIntervals(intervals6).empty());

    // Example 7: Two intervals that intersect.
    std::vector<std::pair<int, int>> intervals7 = {{1, 3}, {2, 4}};
    std::vector<int> expected7 = {1, 2};
    assert(universalIntervals(intervals7) == expected7);

    // Example 8: Two intervals that do not intersect.
    std::vector<std::pair<int, int>> intervals8 = {{1, 2}, {3, 4}};
    assert(universalIntervals(intervals8).empty());

    // Example 9: Nested intervals all intersect.
    std::vector<std::pair<int, int>> intervals9 = {{1, 10}, {2, 9}, {3, 8}};
    std::vector<int> expected9 = {1, 2, 3};
    assert(universalIntervals(intervals9) == expected9);

    // Example 10: Mixed case.
    std::vector<std::pair<int, int>> intervals10 = {{1, 5}, {2, 6}, {10, 12}};
    std::vector<int> expected10 = {1, 2}; // (1,5) & (2,6) intersect; (1,5) & (10,12) no; (2,6) & (10,12) no.
    assert(universalIntervals(intervals10) == expected10);
}
