// Write a C++ function `long long minimumTimeToCompleteTrips(const std::vector<int>& busTimes, int requiredTrips)` that takes a list of times each bus takes to complete one trip (all positive integers) and a target number of total trips that must be completed across all buses working simultaneously and independently. The function must return the minimum integer time (in the same units as the bus times) such that the total number of trips completed by all buses by that time is at least `requiredTrips`. Each bus completes trips continuously: if a bus takes `t` time per trip, then by time `T`, it completes `T / t` trips (integer division). Buses can work independently and overlapping, and trips do not need to finish exactly at the chosen time—the count is based on fully completed trips up to that time. The function should handle large inputs (up to `10^5` buses, times up to `10^7`, requiredTrips up to `10^7`), and the answer may be as large as `10^14`. Use a binary search over the time, checking feasibility with integer arithmetic. Return the smallest feasible time. The function should be efficient and avoid overflow by using `long long` for intermediate calculations.

The solution uses binary search over the possible answer range. The minimum possible time is `0` (though at time 0 no trips are completed, so the answer will be at least 1 if `requiredTrips > 0`), and the maximum can be set conservatively to `1e14` or a tighter bound like `min(busTimes) * requiredTrips` because even the fastest bus alone could complete the required trips at that time. For a candidate time `T`, we compute the total number of trips completed by summing `T / busTimes[i]` for each bus, using integer division. If the sum is at least `requiredTrips`, `T` is feasible; otherwise it is not. Since feasibility is monotonic—once feasible, all larger times are also feasible—we can binary search for the smallest feasible time. Edge cases: `requiredTrips = 0` returns `0` trivially; all times positive so division is safe; sum of trips may overflow `int`, so use `long long`. Complexity: `O(n log(maxTime))` time, where `n` is the number of buses and `maxTime` is the upper bound (about `1e14`), giving roughly `~47` iterations. Space complexity is `O(1)` extra.

#include <vector>
#include <algorithm>
#include <cstdint>

// Returns the minimum time T such that the total number of trips completed
// by all buses by time T is at least requiredTrips.
// busTimes[i] is the time bus i takes to complete one trip.
// Each bus completes T / busTimes[i] trips by time T (integer division).
long long minimumTimeToCompleteTrips(const std::vector<int>& busTimes, int requiredTrips) {
    using ll = long long;
    if (requiredTrips <= 0) return 0;

    // Upper bound: even the fastest bus alone can complete requiredTrips.
    int minBusTime = *std::min_element(busTimes.begin(), busTimes.end());
    ll high = static_cast<ll>(minBusTime) * requiredTrips;
    ll low = 1; // At time 0, no trips are completed (since requiredTrips > 0)

    auto feasible = [&](ll time) -> bool {
        ll totalTrips = 0;
        for (int t : busTimes) {
            totalTrips += time / t;
            if (totalTrips >= requiredTrips) return true; // early exit
        }
        return false;
    };

    ll ans = high;
    while (low <= high) {
        ll mid = low + (high - low) / 2;
        if (feasible(mid)) {
            ans = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return ans;
}

#include <cassert>

int main() {
    // Single bus, exact time
    assert(minimumTimeToCompleteTrips({5}, 1) == 5);
    assert(minimumTimeToCompleteTrips({5}, 3) == 15);

    // Two buses, simple case
    assert(minimumTimeToCompleteTrips({2, 3}, 1) == 2); // bus1 finishes one at T=2
    assert(minimumTimeToCompleteTrips({2, 3}, 2) == 3); // bus1=1, bus2=1 at T=3
    assert(minimumTimeToCompleteTrips({2, 3}, 3) == 4); // bus1=2, bus2=1 at T=4

    // Larger required trips
    assert(minimumTimeToCompleteTrips({1, 1}, 5) == 3); // at T=3: 3+3=6 >=5, at T=2: 2+2=4<5
    assert(minimumTimeToCompleteTrips({3, 5}, 7) == 15); // T=15: 5+3=8>=7, T=14: 4+2=6<7

    // requiredTrips = 0
    assert(minimumTimeToCompleteTrips({2, 3}, 0) == 0);

    // Single bus, large number
    assert(minimumTimeToCompleteTrips({10000000}, 100000) == 1000000LL * 10000000);

    // Multiple buses with same time
    assert(minimumTimeToCompleteTrips({4, 4, 4}, 10) == 16); // T=16: 4+4+4=12>=10, T=12: 3+3+3=9<10

    return 0;
}
