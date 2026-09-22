/*
Write a C++ function named `totalMutedDuration` that takes two parameters: a non-negative integer `n` representing the number of times a user presses the mute button, and a vector of strictly increasing non-negative integers `pressTimes` (of length `n`) representing the times (in seconds) when each mute press occurs. After each press, the user remains muted for exactly `t` seconds starting from the press time, unless a subsequent press occurs before the mute period ends, in which case the mute period restarts from that later press. The function should return the total number of seconds the user is muted, given a fixed mute duration `t` (a non-negative integer) passed as a separate parameter. For example, if presses occur at times 0, 4, and 7 with `t = 5`, the mute intervals are [0,5), [4,9), and [7,12), and the total muted time is 12 seconds. The input vector will have at least one element, and all times are non-negative and sorted in strictly increasing order.
*/
#include <vector>
#include <algorithm>

// Compute total muted duration given press times and fixed mute length t.
// pressTimes must be non-empty and strictly increasing.
long long totalMutedDuration(const std::vector<long long>& pressTimes, long long t) {
    if (pressTimes.empty()) return 0;
    long long total = t;
    for (std::size_t i = 1; i < pressTimes.size(); ++i) {
        total += std::min(pressTimes[i] - pressTimes[i - 1], t);
    }
    return total;
}
#include <cassert>
#include <vector>

// The solution function is declared above (in practice it would be included here).
long long totalMutedDuration(const std::vector<long long>& pressTimes, long long t);

int main() {
    // Single press
    assert(totalMutedDuration({0}, 5) == 5);
    assert(totalMutedDuration({10}, 0) == 0);

    // Two presses with gap less than t
    assert(totalMutedDuration({0, 2}, 5) == 5 + std::min(2LL, 5LL)); // 7
    assert(totalMutedDuration({0, 2}, 5) == 7);

    // Two presses with gap equal to t
    assert(totalMutedDuration({0, 5}, 5) == 5 + 5); // 10

    // Two presses with gap greater than t
    assert(totalMutedDuration({0, 10}, 5) == 5 + 5); // 10

    // Three overlapping presses
    assert(totalMutedDuration({0, 4, 7}, 5) == 12);

    // Non-zero starting time
    assert(totalMutedDuration({3, 5, 8}, 4) == 4 + std::min(2LL,4LL) + std::min(3LL,4LL)); // 4+2+3=9
    assert(totalMutedDuration({3, 5, 8}, 4) == 9);

    // Large values to check 64-bit handling
    assert(totalMutedDuration({1000000000000LL, 1000000000005LL}, 3) == 3LL + 3LL); // 6

    // t=0 with multiple presses
    assert(totalMutedDuration({0, 1, 2}, 0) == 0);

    return 0;
}
// The problem is essentially computing the union of intervals `[press[i], press[i] + t)` and summing their total length. Since the presses are sorted and intervals may overlap, we can iterate through the press times and maintain a running total. For each press after the first, the additional muted time contributed by this press is the minimum of `t` (full mute duration) and the gap between this press and the previous one (because if the gap is shorter than `t`, the previous interval already covers part of this new interval, and the new press only adds the difference until `press[i] + t`). More precisely: start with `total = t` (for the first press). For each subsequent press at time `press[i]`, add `min(press[i] - press[i-1], t)`. This works because if the gap is less than `t`, the overlapping part is already counted, and the new interval extends by exactly the gap; if the gap is at least `t`, the previous interval has ended and we add a full `t`. Edge cases: a single press returns `t`; `t = 0` returns 0; large `n` and large times require 64-bit integers to avoid overflow. Time complexity is `O(n)` and space complexity is `O(1)` (assuming the input vector is given and not modified).
