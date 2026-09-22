Write a C++ function `int maxServed(int n, const vector<pair<int,int>>& domestic, const vector<pair<int,int>>& international)` that, given a total of `n` available airport gates, a list of domestic flight intervals `(arrival, departure)` and a list of international flight intervals, returns the maximum total number of flights that can be served by partitioning the gates into `i` gates for domestic flights and `n-i` gates for international flights (for `i = 0..n`). A gate can serve multiple flights sequentially as long as the next flight’s arrival time is strictly greater than the previous flight’s departure time (i.e., non-overlapping with a positive gap). Flights assigned to a gate must be scheduled in non-decreasing order of arrival; no two flights can share the same gate if their intervals overlap. The function must handle empty lists, duplicate times, and large inputs efficiently. Example: with `n=2`, domestic intervals `{(1,5),(3,6)}`, international `{(2,4),(5,7)}`, assigning 1 gate to each can serve 2+2=4 flights, but assigning 2 gates to domestic and 0 to international gives 2, and 0 to domestic and 2 to international gives 2, so maximum is 4. The algorithm should compute, for a given set of intervals and a gate count `k`, the maximum number of flights that can be served using at most `k` gates, then combine results for all splits.

The core problem reduces to: given a set of flight intervals and `k` gates, what is the maximum number of flights that can be served? This is a greedy scheduling problem. Sort all intervals by arrival time. Maintain a multiset (or priority queue) of the departure times of the last flight assigned to each currently occupied gate. For each flight in sorted order, if there exists a gate whose last departure time is strictly less than the new flight’s arrival, we can reuse that gate; choose the gate with the smallest such departure time (to keep future gates as late as possible). If no such gate exists and we have fewer than `k` gates already in use, open a new gate. Otherwise, skip this flight. This greedy is optimal because it minimizes the departure times on each gate, maximizing future capacity. To compute the maximum total for all splits, precompute `dp[i]` = max flights served by `i` gates for domestic, and `ip[i]` for international, for `i=0..n`. Then answer is `max_{i=0..n} dp[i] + ip[n-i]`. Edge cases: empty interval list yields 0 for any gate count; duplicate arrival/departure times require using `<` for gate reuse, not `<=`; if `k=0`, no flights can be served. Time complexity: for each type, processing `m` flights with a multiset takes `O(m log m)` for sorting plus `O(m log k)` for operations, so overall `O(m log m)` per type. Precomputing for all `k` from 0 to n naively would be `O(n*m log m)` which is too slow; instead, run the greedy once per type and note that the greedy process for `k` gates is equivalent to running the greedy with unlimited gates but only allowing `k` active gates; a better approach is to simulate the greedy once with unlimited gates and record how many flights are served with each additional gate that is opened. Specifically, run the greedy on the sorted list without limit, but whenever we need to open a new gate, increment a counter; the number of flights served with exactly `j` gates is the total flights served up to the moment the `j`-th gate is opened. Then `dp[j]` is the cumulative count after opening `j` gates. This yields `O(m log m)` per type, and we can build `dp` and `ip` arrays of length `n+1`. Space complexity is `O(m)` for the intervals and `O(n)` for the answer arrays.

#include <vector>
#include <algorithm>
#include <set>
#include <cstdint>

// Helper: compute max flights served by exactly k gates for a list of intervals.
static std::vector<int> maxServedByGates(const std::vector<std::pair<int,int>>& flights) {
    std::vector<std::pair<int,int>> sorted = flights;
    std::sort(sorted.begin(), sorted.end());
    std::multiset<int> gateDepartures;
    std::vector<int> cumulative;
    int served = 0;
    for (const auto& f : sorted) {
        int arr = f.first, dep = f.second;
        auto it = gateDepartures.lower_bound(arr); // first dep >= arr
        if (it != gateDepartures.begin()) {
            // There is a gate with dep < arr; reuse the latest such gate.
            --it;
            gateDepartures.erase(it);
            gateDepartures.insert(dep);
            ++served;
        } else if (gateDepartures.size() + 1 <= static_cast<size_t>(gateDepartures.size()) + 1) {
            // Open a new gate (no limit here; we track how many gates used).
            gateDepartures.insert(dep);
            ++served;
        }
        // The condition above is always true; we just open a new gate when no reuse is possible.
        // Record served count at each gate count (1-based).
        if (cumulative.size() < gateDepartures.size()) {
            cumulative.push_back(served);
        }
    }
    return cumulative; // cumulative[j] = max flights served with j+1 gates.
}

// Main solution function.
int maxServed(int n, const std::vector<std::pair<int,int>>& domestic,
              const std::vector<std::pair<int,int>>& international) {
    auto dom = maxServedByGates(domestic);
    auto inter = maxServedByGates(international);
    // Pad arrays to length n+1 (index 0..n).
    std::vector<int> dp(n+1, 0), ip(n+1, 0);
    for (size_t i = 0; i < dom.size() && i <= static_cast<size_t>(n); ++i) dp[i+1] = dom[i];
    for (size_t i = 0; i < inter.size() && i <= static_cast<size_t>(n); ++i) ip[i+1] = inter[i];
    // dp[i] = max flights with i gates; ip[i] similar.
    int ans = 0;
    for (int i = 0; i <= n; ++i) {
        ans = std::max(ans, dp[i] + ip[n-i]);
    }
    return ans;
}

#include <cassert>
#include <vector>
#include <utility>

int maxServed(int, const std::vector<std::pair<int,int>>&, const std::vector<std::pair<int,int>>&);

int main() {
    // Empty lists
    assert(maxServed(3, {}, {}) == 0);
    // Only domestic
    std::vector<std::pair<int,int>> d1 = {{1,2}, {3,4}};
    assert(maxServed(2, d1, {}) == 2); // 2 gates domestic serves 2, 0 intl
    // Only international
    std::vector<std::pair<int,int>> i1 = {{5,6}};
    assert(maxServed(2, {}, i1) == 1);
    // Mixed example from problem statement
    std::vector<std::pair<int,int>> d2 = {{1,5}, {3,6}};
    std::vector<std::pair<int,int>> i2 = {{2,4}, {5,7}};
    assert(maxServed(2, d2, i2) == 4);
    // Overlap requires strict gap
    std::vector<std::pair<int,int>> d3 = {{1,3}, {3,5}}; // cannot share same gate
    assert(maxServed(1, d3, {}) == 1);
    // Large n with few flights
    assert(maxServed(10, {{1,2}}, {{3,4}}) == 2);
    // Many gates but few flights
    assert(maxServed(100, {{1,10}}, {{20,30}}) == 2);
    // Sequential reuse with many flights
    std::vector<std::pair<int,int>> d4 = {{1,2}, {3,4}, {5,6}};
    assert(maxServed(1, d4, {}) == 3);
    // Interleaved times
    std::vector<std::pair<int,int>> d5 = {{1,4}, {2,3}, {5,6}};
    std::vector<std::pair<int,int>> i5 = {{2,5}, {4,7}};
    // Optimal split: i=1 domestic, 1 intl -> domestic serves 2 (1-4,5-6), intl serves 1 (2-5? no because 4-7 overlaps? Actually intl sorted: (2,5),(4,7): with 1 gate, serve (2,5) then cannot use 4-7; so 1. total=3). i=2 domestic,0 intl -> domestic serves 2 (1-4 and 5-6), total=2. i=0 domestic,2 intl -> intl serves 1 (overlap). So max=3.
    assert(maxServed(2, d5, i5) == 3);
    return 0;
}
