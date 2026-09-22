/*
Write a C++ function `longestUnlitGap` that takes a positive integer `n` (the number of street lamps), a positive integer `l` (the length of the street), and a `std::vector<long long>` of lamp positions on the street, and returns the length of the longest unlit segment of the street as a `double`. The street starts at position 0 and ends at position `l`. Each lamp illuminates a segment around it: the first lamp at the smallest position illuminates from position 0 up to its own position, the last lamp at the largest position illuminates from its own position up to position `l`, and every other lamp illuminates half the distance to its neighboring lamp on each side (i.e., the midpoint between two adjacent lamps is the boundary of illumination). The function must handle unsorted input, duplicate lamp positions, and any number of lamps, and return the maximum illuminated gap, considering both the edges of the street and the gaps between adjacent lamps. If there are no lamps (n=0), the whole street is dark and the function should return `l` as a double. The output should be correct to a precision of at least 1e-9.
*/
#include <vector>
#include <algorithm>
#include <cmath>

// Given a vector of lamp positions on a street of length l,
// return the length of the longest unlit segment.
// Each internal lamp illuminates half the distance to its neighbors.
double longestUnlitGap(long long n, long long l, const std::vector<long long>& lamps) {
    if (n == 0) return static_cast<double>(l);

    std::vector<long long> pos = lamps;
    std::sort(pos.begin(), pos.end());

    double maxGap = 0.0;

    // Leftmost unlit segment: from street start to first lamp
    maxGap = std::max(maxGap, static_cast<double>(pos[0]));

    // Between lamps: half the distance between consecutive positions
    for (long long i = 1; i < n; ++i) {
        double gap = static_cast<double>(pos[i] - pos[i - 1]) / 2.0;
        maxGap = std::max(maxGap, gap);
    }

    // Rightmost unlit segment: from last lamp to street end
    maxGap = std::max(maxGap, static_cast<double>(l - pos[n - 1]));

    return maxGap;
}
#include <cassert>
#include <cmath>
#include <vector>

double longestUnlitGap(long long n, long long l, const std::vector<long long>& lamps);

int main() {
    // Case 1: Example from code snippet: n=2, l=5, lamps [2,5]
    assert(std::fabs(longestUnlitGap(2, 5, {2, 5}) - 2.0) < 1e-9);

    // Case 2: Single lamp in the middle of street: n=1, l=10, lamp at 5
    assert(std::fabs(longestUnlitGap(1, 10, {5}) - 5.0) < 1e-9);

    // Case 3: No lamps: n=0, l=7
    assert(std::fabs(longestUnlitGap(0, 7, {}) - 7.0) < 1e-9);

    // Case 4: Three lamps evenly spaced on street length 12: positions 2,6,10
    // gaps: left=2, between (6-2)/2=2, (10-6)/2=2, right=2 -> answer 2
    assert(std::fabs(longestUnlitGap(3, 12, {6, 2, 10}) - 2.0) < 1e-9);

    // Case 5: Lamps at both ends: n=2, l=8, positions 0 and 8
    // left=0, between (8-0)/2=4, right=0 -> answer 4
    assert(std::fabs(longestUnlitGap(2, 8, {0, 8}) - 4.0) < 1e-9);

    // Case 6: Duplicate lamps don't break: n=3, l=10, positions 2,2,5
    // left=2, between (2-2)/2=0, (5-2)/2=1.5, right=5 -> answer 5
    assert(std::fabs(longestUnlitGap(3, 10, {2, 2, 5}) - 5.0) < 1e-9);

    // Case 7: Large values: n=2, l=1000000000, positions 1 and 999999999
    // left=1, between (999999998)/2=499999999, right=1 -> answer 499999999
    assert(std::fabs(longestUnlitGap(2, 1000000000LL, {1, 999999999LL}) - 499999999.0) < 1e-6);

    // Case 8: Unsorted input with many lamps: n=5, l=20, positions [20,0,10,5,15]
    // sorted: 0,5,10,15,20 -> gaps: left=0, (5)/2=2.5, (5)/2=2.5, (5)/2=2.5, (5)/2=2.5, right=0 -> answer 2.5
    assert(std::fabs(longestUnlitGap(5, 20, {20,0,10,5,15}) - 2.5) < 1e-9);
}
// The problem reduces to finding the maximum distance along the street that is not illuminated. Since each internal lamp lights half the distance to its neighbors, the unlit gap between two adjacent lamps `a` and `b` (where `a < b`) is exactly `(b - a) / 2.0`. The leftmost lamp at position `v[0]` lights from 0 to `v[0]`, so the unlit segment at the start is `v[0] - 0 = v[0]`. Similarly, the rightmost lamp at position `v[n-1]` lights from `v[n-1]` to `l`, so the unlit segment at the end is `l - v[n-1]`. The answer is the maximum of all these values. We must sort the lamp positions in ascending order to compute adjacent differences correctly. Edge cases: if `n == 0`, return `l`; if there is only one lamp, the only unlit gaps are the two ends: `v[0]` and `l - v[0]`, and we take the larger. Duplicate positions result in a zero gap between them, which never affects the maximum positively. The sorting step dominates the complexity: `O(n log n)` time and `O(n)` space for the copy of the vector if we sort in place, but we can take the vector by value or copy it. The auxiliary space is `O(n)` if we sort a copy, or `O(log n)` for the sort recursion if we modify in place; here we will make a copy to avoid modifying the caller's data.
