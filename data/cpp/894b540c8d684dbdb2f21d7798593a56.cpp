// Write a C++ function `vector<int> minRemovalsForNonCrossingPairs(const vector<pair<int,int>>& connections)` that takes `n` connections between two parallel lines, where each connection is a pair `(a, b)` meaning point `a` on the left line connects to point `b` on the right line. All `a` values are distinct, and all `b` values are distinct, but they are not necessarily sorted. The function must determine the minimum number of connections to remove so that no two remaining connections cross. Two connections `(a1,b1)` and `(a2,b2)` cross if `a1 < a2` but `b1 > b2`. Return a vector containing the `a` values of the connections that should be removed (in ascending order of `a`). If multiple possible sets of removed connections exist with the same minimum size, output the set with the smallest lexicographic order when sorted by `a`. The input size `n` is at least 1 and at most 100,000, and all coordinates are integers in the range [-10^9, 10^9].
#include <cassert>
#include <vector>
#include <utility>

// Function prototype (include the solution here or appropriate header)
std::vector<int> minRemovalsForNonCrossingPairs(const std::vector<std::pair<int,int>>& connections);

int main() {
    // Already non-crossing: no removals
    assert(minRemovalsForNonCrossingPairs({{1,1},{2,2},{3,3}}) == std::vector<int>{});

    // All crossing: keep one, remove two smallest a's
    assert(minRemovalsForNonCrossingPairs({{1,3},{2,2},{3,1}}) == std::vector<int>({1,2}));

    // Example from typical problem: expect remove a=1 and a=6? Let's build specific
    // (1,8),(2,2),(3,9),(4,1),(5,6),(6,7) -> n=6, LIS length? b:8,2,9,1,6,7 -> LIS: 2,6,7 length 3, remove 3
    // Lexicographically smallest removed set: after sorting by a, find LIS of length 3. Possible LIS: (2,2),(5,6),(6,7) keeps a=2,5,6 removes {1,3,4}. Another LIS: (4,1),(5,6),(6,7) keeps {4,5,6} removes {1,2,3}. The removed {1,2,3} is lexicographically smaller than {1,3,4}. So expected answer {1,2,3}
    std::vector<std::pair<int,int>> conn = {{1,8},{2,2},{3,9},{4,1},{5,6},{6,7}};
    assert(minRemovalsForNonCrossingPairs(conn) == std::vector<int>({1,2,3}));

    // Single connection: nothing to remove
    assert(minRemovalsForNonCrossingPairs({{5,5}}) == std::vector<int>{});

    // Duplicates? Not allowed but test with distinct
    // Negative coordinates
    assert(minRemovalsForNonCrossingPairs({{-3,1},{-2,3},{-1,2}}) == std::vector<int>({-2}));
    // Because b:1,3,2 -> LIS length 2 (1,2 or 1,3) keep -3 and -1 (b:1,2) remove -2; the other LIS keep -3,-2 (b:1,3) remove -1; removed {-2} vs {-1}, lexicographically smaller is {-2}. So correct.

    return 0;
}
#include <vector>
#include <algorithm>
#include <utility>

// Given a list of (a,b) connections, return the a-values of the minimum
// set of connections to remove so that no two remaining connections cross.
// The result is sorted ascending and is the lexicographically smallest among
// all minimum removal sets (with respect to the sorted a-values).
std::vector<int> minRemovalsForNonCrossingPairs(const std::vector<std::pair<int,int>>& connections) {
    int n = (int)connections.size();
    if (n == 0) return {};

    // Sort by a ascending
    std::vector<std::pair<int,int>> sorted = connections;
    std::sort(sorted.begin(), sorted.end());

    // Extract b values in sorted order
    std::vector<int> b(n);
    for (int i = 0; i < n; ++i) {
        b[i] = sorted[i].second;
    }

    // LIS on b values
    std::vector<int> lis;
    std::vector<int> lenV(n); // length of LIS ending at i
    for (int i = 0; i < n; ++i) {
        auto it = std::lower_bound(lis.begin(), lis.end(), b[i]);
        int pos = (int)(it - lis.begin());
        if (pos == (int)lis.size()) {
            lis.push_back(b[i]);
        } else {
            lis[pos] = b[i];
        }
        lenV[i] = pos + 1;
    }

    int lisLen = (int)lis.size();
    int removedCount = n - lisLen;

    // Reconstruct LIS to keep: pick from rightmost possible indices
    std::vector<bool> keep(n, false);
    int need = lisLen;
    int lastB = INT_MAX; // sentinel larger than all b
    for (int i = n - 1; i >= 0 && need > 0; --i) {
        if (lenV[i] == need && b[i] < lastB) {
            keep[i] = true;
            lastB = b[i];
            --need;
        }
    }

    // Removed are all not kept
    std::vector<int> result;
    for (int i = 0; i < n; ++i) {
        if (!keep[i]) {
            result.push_back(sorted[i].first);
        }
    }
    // Since we scanned sorted by a, result is already ascending.
    return result;
}
// The problem reduces to finding the longest increasing subsequence (LIS) of the `b` values after sorting the connections by `a`. Since connections cross exactly when the `b` sequence is not strictly increasing, the maximum number of non-crossing connections equals the length of the LIS of `b` values (after sorting by `a`). Thus, the minimum removals = `n - LIS_length`. To recover which connections to remove, we need to construct one LIS and then remove all connections not in that LIS. However, since the task asks for the lexicographically smallest set of removed `a` values, we must be careful: if multiple LIS exist, we should pick the one whose complement (removed set) sorts to the smallest lexicographic order. This is tricky; a simpler equivalent: among all LIS, we want the one that includes as many small `a` values as possible? Actually, since we remove the complement, we want to keep a set of `a` values that are as large as possible lexicographically? Wait: the removed set sorted ascending should be lexicographically smallest. So we want to keep the LIS that maximizes the kept set in a way that the removed set becomes smallest. This is equivalent to constructing the LIS that is lexicographically largest in terms of kept `a` values (since removing the smallest numbers yields the smallest removed set). But care: lexicographic comparison of removed vectors directly is complex. A standard approach: after building LIS lengths, we reconstruct an LIS by scanning from right to left, picking the first occurrence (from right) of each length that satisfies the LIS condition, which yields a specific LIS but not necessarily the one giving lexicographically smallest removal. For the contest-style problem, usually the intended solution picks any valid minimum removal set, and the expected answer is unique? But the task explicitly asks for lexicographically smallest removed set. Since coordinates are distinct, we can reconstruct the lexicographically smallest removed set by a greedy approach: among all possible LIS, we want to include as many smaller `a` values as possible? Actually, if we keep a smaller `a` value, it is not removed, so the removed set becomes larger (since it misses that small number). To minimize the removed set lexicographically, we want the smallest element of removed set to be as large as possible. That means we want to include all `a` values up to some point if possible, but LIS constraints limit. The safest is to construct the LIS greedily from the end: we choose the rightmost element with the largest possible `b` for each length, which tends to keep larger indices (hence larger `a` after sorting) for later positions. However, to guarantee lexicographically smallest removed vector, we can do the following: after sorting by `a`, we compute `lenV[i]` = LIS length ending at `i`. Then we reconstruct the kept set by scanning from `i=n-1` down to `0`, choosing the first `i` (from right) with `lenV[i] == current_length` and also with `b[i] < next_b` (where `next_b` is the `b` of the already chosen later element). This gives the lexicographically largest kept sequence of `a` (because we pick from right, meaning larger `a` indices, and when ties, the rightmost has larger `a`). That yields the smallest removed set lexicographically because the removed set consists of all other `a` values, and by keeping the largest possible `a` values in the LIS, the removed set will contain the smallest possible numbers, making it lexicographically smallest. This is correct because the removed set sorted ascending compares primarily on its first element; having a larger first element in the kept set means the removed set's first element is smaller. Conversely, picking rightmost ensures that we skip as few as possible.
//
// Time complexity: `O(n log n)` for sorting and LIS using binary search. Space complexity: `O(n)`.
//
// Edge cases: all connections already non-crossing (LIS length = n, removed vector empty). Duplicate `a` or `b` are not present per statement, but we assume distinct. Negative coordinates fine.
