/*
Write a C++ function that takes two integers, `n` (the number of apples) and `l` (a base price modifier), and computes a total cost using the following rule: start with `n * l`, then add `i` for each `i` from `0` to `n-1` (inclusive). If `l` is negative and `l < -n`, subtract `(l + n - 1)` from the total. If `l` is positive, subtract `l` from the total. The final result must be returned as an `int`. The function should handle all integer inputs, including negative `n`? In your implementation, assume `n >= 1` and `l` can be any integer. Edge cases: when `l` is zero, no extra adjustment occurs (since `l > 0` is false and `l < -n` is false if `n > 0`). Also, if `l` is negative but not less than `-n`, no adjustment occurs. Provide a robust solution with a descriptively named function.
*/
#include <cstdint>  // not needed, but for completeness

// Computes the total cost according to the described rules.
// n must be >= 1. l can be any integer.
int computeAppleCost(int n, int l) {
    int answer = n * l;
    for (int i = 0; i < n; ++i) {
        answer += i;
    }
    if (l < 0 && l < -n) {
        answer -= (l + n - 1);
    }
    if (l > 0) {
        answer -= l;
    }
    return answer;
}
#include <cassert>

int main() {
    // Basic case with positive l and small n
    assert(computeAppleCost(3, 2) == 3*2 + 0 + 1 + 2 - 2); // 6+3-2 = 7
    // l = 0: no adjustments
    assert(computeAppleCost(4, 0) == 0 + 0 + 1 + 2 + 3); // 6
    // Negative l but not less than -n: no negative adjustment
    assert(computeAppleCost(3, -2) == 3*(-2) + 0+1+2); // -6+3 = -3
    // Negative l less than -n: apply negative adjustment
    // n=3, l=-4: base -12 + (0+1+2)= -12+3=-9; l< -n? -4<-3 true; subtract (l+n-1)=(-4+3-1)=-2 => -9 - (-2)= -7
    assert(computeAppleCost(3, -4) == -3 + 3 - (-2) == -7);
    // Positive l with n=1
    assert(computeAppleCost(1, 5) == 1*5 + 0 - 5 == 0);
    // l negative but exactly -n: no adjustment
    assert(computeAppleCost(5, -5) == 5*(-5) + (0+1+2+3+4) == -25 + 10 == -15);
    // Large n and positive l
    assert(computeAppleCost(10, 1) == 10*1 + 45 - 1 == 54);
    // Negative l where l = -n-1
    // n=2, l=-3: base -6 + (0+1)= -5; l< -2 true; subtract (l+n-1)=(-3+2-1) = -2 => -5 - (-2) = -3
    assert(computeAppleCost(2, -3) == -5 - (-2) == -3);
    // l > 0 and n=0? Not required, but we assume n>=1. Test n=1, l=1
    assert(computeAppleCost(1, 1) == 1 + 0 - 1 == 0);
    return 0;
}
// The algorithm directly follows the problem statement. First, compute the base sum as `n * l`. Then, iterate `i` from `0` to `n-1` and add each `i` to the running total. This loop sums the first `n` non-negative integers, which equals `n*(n-1)/2`, but we keep the loop for clarity and to match the description. After the loop, apply two conditional adjustments:
// - If `l < 0 && l < -n`, subtract `(l + n - 1)`. Note that because `l` is negative and `l < -n`, `l+n` is negative, so `l+n-1` is negative, and subtracting it adds a positive amount. This case only triggers when `l` is a sufficiently negative number (e.g., `n=5, l=-7`).
// - If `l > 0`, subtract `l` (which reduces the total). If `l == 0`, no adjustment is made.
// Important edge cases: `l=0` (no adjustment), `l=-n` (not less than `-n`, so no negative adjustment), `l=-n-1` (triggers the negative adjustment), and `l` large positive (just subtract `l`). The time complexity is `O(n)` due to the loop, and space complexity is `O(1)`.
