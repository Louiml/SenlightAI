/*
Implement a C++ function `int minBananaEatingSpeed(std::vector<int>& piles, int h)` that, given a non-empty vector `piles` where each integer represents the number of bananas in one pile (each pile must be completely eaten), and an integer `h` representing the maximum number of hours available, returns the minimum integer eating speed `x` (bananas per hour) such that Koko can eat all bananas within `h` hours. Eating speed is an integer ≥ 1. If she eats at speed `x`, each pile takes `ceil(pile/x)` hours (e.g., 7 bananas at speed 3 takes 3 hours). The function should work for `h >= piles.size()` (since she can only eat one pile at a time, she must have at least as many hours as piles, but may need more depending on pile sizes). Assume `piles` contains positive integers, `h` ≤ 10^9, and `piles.size()` ≤ 10^4. Return the smallest possible speed.
*/
#include <vector>
#include <algorithm>
#include <cmath> // optional, but we can avoid floating point

// Helper: returns true if eating speed x allows finishing all piles within h hours.
bool canEatAll(const std::vector<int>& piles, int x, int h) {
    long long totalHours = 0;
    for (int pile : piles) {
        // Compute ceil(pile / x) without floating point
        totalHours += pile / x;
        if (pile % x != 0) {
            totalHours += 1;
        }
        if (totalHours > h) { // early exit to avoid overflow
            return false;
        }
    }
    return totalHours <= h;
}

// Returns the minimum integer eating speed (bananas per hour) to finish all piles within h hours.
int minBananaEatingSpeed(const std::vector<int>& piles, int h) {
    int low = 0;      // infeasible lower bound (0 is never a valid speed)
    int high = *std::max_element(piles.begin(), piles.end()); // feasible upper bound
    while (low + 1 < high) {
        int mid = low + (high - low) / 2;
        if (canEatAll(piles, mid, h)) {
            high = mid;
        } else {
            low = mid;
        }
    }
    return high;
}
#include <cassert>
#include <vector>

// The solution function is expected to be defined above; include it here for completeness.
// (In practice, this would be included from the solution file.)

int main() {
    // Basic example
    assert(minBananaEatingSpeed({3, 6, 7, 11}, 8) == 4);
    // All piles equal, minimal speed
    assert(minBananaEatingSpeed({30, 11, 23, 4, 20}, 5) == 30);
    // Huge hours, answer is 1
    assert(minBananaEatingSpeed({5, 5, 5}, 100) == 1);
    // Single pile
    assert(minBananaEatingSpeed({7}, 7) == 1);
    // Single pile, tight deadline
    assert(minBananaEatingSpeed({7}, 1) == 7);
    // Multiple piles, all same size
    assert(minBananaEatingSpeed({10, 10, 10}, 6) == 6);
    // More hours than needed, answer 1
    assert(minBananaEatingSpeed({1, 2, 3}, 10) == 1);
    // Worst case: one huge pile
    assert(minBananaEatingSpeed({1000000000}, 1) == 1000000000);
    // Mixed values
    assert(minBananaEatingSpeed({312884470}, 312884469) == 2);
    return 0;
}
// This problem is a classic binary search on the answer. The search space for the speed `x` is from 1 to the maximum pile size, because at speed equal to the largest pile, each pile takes exactly 1 hour (if pile ≤ x) or 1 hour (if exactly x), so total hours ≤ number of piles ≤ h (given h ≥ piles.size()), so it's always feasible. The feasibility function `canEatAll(x)` computes total hours by summing `ceil(pile / x)` for each pile and returns true if that total ≤ h. Since the feasibility predicate is monotonic – if speed `x` works, any larger speed also works – we can binary search for the smallest `x` such that `canEatAll(x)` is true. Use `left = 0` (exclusive) and `right = max(piles)` (inclusive) with invariant that `left` is infeasible (or sentinel 0) and `right` is feasible. In each iteration, set `mid = (left+right)/2` (integer floor), if `canEatAll(mid)` true then `right = mid` else `left = mid`. Eventually `right` is the answer. Edge cases: all piles equal → answer 1 if h ≥ piles.size(); if h is huge, answer may be 1. Time complexity: O(n log M) where n = piles.size() and M = max pile value, because each feasibility check is O(n) and binary search runs log(M) iterations. Space: O(1) extra.
