/*
Given \(n\) intervals \([l_i, r_i]\) (with \(1 \le l_i \le r_i \le 10^9\)) and \(n\) test queries each containing all intervals, write a C++ function `vector<long long> countQualifyingIntervals(const vector<pair<long long,long long>>& intervals)` that, for each interval \(i\), counts the number of integer points that belong to **exactly one** interval among all \(n\) intervals, excluding the points inside interval \(i\) itself if it is a duplicate (i.e., if there are at least two identical intervals, then any point covered by that interval is covered by multiple intervals, so it contributes 0). More formally, for each interval \(i\), output the size of the set \(\{ x \in \mathbb{Z} \mid x \in \text{some interval } j,\, j \ne i, \text{ and } x \text{ is not in any other interval besides that one}\}\). If no such points exist, output 0. Note: The intervals are considered closed integers; for example, \([2,4]\) covers {2,3,4}. Duplicate intervals are possible. The intervals are 0-indexed, and the output order must match the input order.
*/

#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <cstdint>

// For each interval, count points in the common intersection of all
// intervals that fully contain it, excluding points of the interval itself.
// Returns a vector of counts in the original input order.
std::vector<long long> countCommonSuperPoints(
    const std::vector<std::pair<long long, long long>>& intervals) {
    using ll = long long;
    const int n = static_cast<int>(intervals.size());

    // Prepare triples (l, r, originalIndex)
    std::vector<std::array<ll, 3>> data(n);
    for (int i = 0; i < n; ++i) {
        data[i] = {intervals[i].first, intervals[i].second, i};
    }

    std::vector<ll> minRight(n, -1);
    std::vector<ll> maxLeft(n, -1);

    // First sweep: sort by (l asc, r desc)
    std::vector<std::array<ll, 3>> byLeft = data;
    std::sort(byLeft.begin(), byLeft.end(),
              [](const auto& a, const auto& b) {
                  if (a[0] != b[0]) return a[0] < b[0];
                  return a[1] > b[1];
              });

    std::set<ll> rights;
    for (const auto& entry : byLeft) {
        ll l = entry[0], r = entry[1];
        int idx = static_cast<int>(entry[2]);
        auto it = rights.lower_bound(r);  // smallest right >= r
        if (it != rights.end()) {
            minRight[idx] = *it;
        }
        rights.insert(r);
    }

    // Second sweep: sort by (r desc, l asc)
    std::vector<std::array<ll, 3>> byRight = data;
    std::sort(byRight.begin(), byRight.end(),
              [](const auto& a, const auto& b) {
                  if (a[1] != b[1]) return a[1] > b[1];
                  return a[0] < b[0];
              });

    std::set<ll> lefts;
    for (const auto& entry : byRight) {
        ll l = entry[0], r = entry[1];
        int idx = static_cast<int>(entry[2]);
        auto it = lefts.upper_bound(l);  // first left > l
        if (it != lefts.begin()) {
            --it;
            maxLeft[idx] = *it;  // largest left <= l
        }
        lefts.insert(l);
    }

    // Count duplicates
    std::map<std::pair<ll, ll>, int> freq;
    for (const auto& p : intervals) {
        freq[p]++;
    }

    std::vector<ll> result(n, 0);
    for (int i = 0; i < n; ++i) {
        ll l = intervals[i].first, r = intervals[i].second;
        if (freq[{l, r}] >= 2 || minRight[i] == -1 || maxLeft[i] == -1) {
            result[i] = 0;
        } else {
            ll intersectionLength = minRight[i] - maxLeft[i] + 1;
            ll selfLength = r - l + 1;
            result[i] = intersectionLength - selfLength;
        }
    }

    return result;
}

#include <cassert>
#include <vector>
#include <utility>
#include <cstdint>

// The solution function is expected to be declared above.
// Here we write the test harness.

int main() {
    // 1) Single interval -> no containers -> 0
    {
        std::vector<std::pair<long long, long long>> v = {{1,3}};
        std::vector<long long> res = countCommonSuperPoints(v);
        assert(res == std::vector<long long>{0});
    }

    // 2) Two disjoint intervals -> each has no container -> 0
    {
        std::vector<std::pair<long long, long long>> v = {{1,2}, {4,5}};
        std::vector<long long> res = countCommonSuperPoints(v);
        assert(res == std::vector<long long>{0, 0});
    }

    // 3) Two overlapping but neither contains the other -> 0 each
    {
        std::vector<std::pair<long long, long long>> v = {{1,3}, {2,4}};
        std::vector<long long> res = countCommonSuperPoints(v);
        assert(res == std::vector<long long>{0, 0});
    }

    // 4) Nested intervals: [1,10], [2,8], [3,5]
    // For [2,8]: container [1,10] -> extra {1,10} => 2
    // For [3,5]: containers [1,10] and [2,8] -> intersection [2,8] -> extra {2,9}? wait [2,8] length 7 minus [3,5] length 3 = 4
    // But let's compute: [1,10] contains all; [2,8] contains [3,5]; intersection of containers of [3,5] = [max(1,2)=2, min(10,8)=8] = [2,8], length 7, minus [3,5] length 3 = 4.
    // For [1,10]: no container -> 0
    {
        std::vector<std::pair<long long, long long>> v = {{1,10}, {2,8}, {3,5}};
        std::vector<long long> res = countCommonSuperPoints(v);
        assert(res == std::vector<long long>{0, 2, 4});
    }

    // 5) Duplicate intervals -> both 0
    {
        std::vector<std::pair<long long, long long>> v = {{1,5}, {1,5}};
        std::vector<long long> res = countCommonSuperPoints(v);
        assert(res == std::vector<long long>{0, 0});
    }

    // 6) More complex: [1,7], [2,6], [3,5] -> [1,7] 0, [2,6] container [1,7] -> extra {1,7} => 2, [3,5] containers [1,7],[2,6] -> intersection [2,6] length 5 minus [3,5] length 3 = 2
    {
        std::vector<std::pair<long long, long long>> v = {{1,7}, {2,6}, {3,5}};
        std::vector<long long> res = countCommonSuperPoints(v);
        assert(res == std::vector<long long>{0, 2, 2});
    }

    // 7) Empty input -> empty output
    {
        std::vector<std::pair<long long, long long>> v;
        std::vector<long long> res = countCommonSuperPoints(v);
        assert(res.empty());
    }

    return 0;
}

// We need, for each interval \(i\), the set of all intervals \(j\) such that \(l_j \le l_i\) and \(r_j \ge r_i\). The intersection of these containing intervals is \([\max(l_j), \min(r_j)]\), and the desired count is the length of that intersection minus the length of interval \(i\).  
// We can compute the maximum left and minimum right among containing intervals using two sweeps with sorted orders and a `std::set`.
//
// - **First sweep:** Sort intervals by left ascending, and for equal left, by right descending. Traverse in this order. Maintain a set of right endpoints of previously processed intervals (which all have left ≤ current left, and if left equal, right ≥ current right). For the current interval, the smallest right ≥ current right in the set (found via `lower_bound`) is the minimum right among all containing intervals; store it as `minRight[i]` (or `-1` if none). Then insert current right.
// - **Second sweep:** Sort intervals by right descending, and for equal right, by left ascending. Traverse in this order. Maintain a set of left endpoints of previously processed intervals (which all have right ≥ current right, and if right equal, left ≤ current left). For the current interval, the largest left ≤ current left in the set (found via `upper_bound` and taking the previous iterator) is the maximum left among all containing intervals; store it as `maxLeft[i]` (or `-1` if none). Insert current left.
//
// Then for each interval, if either stored value is `-1`, or the interval is a duplicate (use a frequency map), the answer is `0`. Otherwise, the answer is `minRight[i] - maxLeft[i] + 1 - (r_i - l_i + 1)`, which is always non-negative because `maxLeft[i] ≤ l_i` and `minRight[i] ≥ r_i`.
//
// **Time complexity:** \(O(n \log n)\) due to two sorts and set operations. **Space complexity:** \(O(n)\).
//
// **Edge cases:** Single interval → no containers → `0`. Duplicate intervals → `0`. Large coordinates → use `long long`. All intervals overlapping but not containing each other appropriately → some answers may be `0`. Empty input → return empty vector.
