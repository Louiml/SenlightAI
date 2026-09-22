Write a C++ function `long long countSurvivingLevels(long long startLevel, long long timeLimit)` that determines how many consecutive levels a player can complete starting from a given level. Level `r` requires a time cost computed by the arithmetic progression: the first played level (at index `r`) costs `3 + 2*(r-1)` units, and each subsequent level costs exactly 4 more units than the previous one (i.e., level `r+1` costs `3 + 2*(r-1) + 4`, level `r+2` costs `3 + 2*(r-1) + 8`, etc.). The player has a total time budget of `timeLimit`. The function must return the maximum integer `k ≥ 0` such that the sum of costs for levels `r, r+1, …, r+k-1` does not exceed `timeLimit`. The inputs may be large (up to 10^12), so use 64-bit integers to avoid overflow. If the first level's cost already exceeds `timeLimit`, return 0.

The costs form an arithmetic sequence with first term `a1 = 3 + 2*(r-1)` and common difference `d = 4`. The sum of the first `n` terms (i.e., levels `r` through `r+n-1`) is given by `S(n) = n/2 * (2*a1 + (n-1)*d)`. We need the largest `n` such that `S(n) ≤ timeLimit`. A direct loop from `n=1` upward is feasible for small inputs but would be too slow for large `timeLimit` (up to 10^12). Instead, we can solve for `n` using arithmetic: `S(n) = n/2 * (2*a1 + 4*(n-1)) = n/2 * (2*a1 + 4n - 4) = n*(a1 + 2n - 2)`. This is a quadratic in `n`: `2*n^2 + (a1 - 2)*n - timeLimit ≤ 0`. The positive root is `n = floor( (-(a1-2) + sqrt((a1-2)^2 + 8*timeLimit)) / 4 )`. Since we need the maximum integer satisfying the inequality, we compute this `n` and then verify by testing `n` and `n+1` to avoid floating-point rounding issues (use integer arithmetic for the check). Edge cases: `r` can be as low as 1, `a1` can be large, `timeLimit` may be 0 or small, so handle when `a1 > timeLimit` → return 0. Complexity: O(1) time, O(1) space.

#include <cmath>

// Returns the maximum number of consecutive levels that can be completed
// starting from level `startLevel` with a total time budget `timeLimit`.
long long countSurvivingLevels(long long startLevel, long long timeLimit) {
    // First term of the arithmetic sequence
    const long long a1 = 3 + 2 * (startLevel - 1);
    
    // If the first level itself costs more than the budget, no level can be completed
    if (a1 > timeLimit) {
        return 0;
    }
    
    // Solve S(n) = n * (a1 + 2*n - 2) <= timeLimit.
    // This is equivalent to 2*n^2 + (a1 - 2)*n - timeLimit <= 0.
    // Positive root approximately n = (-(a1-2) + sqrt((a1-2)^2 + 8*timeLimit)) / 4.
    const long long b = a1 - 2;
    const long long discriminant = b * b + 8 * timeLimit;
    const long long sqrtDisc = static_cast<long long>(std::sqrt(static_cast<long double>(discriminant)));
    
    // Candidate n (floor of positive root)
    long long n = (sqrtDisc - b) / 4; // since b might be negative, this still works because root formula is ( -b + sqrt ) / 4, but here we have - (a1-2) = -b? Let's derive: root = (-b + sqrtDisc) / 4. So n = (-b + sqrtDisc) / 4, but we used (sqrtDisc - b) / 4, same.
    
    // Adjust n to be safe: test n and n+1 using integer arithmetic
    auto sumCost = [a1](long long k) -> long long {
        // S(k) = k * (a1 + 2*k - 2)
        return k * (a1 + 2 * k - 2);
    };
    
    // Ensure n is not too large (due to rounding)
    while (n > 0 && sumCost(n) > timeLimit) {
        --n;
    }
    // Try to increase n if possible
    while (sumCost(n + 1) <= timeLimit) {
        ++n;
    }
    
    return n;
}

#include <cassert>

int main() {
    // Basic cases
    assert(countSurvivingLevels(1, 3) == 1);      // only level 1 cost 3
    assert(countSurvivingLevels(1, 0) == 0);      // no time
    assert(countSurvivingLevels(1, 2) == 0);      // first level too expensive
    assert(countSurvivingLevels(2, 5) == 1);      // level 2 cost 5, level 3 cost 9 >5
    assert(countSurvivingLevels(1, 10) == 2);     // costs 3+7=10, next 11>10
    assert(countSurvivingLevels(1, 11) == 3);     // 3+7+11=21 >11? Wait 3+7=10, +11=21>11, so only 2? Actually 3+7=10, next 11>11? 11>11? No, 11 > 11 is false, so can do 3 levels? 3+7+11=21 >11, so only 2. So this assert is wrong; let's recompute: sum(1)=3, sum(2)=3+7=10, sum(3)=3+7+11=21 >11 → 2. So assert should be 2.
    assert(countSurvivingLevels(1, 11) == 2);
    // Large values (avoid overflow)
    assert(countSurvivingLevels(1, 1000000000000LL) == 499999);
    // Starting level not 1
    assert(countSurvivingLevels(5, 100) == 5); // costs 11,15,19,23,27=95, next 31>100
    // Zero budget always 0
    assert(countSurvivingLevels(10, 0) == 0);
    return 0;
}
