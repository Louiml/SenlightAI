// You are given three integers: `n` (number of holidays already taken), `d` (total days in the year), and `s` (consecutive sick days you can take), followed by `n` integers representing the day numbers (1-based) of holidays already taken. Your task is to write a C++ function `int maxZeroDays(int n, int d, int s, const std::vector<int>& holidays)` that simulates the following process: Start with an array of length `d` where all days are marked as available (1). Mark the given holiday days as unavailable (0). Then, for each possible starting index `i` from `0` to `d - s` (inclusive), simulate taking `s` consecutive sick days starting at index `i` (if a day is available, mark it as unavailable; if it is already unavailable, it stays unavailable). After the simulation, count the total number of days that are unavailable (0) in the whole array. The function should return the maximum such count achievable over all valid starting indices `i`. Note that the sick days are taken in the actual order from `i` to `i+s-1` and only those days can be changed (days outside the sick period are not affected during that simulation). The holidays are guaranteed to be distinct and within `[1, d]`.

#include <cassert>
#include <vector>

int main() {
    // Test 1: Basic example from snippet: n=2, d=5, s=2, holidays {1,4}
    assert(maxZeroDays(2, 5, 2, {1,4}) == 4);

    // Test 2: All days holidays, any sick days don't change count
    assert(maxZeroDays(3, 3, 1, {1,2,3}) == 3);

    // Test 3: No holidays, one sick day anywhere, count is 1
    assert(maxZeroDays(0, 5, 1, {}) == 1);

    // Test 4: No holidays, s=3, d=5, best is 3 consecutive days
    assert(maxZeroDays(0, 5, 3, {}) == 3);

    // Test 5: Holidays in the middle, sick days overlap with holidays
    assert(maxZeroDays(1, 5, 3, {3}) == 3); // start at 0: days 0,1,2 become sick, total zeros=3; start at 1: days 1,2,3 become sick (but 3 already zero), zeros=3; start at 2: days 2,3,4 zeros=3.

    // Test 6: s=0, only holidays count
    assert(maxZeroDays(2, 5, 0, {2,5}) == 2);

    // Test 7: d=1, s=1, one holiday
    assert(maxZeroDays(1, 1, 1, {1}) == 1);

    // Test 8: d=6, s=6, all days become sick (but holidays already mark some)
    assert(maxZeroDays(2, 6, 6, {2,4}) == 6); // all days become 0 because sick covers all, but holidays already 0, so total 6

    // Test 9: d=4, s=2, holidays at ends
    assert(maxZeroDays(2, 4, 2, {1,4}) == 3); // start 0: zeros {1,2}=2; start1: {1,2,3}=3; start2: {2,3,4}=3

    // Test 10: Large d but small s, no holidays
    assert(maxZeroDays(0, 1000, 1, {}) == 1);
}

#include <vector>
#include <algorithm>

// Simulates taking s consecutive sick days starting at each possible index.
// Returns the maximum number of unavailable (0) days after any simulation.
int maxZeroDays(int n, int d, int s, const std::vector<int>& holidays) {
    // Build the base availability: 1 = available, 0 = already holiday
    std::vector<int> base(d, 1);
    for (int h : holidays) {
        if (h >= 1 && h <= d) {
            base[h - 1] = 0;
        }
    }

    int best = 0;
    // If s is 0, we can't take any sick days, so the count is just number of holidays.
    if (s == 0) {
        return n;
    }

    // Try every possible starting index for the sick days.
    for (int start = 0; start <= d - s; ++start) {
        // Copy the base state for this simulation.
        std::vector<int> days = base;

        // Simulate taking sick days from start to start+s-1.
        for (int j = start; j < start + s; ++j) {
            if (days[j] == 1) {
                days[j] = 0; // mark as unavailable
            }
        }

        // Count zeros (unavailable days)
        int zeros = 0;
        for (int val : days) {
            if (val == 0) ++zeros;
        }

        best = std::max(best, zeros);
    }

    return best;
}

// The problem can be solved by brute-force simulation. For each starting index `i` from 0 to `d-s` inclusive, create a copy of the original availability vector (size `d`, initially all 1, then set corresponding holiday positions to 0). Then iterate through the `s` days starting at `i`, and if the current day is 1 (available), set it to 0. After the simulation, count the number of zeros in the entire vector. Track the maximum count across all possible starting indices. Edge cases: when `s` is 0, only holidays count, but the problem implies `s > 0` (since it's sick days), but handle gracefully. When `d` equals `s`, only one starting index exists (`i=0`). When holidays already cover many days, the sick days might not reduce the total count if they overlap with holidays or if all days become unavailable. Time complexity: O((d-s+1) * (d + s)) = O(d^2) in the worst case because for each starting index we copy the vector O(d) and simulate O(s). Space complexity: O(d) for the temporary vector. This is acceptable for small to moderate `d` (e.g., up to 2000). For larger constraints, a more optimized prefix-sum approach could be considered, but this simulation is straightforward.
