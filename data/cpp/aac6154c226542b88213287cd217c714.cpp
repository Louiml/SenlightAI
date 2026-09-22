/*
Write a C++ function `int minStepsToReachN(int n)` that, given a positive integer `n`, simulates a process where you start at 1 and can repeatedly either (a) add the largest proper divisor of your current number to itself, or (b) if the current number is already `n`, stop. Specifically, emulate the recursive logic in the provided `Solution::solve`: from a current number `x`, if `x == n`, return 0; otherwise, find the largest integer `d` strictly less than `x` that divides `x` (i.e., the largest proper divisor), then the total cost is `d + minStepsToReachN(d)` — note the recursive call always uses `d` as the next argument, and the answer accumulates via addition. Your function should return the final accumulated result for a given `n`, assuming the recursion always terminates (which it does because `d < x`). For example, `n = 1` returns 0, and `n = 27` should produce 35 (since the largest proper divisors chain: 27→9→3→1, sum = 9+3+1=13, but verify carefully; the provided snippet's logic yields a specific value — implement exactly that logic). Ensure your function handles `n == 1` returning 0 without recursion.
*/
#include <algorithm> // For std::max, though not used here, keep for completeness

// Returns the largest proper divisor of n (strictly less than n) that divides n.
// Precondition: n >= 2.
int largestProperDivisor(int n) {
    // Start from n/2 and go downwards to find the first divisor.
    for (int i = n / 2; i >= 1; --i) {
        if (n % i == 0) {
            return i;
        }
    }
    return 1; // Should never reach here for n > 1, but safe fallback.
}

// Computes the total cost to reduce n to 1 by repeatedly adding the largest proper divisor
// and recursing on that divisor. Base case: n == 1 returns 0.
// For n > 1, result = largestProperDivisor(n) + minStepsToReachN(largestProperDivisor(n)).
int minStepsToReachN(int n) {
    if (n == 1) {
        return 0;
    }
    int next = largestProperDivisor(n);
    return next + minStepsToReachN(next);
}
int main() {
    // Base case
    assert(minStepsToReachN(1) == 0);
    // n = 2: largest divisor = 1, then 1 -> 0, total = 1
    assert(minStepsToReachN(2) == 1);
    // n = 3: largest divisor = 1, total = 1
    assert(minStepsToReachN(3) == 1);
    // n = 4: largest divisor = 2, then 2 -> 1, total = 2 + 1 = 3
    assert(minStepsToReachN(4) == 3);
    // n = 6: largest divisor = 3, then 3 -> 1, total = 3 + 1 = 4
    assert(minStepsToReachN(6) == 4);
    // n = 8: largest divisor = 4, then 4 -> 3, then 3 -> 1, total = 4 + 3 + 1 = 8
    assert(minStepsToReachN(8) == 8);
    // n = 9: largest divisor = 3, then 3 -> 1, total = 3 + 1 = 4
    assert(minStepsToReachN(9) == 4);
    // n = 27: largest divisor = 9, then 9 -> 3, then 3 -> 1, total = 9 + 3 + 1 = 13
    assert(minStepsToReachN(27) == 13);
    // n = 12: largest divisor = 6, then 6 -> 3, then 3 -> 1, total = 6 + 3 + 1 = 10
    assert(minStepsToReachN(12) == 10);
    // n = 16: largest divisor = 8, then 8 -> 8? Wait, 8->4->2->1, total = 8+4+2+1=15
    assert(minStepsToReachN(16) == 15);
    return 0;
}
// The core algorithm mirrors the recursive process in the snippet. For a given `n`, if `n == 1`, the answer is 0 because we are already at the target. Otherwise, we must find the largest proper divisor of `n` (a divisor `i` where `1 <= i < n` and `n % i == 0`). The largest proper divisor is always `n / smallest_prime_factor`, but the simplest approach is to iterate downward from `n/2` until a divisor is found, which is what the snippet does. Then the result for `n` is `largestProperDivisor + solve(largestProperDivisor)`. This recursion continues until reaching 1. The key edge case is `n == 1` (base case). Also, note that for prime `n`, the largest proper divisor is 1, so the recursion goes to 1 and adds 1. The recursion is guaranteed to terminate because `largestProperDivisor < n` for all `n > 1`. Time complexity: In the worst case, finding the largest proper divisor by linear scan is O(n), and this is done recursively for each number in the chain, giving O(n * chain_length) which is O(n^2) in the worst case (e.g., for n that is a power of 2, chain length is log n, but for primes, chain length is 2). But for typical constraints, it's acceptable. More efficient divisor-finding could use sqrt, but we stay faithful to the snippet. Space complexity is O(chain length) due to recursion stack, at most O(log n) for many numbers but could be O(n) for n=2? Actually chain length for n=2 is 2, for n=3 is 2, for n=4: 4→2→1, length 3. So recursion depth is at most about O(log n) for composite numbers with small factors, but for worst-case like n=2^k, depth is k, so O(log n). So space O(log n) typically, but we must mention it.
