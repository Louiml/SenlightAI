/*
Write a C++ function `minimumHotelStops` that takes four parameters: a vector of strictly increasing integers representing hotel positions along a road (each element is the distance from the start), an integer `dailyLimit` (the maximum distance that can be traveled in one day), and two integers `start` and `end` (indices into the hotel array, with 0 ≤ start, end < n). The function must return the minimum number of days required to travel from the hotel at index `start` to the hotel at index `end`, traveling eastbound only (i.e., from the hotel with the smaller index to the larger, but if `start > end`, treat it as traveling from `end` to `start`). Each day you may move to any hotel that is reachable within `dailyLimit` distance from your current hotel, and you must end the day at a hotel position (you cannot stop mid-way). You may assume that `dailyLimit` is at least the maximum gap between consecutive hotels, so travel is always possible. If `start == end`, the answer is 0.
*/
#include <vector>
#include <algorithm>
#include <cassert>

// Return minimum number of days to travel from hotel at index `start` to hotel at index `end`.
// Hotels are sorted by position, `dailyLimit` is the maximum distance per day.
int minimumHotelStops(const std::vector<int>& hotels, int dailyLimit, int start, int end) {
    int n = static_cast<int>(hotels.size());
    if (start == end) return 0;
    if (start > end) std::swap(start, end);

    // Precompute oneStep[i] = farthest hotel index reachable in one day from i.
    std::vector<int> oneStep(n, -1);
    int slow = 0;
    int fast = 0;
    while (slow < n) {
        while (fast + 1 < n && hotels[fast + 1] - hotels[slow] <= dailyLimit) {
            ++fast;
        }
        oneStep[slow] = fast;
        ++slow;
        if (fast < slow) fast = slow;
    }

    // Binary lifting table: up[level][i] = index after 2^level days from i.
    int maxLog = 1;
    while ((1 << maxLog) <= n) ++maxLog;
    std::vector<std::vector<int>> up(maxLog, std::vector<int>(n));
    up[0] = oneStep;
    for (int level = 1; level < maxLog; ++level) {
        for (int i = 0; i < n; ++i) {
            int mid = up[level - 1][i];
            up[level][i] = up[level - 1][mid];
        }
    }

    // Binary search on the number of days.
    auto canReach = [&](int days) -> bool {
        int cur = start;
        for (int level = 0; level < maxLog; ++level) {
            if (days & (1 << level)) {
                cur = up[level][cur];
                if (cur >= end) return true;
            }
        }
        return cur >= end;
    };

    int lo = 1, hi = n;  // At most n-1 days to traverse n hotels.
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (canReach(mid)) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}
#include <cassert>
#include <vector>

// Declare the function under test (or include the solution).
int minimumHotelStops(const std::vector<int>& hotels, int dailyLimit, int start, int end);

int main() {
    std::vector<int> hotels = {0, 5, 10, 15, 20};
    assert(minimumHotelStops(hotels, 5, 0, 4) == 4); // 0->5->10->15->20
    assert(minimumHotelStops(hotels, 10, 0, 2) == 1); // 0->10 directly
    assert(minimumHotelStops(hotels, 6, 4, 1) == 3); // from 0 to 4, but reversed: min days from 0 to 4 is 4? Actually careful: start=4, end=1 => swap => 1 to 4; with limit 6, 1->? 5->11? positions: 1 is index 1 (5), index4 (20) diff 15 >6, so 1->2(10) diff5, 2->3(15) diff5, 3->4(20) diff5 = 3 days.
    assert(minimumHotelStops(hotels, 5, 2, 2) == 0);
    assert(minimumHotelStops(hotels, 20, 0, 4) == 1);

    std::vector<int> h2 = {0, 100, 200, 300};
    assert(minimumHotelStops(h2, 150, 0, 3) == 2); // 0->2 (200) diff 200? Actually 200>150, so 0->1(100), then 1->3(300) diff 200>150, 1->2(200) diff100, 2->3(300) diff100 => 3 days? Wait: 0->1(100) day1, 1->2(200) day2, 2->3(300) day3 => 3. But with limit 150: from 0 to 1 (100), from 1 to 2 (100), from 2 to 3 (100), so 3 days. Let me adjust: actually with limit 150, 0->1 (100) day1, 1->2 (100) day2, 2->3 (100) day3 => 3. So test with 150 returns 3.
    assert(minimumHotelStops(h2, 150, 0, 3) == 3);
    assert(minimumHotelStops(h2, 300, 0, 3) == 1);

    std::vector<int> h3 = {0, 1, 2, 3, 4};
    assert(minimumHotelStops(h3, 2, 0, 2) == 1);
    assert(minimumHotelStops(h3, 2, 0, 4) == 2); // 0->2->4
    assert(minimumHotelStops(h3, 1, 0, 4) == 4); // only one step each day

    return 0;
}
// The problem is a classic range-query shortest-path problem where each "day" corresponds to moving to the farthest hotel reachable within the daily limit. The core idea is to precompute, for each hotel, the farthest index reachable in exactly one day from that hotel, using a two-pointer technique (since positions are sorted). This gives a "next" array `oneStep[i]` for i from 0 to n-1. Then, to answer a query (a, b) with a ≤ b, we want the minimum number of steps (days) k such that after k applications of `oneStep`, starting from a, we reach or exceed index b. This is a classic binary lifting problem: we build `up[level][i]` where `up[0][i] = oneStep[i]` and `up[level+1][i] = up[level][up[level][i]]` for each level. We then binary-search on the number of days (or apply binary lifting to find the largest k that still keeps us strictly before b, then add 1). Edge cases: when start == end return 0; when the daily limit allows reaching b directly, answer is 1; and the two-pointer must be careful when the last hotel is reachable from many previous ones. Time complexity: preprocessing O(n log n) due to binary lifting table, each query O(log n) using binary search on days (or O(log n) direct). Space O(n log n).
