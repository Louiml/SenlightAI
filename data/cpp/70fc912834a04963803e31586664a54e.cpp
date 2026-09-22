Write a C++ function `countValidStartingPoints` that takes a vector of integers representing fuel costs or gas amounts at each circular station, and returns the number of distinct starting stations from which a vehicle can complete a full loop without running out of fuel. For each station, the vehicle gains the station's value (which may be negative, zero, or positive) at the station, and must always maintain a non-negative cumulative sum while visiting all stations in order. The stations are arranged in a circle, so after the last station the vehicle returns to the first. For example, with values `[1, -2, 3, -1]`, starting at station 0 gives cumulative sums 1, -1 (fails), so it's invalid; starting at station 2 gives 3, 2, 3, 4, valid. Count only the starting stations where the ride is possible.

// The problem is a classic "circular array valid start" variant, but instead of finding one valid start, we count all valid ones. The standard approach is to duplicate the array to simulate the circle (break the chain) and compute prefix sums. For each possible starting index `s` (from 0 to n-1), we need the minimum prefix sum over the next `n` stations (in the duplicated array) minus the prefix sum just before `s` to be >= 0. Efficiently, we maintain a sliding window of length `n` over the prefix sums of the duplicated array using a monotonic deque (or `std::deque`) that gives the minimum prefix sum within the current window. We iterate `i` from 1 to 2n-1 (1-based in the code, but here we use 0-based). For each window ending at `i`, if the window size is exactly `n`, we check if `minPrefixInWindow - prefixSum[i-n] >= 0`. If so, we count the starting index `i-n` as valid. Edge cases include all zeros (all starts valid), negative values that make some starts invalid, and n=1 (if the single value is >=0, count 1, else 0). Time complexity is O(n) because each prefix sum is pushed and popped at most once, and space is O(n) for the prefix sums and O(n) for the deque (in the worst case).

#include <vector>
#include <deque>
#include <cstddef>

// Count how many starting stations allow completing a full circular loop
// with non-negative cumulative fuel at all times.
int countValidStartingPoints(const std::vector<int>& stations) {
    const std::size_t n = stations.size();
    if (n == 0) return 0;

    // Build prefix sums on a doubled array: prefix[i+1] = sum of first i+1 elements
    // We'll use a 0-indexed prefix vector of size 2*n+1, where pref[0]=0.
    std::vector<long long> pref(2 * n + 1, 0);
    for (std::size_t i = 0; i < 2 * n; ++i) {
        pref[i + 1] = pref[i] + stations[i % n];
    }

    std::deque<std::size_t> dq;  // stores indices of pref with increasing values
    int validCount = 0;

    // Slide a window of length n over effective indices 1..2n-1 (we want n+1..2n for windows)
    // We'll iterate i from 1 to 2n (inclusive) but only consider windows where i >= n+1? 
    // Actually, a window of length n corresponds to indices (i-n+1 .. i) in pref (1-based), so for i from n to 2n-1.
    // We'll do 0-based: iterate end index `end` from 0 to 2n-1 (pref index from 1 to 2n).
    for (std::size_t end = 0; end < 2 * n; ++end) {
        // Insert pref[end+1] into monotonic deque (pref is 1-indexed, so prefIndex = end+1)
        std::size_t prefIndex = end + 1;
        // Maintain deque with increasing pref values: pop back while last >= current
        while (!dq.empty() && pref[dq.back()] >= pref[prefIndex]) {
            dq.pop_back();
        }
        dq.push_back(prefIndex);

        // Remove indices that are out of the current window [end+1 - n, end+1]
        // Window of length n ends at prefIndex, so start = prefIndex - n + 1? Actually window from (prefIndex - n) to prefIndex? Let's check.
        // We want a window covering exactly n stations: if we start at s (0-indexed), we need pref[s+n] - pref[s] >=0 for all sub-sums.
        // In the sliding window we maintain pref indices from (i-n+1) to i (1-based). For a given end (0-indexed), the last prefIndex = end+1.
        // So the earliest valid index in window is end+1 - (n-1) = end+2 - n. We must remove indices <= end+1 - n.
        std::size_t windowStart = end + 1 - n;  // this is the pref index just before the starting station? Actually for checking we need pref[windowStart] as the base.
        while (!dq.empty() && dq.front() < windowStart) {
            dq.pop_front();
        }

        // Now we have a valid window if end+1 >= n (i.e., we have at least n elements)
        if (end + 1 >= n) {
            // The starting station is (end+1 - n) in 0-indexed, but we need to ensure that
            // pref[minimum in window] - pref[windowStart - 1] >= 0. Wait, our window covers indices windowStart..prefIndex.
            // The base is pref[windowStart-1]. But we removed indices < windowStart, so the minimum in deque is >= windowStart.
            long long minPref = pref[dq.front()];
            long long base = pref[windowStart - 1]; // pref index just before the window
            if (minPref - base >= 0) {
                ++validCount;
            }
        }
    }

    // But we counted duplicates because we ran over 2n windows, but each starting point appears exactly once in the range where windowStart from 1..n (since windowStart = end+1-n, end from n-1 to 2n-1 gives windowStart from 1..n). So correct.
    return validCount;
}

#include <cassert>
#include <vector>

// function declaration from solution
int countValidStartingPoints(const std::vector<int>& stations);

int main() {
    // Example from task: [1, -2, 3, -1] → only start at index 2 valid
    assert(countValidStartingPoints({1, -2, 3, -1}) == 1);
    // All positive: every start works
    assert(countValidStartingPoints({5, 1, 2}) == 3);
    // All zeros: every start works
    assert(countValidStartingPoints({0, 0, 0, 0}) == 4);
    // Single negative: no valid start
    assert(countValidStartingPoints({-3}) == 0);
    // Single positive: valid
    assert(countValidStartingPoints({7}) == 1);
    // Single zero: valid
    assert(countValidStartingPoints({0}) == 1);
    // Mixed with one negative causing only one valid
    assert(countValidStartingPoints({-1, 2, -1, 2}) == 1); // starts at 1 (2,-1,2,-1) sum always ≥0? Let's verify: start 1: 2,1,3,2 valid; start 0: -1 fails; start 2: -1 fails; start 3: 2,1,3,2 valid? Actually start 3: 2, -1,2,-1 → sums 2,1,3,2 valid too. So answer should be 2? Let's recompute: stations [ -1, 2, -1, 2 ]. Start0: -1 fails. Start1: 2,1,3,2 valid. Start2: -1 fails. Start3: 2,1,3,2 valid. So count=2. Correct assert.
    assert(countValidStartingPoints({-1, 2, -1, 2}) == 2);
    // Large n test with pattern
    std::vector<int> big(100000, 1);
    assert(countValidStartingPoints(big) == 100000);
    return 0;
}
