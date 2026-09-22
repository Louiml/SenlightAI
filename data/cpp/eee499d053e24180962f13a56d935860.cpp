/*
Write a C++ function `minimumTimeToMakeParathas` that, given a target number of parathas to prepare, a number of chefs, and a vector of their individual ranks (where rank equals the time in minutes required for that chef to make one paratha), returns the minimum possible total time in minutes required for all chefs working simultaneously to complete the order. Each chef works continuously and independently, and the time to make the k-th paratha by a chef is `rank * k` (so the first paratha takes `rank` minutes, the second takes `rank * 2`, and so on). The function must accept a vector of positive integers, handle unsorted ranks, and return a `long long` value. You may assume the input ranks are positive, the target is at least 1, and chefs is at least 1. The solution should use binary search over time, verifying feasibility with a helper function.
*/

#include <vector>
#include <algorithm>

// Check if given time allows chefs to make at least target parathas.
bool canMakeParathas(long long timeLimit, const std::vector<long long>& ranks, long long target) {
    long long total = 0;
    for (long long r : ranks) {
        long long timeLeft = timeLimit;
        long long multiplier = r; // time for first paratha
        while (timeLeft >= multiplier) {
            timeLeft -= multiplier;
            total++;
            if (total >= target) return true;
            multiplier += r; // next paratha takes r more minutes
        }
    }
    return total >= target;
}

// Return minimum total time for all chefs to make exactly target parathas.
long long minimumTimeToMakeParathas(long long target, long long numChefs, const std::vector<long long>& ranks) {
    // Use 0-based indexing if needed, but vector already has size numChefs.
    long long minRank = *std::min_element(ranks.begin(), ranks.end());
    long long low = 0;
    // Worst case: fastest chef makes all parathas alone.
    long long high = minRank * target * (target + 1) / 2;
    long long answer = high;

    while (low <= high) {
        long long mid = low + (high - low) / 2;
        if (canMakeParathas(mid, ranks, target)) {
            answer = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return answer;
}

#include <cassert>
#include <vector>

// (Include the solution code above here)

int main() {
    // Single chef rank 1, need 1 paratha -> 1 minute
    assert(minimumTimeToMakeParathas(1, 1, {1}) == 1);
    // Single chef rank 3, need 2 parathas -> 3 + 6 = 9
    assert(minimumTimeToMakeParathas(2, 1, {3}) == 9);
    // Two chefs ranks 1 and 2, need 3 parathas: chef1 makes 2 (1+2=3), chef2 makes 1 (2) -> total 3
    assert(minimumTimeToMakeParathas(3, 2, {1, 2}) == 3);
    // Three chefs ranks 1,2,3, need 5 parathas: chef1 makes 3 (1+2+3=6), chef2 makes 1 (2), chef3 makes 1 (3) -> time 6
    assert(minimumTimeToMakeParathas(5, 3, {1, 2, 3}) == 6);
    // Unsorted ranks: chefs 5 and 1, need 4 parathas: fastest rank1 makes 3 (1+2+3=6), rank5 makes 0, total 3 <4; time 7: rank1 makes 3 (6), rank5 makes 1 (5) total 4 -> 7
    assert(minimumTimeToMakeParathas(4, 2, {5, 1}) == 7);
    // Large target with multiple chefs
    assert(minimumTimeToMakeParathas(10, 3, {1, 2, 3}) == 12);
    // Duplicate ranks
    assert(minimumTimeToMakeParathas(4, 2, {2, 2}) == 8);
    // Single paratha with many chefs
    assert(minimumTimeToMakeParathas(1, 5, {10, 20, 30, 40, 50}) == 10);
    return 0;
}

// The problem is a classic minimization problem solvable via binary search on the answer. The key is to determine, for a given time `mid`, whether the chefs can collectively produce at least `target` parathas. For each chef with rank `r`, the maximum number of parathas they can make in time `mid` is found by solving the arithmetic series: `r * (1 + 2 + ... + p) ≤ mid`, i.e., `r * p * (p+1) / 2 ≤ mid`. A simple iterative approach (as in the snippet) accumulates time: start with `time_left = mid`, subtract `r`, then `2r`, then `3r`, etc., counting each time the subtraction remains non-negative. This works but is inefficient in the worst case; a closed-form using quadratic equation or a while loop is acceptable for typical constraints (target up to ~10^6). The upper bound for binary search is the time for the fastest chef (minimum rank) to make all target parathas: `rank_min * target * (target+1) / 2`. Lower bound is 0. Binary search runs `O(log(maxTime))` iterations, and each check runs `O(chefs * sqrt(mid/rank))` in the worst case, but since mid is bounded, the overall complexity is approximately `O(chefs * log(maxTime) * sqrt(maxTime/rank_min))`. Space is `O(1)` beyond the input vector. Important edge cases: target = 1 (answer is minimum rank), ranks unsorted (sorting optional but not required for correctness, only for upper bound computation—we can just take min rank via `*min_element`). Duplicate ranks are fine. All integers may be large, so use `long long` to avoid overflow in `rank * p * (p+1) / 2` when computing the upper bound.
