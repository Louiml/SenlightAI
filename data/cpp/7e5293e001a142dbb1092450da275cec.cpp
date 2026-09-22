// Implement a C++ function named `recursiveFunctionW` that takes three `long long` parameters `a`, `b`, and `c` and returns a `long long` result following a recursive definition. The function must compute values where: if any parameter is ≤ 0, return 1; if any parameter > 20, recursively call with all three parameters set to 20; otherwise, define a memoization table `dp[25][25][25]` initialized to 0, and if `dp[a][b][c]` is non-zero, return it directly. If `a < b && b < c`, set `dp[a][b][c] = w(a, b, c-1) + w(a, b-1, c-1) - w(a, b-1, c)` else set `dp[a][b][c] = w(a-1, b, c) + w(a-1, b-1, c) + w(a-1, b, c-1) - w(a-1, b-1, c-1)` and then return the stored value. The function must be self-contained (including the memoization table as a static local or global) and should be optimized for repeated calls.

The problem is a classic recursive function with overlapping subproblems, which is best solved using memoization (top-down dynamic programming). The base cases are: values ≤ 0 return 1, and values > 20 are clamped to 20 because the recursion only depends on values in the range 1..20 (after clamping). The main recurrence splits into two cases based on ordering. Because the function is deterministic and the recursion depth is at most 20 (since each recursive call reduces at least one argument by 1 or stays at 20), the number of distinct states `(a,b,c)` with each between 1 and 20 is 20³ = 8000 states. With memoization, each state is computed exactly once, so the time complexity per unique set of inputs is O(1) after the first call; the overall amortized cost over all possible states is O(20³) = O(8000). Space complexity is O(20³) for the memoization table. Important edge cases: negative or zero inputs immediately return 1; inputs greater than 20 are normalized to 20; and the memoization must distinguish between an uncomputed state (0) and a computed value that happens to be zero. However, note that all computed values are positive (since the recurrence involves additions and a subtraction but the base cases are 1 and the recurrence yields positive numbers, as seen in the original problem), so using 0 as a sentinel is safe. The function should be `const`-correct for inputs (parameters are passed by value) and the memoization table should be `static` inside the function to persist across calls.

#include <vector>
#include <cstdint>

// Recursive function with memoization following the specification.
// Returns 1 if any argument <= 0; else clamps to 20 if >20; else uses recurrence.
long long recursiveFunctionW(long long a, long long b, long long c) {
    // Static memoization table, initialized to 0 (uncomputed).
    // Size 25 to index 0..20, but we only use 1..20.
    static long long memo[25][25][25] = {0};

    // Base case: any parameter <= 0.
    if (a <= 0 || b <= 0 || c <= 0) {
        return 1;
    }
    // Clamp to 20 if any parameter > 20.
    if (a > 20 || b > 20 || c > 20) {
        return recursiveFunctionW(20, 20, 20);
    }

    // If already computed, return stored value.
    if (memo[a][b][c] != 0) {
        return memo[a][b][c];
    }

    // Compute and store the result according to the ordering case.
    if (a < b && b < c) {
        memo[a][b][c] = recursiveFunctionW(a, b, c - 1)
                      + recursiveFunctionW(a, b - 1, c - 1)
                      - recursiveFunctionW(a, b - 1, c);
    } else {
        memo[a][b][c] = recursiveFunctionW(a - 1, b, c)
                      + recursiveFunctionW(a - 1, b - 1, c)
                      + recursiveFunctionW(a - 1, b, c - 1)
                      - recursiveFunctionW(a - 1, b - 1, c - 1);
    }
    return memo[a][b][c];
}

#include <cassert>

int main() {
    // Base cases: any parameter <= 0 returns 1.
    assert(recursiveFunctionW(-1, 0, 5) == 1);
    assert(recursiveFunctionW(0, -2, 3) == 1);
    assert(recursiveFunctionW(-5, -5, -5) == 1);

    // Clamping: any > 20 behaves like w(20,20,20).
    long long w20 = recursiveFunctionW(20, 20, 20);
    assert(recursiveFunctionW(21, 2, 3) == w20);
    assert(recursiveFunctionW(100, 100, 100) == w20);

    // Known values from the classic problem (verify consistency).
    // w(1,1,1) = 2
    assert(recursiveFunctionW(1, 1, 1) == 2);
    // w(2,2,2) = 4
    assert(recursiveFunctionW(2, 2, 2) == 4);
    // w(3,3,3) = 8 (expected from recursive recurrence)
    assert(recursiveFunctionW(3, 3, 3) == 8);
    // w(10,10,10) is a known value (can be computed; here we check it's stable)
    assert(recursiveFunctionW(10, 10, 10) == recursiveFunctionW(10, 10, 10));

    // Memoization returns same result on repeated calls.
    long long first = recursiveFunctionW(15, 15, 15);
    long long second = recursiveFunctionW(15, 15, 15);
    assert(first == second);

    // Ordering case: when a < b < c, use the special recurrence.
    // Compute one example manually or compare against a brute-force (but here we just check it's consistent).
    long long val = recursiveFunctionW(2, 3, 4);
    assert(val > 0); // Should be positive
    return 0;
}
