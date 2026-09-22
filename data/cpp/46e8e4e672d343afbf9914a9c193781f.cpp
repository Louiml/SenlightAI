// Write a C++ function `maxPenalty` that takes a vector of non-negative integers representing altitudes at consecutive checkpoints along a hiking trail, and an integer `k` representing the maximum allowed ascent between adjacent checkpoints. The function should return the maximum value of `(current altitude - next altitude - k)` computed over all adjacent pairs, where this value represents the "penalty" when descending more than allowed. If the maximum penalty is negative, return 0 instead, and if the input has fewer than 2 altitudes, also return 0. The function must be pure and not modify the input vector.

#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(maxPenalty({10, 5}, 3) == 2);   // 10-5-3 = 2
    assert(maxPenalty({10, 5}, 5) == 0);   // 10-5-5 = 0 -> returns 0
    assert(maxPenalty({10, 5}, 10) == 0);  // negative penalty -> returns 0

    // Multiple pairs
    assert(maxPenalty({10, 8, 2}, 0) == 6); // max(10-8-0=2, 8-2-0=6) = 6
    assert(maxPenalty({10, 8, 2}, 4) == 2); // max(10-8-4=-2, 8-2-4=2) = 2
    assert(maxPenalty({10, 8, 2}, 7) == 0); // max(-5, -1) -> 0

    // Edge cases
    assert(maxPenalty({}, 3) == 0);
    assert(maxPenalty({5}, 3) == 0);
    assert(maxPenalty({5, 5}, 0) == 0);

    // All equal
    assert(maxPenalty({7, 7, 7}, 2) == 0);
    assert(maxPenalty({7, 7, 7}, -1) == 1); // 7-7-(-1)=1

    return 0;
}

#include <vector>
#include <algorithm>

// Returns the maximum descent penalty over allowed k, or 0 if no positive penalty exists.
// For each adjacent pair (a[i], a[i+1]), penalty = a[i] - a[i+1] - k.
int maxPenalty(const std::vector<int>& altitudes, int k) {
    if (altitudes.size() < 2) {
        return 0;
    }
    int best = 0;
    for (size_t i = 0; i + 1 < altitudes.size(); ++i) {
        int penalty = altitudes[i] - altitudes[i+1] - k;
        if (penalty > best) {
            best = penalty;
        }
    }
    return best;
}

// The core idea is to iterate over adjacent pairs in the input vector, computing for each pair `a[i] - a[i+1] - k`. This value is positive when the descent between those two points exceeds the allowed `k` by that amount. The answer is the maximum of these values across all pairs, but if no pair yields a positive result, the penalty should be zero (since you wouldn't penalize for negative values). Edge cases: if the vector has fewer than 2 elements, there are no adjacent pairs, so the maximum is 0. If the maximum computed value is negative or zero, return 0. Time complexity is \(O(n)\) where \(n\) is the number of altitudes, and space complexity is \(O(1)\) besides the input vector.
