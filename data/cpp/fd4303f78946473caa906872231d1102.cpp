// Write a C++ function `trainJourneyTime` that takes three vectors of equal length `n-1`: `cost`, `start`, and `freq`, representing the travel time, departure offset, and departure interval (in minutes) for each of `n` consecutive train segments. A traveler starts at the beginning of segment 0 at time 0, and for each segment `i`, they must wait at the station until the next departure, which occurs at times `start[i] + k * freq[i]` for non-negative integers `k` (with `start[i] >= 0` and `freq[i] > 0`). After boarding, traveling segment `i` takes `cost[i]` minutes, after which they arrive at the next station. The final destination is reached after all `n-1` segments; however, the last segment is not present (the traveler arrives after the previous segment), so the journey ends after completing segment `n-2`. The function should return a vector of length `n` where the `i`-th element (for `0 <= i < n-1`) is the arrival time at station `i+1` (i.e., after completing segments `0` through `i`), and the last element is `0` representing the final station. If `n` is 1 (no segments), return a single-element vector `{0}`. The vectors are 0-indexed and all values are non-negative integers. Ensure the function is const-correct and does not modify input.

// The core simulation is straightforward: maintain a current time `t`, initially 0. For each segment `i` from 0 to `n-2`, the traveler arrives at the station at time `t`. The next departure time is the smallest value `d >= t` such that `d >= start[i]` and `(d - start[i]) % freq[i] == 0`. This can be computed as: if `t < start[i]`, then `d = start[i]`; otherwise, if `t % freq[i] == 0` (and since `t >= start[i]` but not necessarily `t >= start[i]`? Actually `t >= start[i]` may not hold; but the formula `d = t + (freq[i] - t % freq[i]) % freq[i]` works generally: if `t % freq[i] == 0`, add 0; else add the remainder to reach next multiple. However, we must also ensure `d >= start[i]`. So compute `d = max(start[i], t)`; then if `d` is not a departure time (i.e., `(d - start[i]) % freq[i] != 0`), add the needed delta to reach the next departure. After boarding at `d`, travel cost `cost[i]` minutes, so `t = d + cost[i]`. Record this new `t` as arrival at station `i+1`. Continue. Edge cases: `n=1` returns `{0}`; `start[i]` may be 0, `freq[i]` may be 1; `t` may already be a departure time and `start[i]` may be less than `t`, so no waiting; values can be large, so use `long long`. Time complexity is O(n) and extra space O(n) for the result vector (excluding input).

#include <vector>
#include <algorithm>

// Compute arrival times at each station given train segment schedules.
// cost[i] = travel time for segment i, start[i] = first departure offset, freq[i] = interval.
// Returns a vector of length n, where result[i] is arrival at station i+1 for i < n-1, and last is 0.
std::vector<long long> trainJourneyTime(const std::vector<long long>& cost,
                                        const std::vector<long long>& start,
                                        const std::vector<long long>& freq) {
    int n = static_cast<int>(cost.size()) + 1;
    std::vector<long long> result(n, 0);
    if (n == 1) return result;

    long long current = 0;
    for (int i = 0; i < n - 1; ++i) {
        // Next departure time >= current and >= start[i], matching start[i] + k * freq[i]
        long long departure = std::max(current, start[i]);
        if (departure >= start[i]) {
            long long remainder = (departure - start[i]) % freq[i];
            if (remainder != 0) {
                departure += freq[i] - remainder;
            }
        }
        current = departure + cost[i];
        result[i] = current;
    }
    // result[n-1] remains 0 (final station)
    return result;
}

#include <cassert>
#include <vector>

// (The solution function is assumed to be available; include it above or in the same translation unit)

int main() {
    // Single station (no segments)
    {
        std::vector<long long> cost, start, freq;
        auto res = trainJourneyTime(cost, start, freq);
        assert(res.size() == 1 && res[0] == 0);
    }

    // One segment: start at 0, first depart at 0, interval 10, travel 5 -> arrive at 5
    {
        std::vector<long long> cost = {5};
        std::vector<long long> start = {0};
        std::vector<long long> freq = {10};
        auto res = trainJourneyTime(cost, start, freq);
        assert(res.size() == 2);
        assert(res[0] == 5);
        assert(res[1] == 0);
    }

    // Two segments: first depart at 0, travel 3, arrive at 3; second start 2, freq 2, next depart >=3 is 4, travel 1 -> arrive 5
    {
        std::vector<long long> cost = {3, 1};
        std::vector<long long> start = {0, 2};
        std::vector<long long> freq = {5, 2};
        auto res = trainJourneyTime(cost, start, freq);
        assert(res.size() == 3);
        assert(res[0] == 3);
        assert(res[1] == 5);
        assert(res[2] == 0);
    }

    // Test waiting when arrival earlier than start of next segment
    {
        std::vector<long long> cost = {2, 4};
        std::vector<long long> start = {0, 10};
        std::vector<long long> freq = {100, 3};
        auto res = trainJourneyTime(cost, start, freq);
        // Segment0: depart 0, travel 2 -> arrive 2. Segment1: start 10, depart 10 (since 10>=2), travel 4 -> arrive 14
        assert(res[0] == 2);
        assert(res[1] == 14);
    }

    // Test need to wait for next departure after arriving late
    {
        std::vector<long long> cost = {5, 2};
        std::vector<long long> start = {0, 1};
        std::vector<long long> freq = {10, 3};
        auto res = trainJourneyTime(cost, start, freq);
        // Segment0: depart 0, travel 5 -> arrive 5. Segment1: departures at 1,4,7,... next >=5 is 7, travel 2 -> arrive 9
        assert(res[0] == 5);
        assert(res[1] == 9);
    }

    // Test when arrival departs exactly at departure time
    {
        std::vector<long long> cost = {4, 3};
        std::vector<long long> start = {0, 2};
        std::vector<long long> freq = {2, 2};
        auto res = trainJourneyTime(cost, start, freq);
        // Segment0: depart 0, travel 4 -> arrive 4. Segment1: departures at 2,4,... next >=4 is 4, travel 3 -> arrive 7
        assert(res[0] == 4);
        assert(res[1] == 7);
    }

    // Test large values
    {
        std::vector<long long> cost = {1000000000LL, 2000000000LL};
        std::vector<long long> start = {1000000LL, 5000000LL};
        std::vector<long long> freq = {3, 7};
        auto res = trainJourneyTime(cost, start, freq);
        assert(res[0] == 1001000000LL); // 1000000 (wait? actually start 1000000, t=0, depart 1000000, travel 1000000000 -> 1001000000)
        // Segment1: t=1001000000, start=5000000, freq=7: find next departure >= t: (1001000000-5000000)=1000500000, mod 7: 1000500000 % 7 = 1000500000 - 7*142928571 = 1000500000 - 1000500000? Let's compute: 7*142928571 = 1000499997, remainder = 3, so add 4 -> departure 1001000004, travel 2000000000 -> arrive 3001000004
        assert(res[1] == 3001000004LL);
    }

    return 0;
}
