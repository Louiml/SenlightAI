/*
Write a C++ function `bool isRepresentableAsGeometricSeriesSum(long long n)` that determines whether a given positive integer `n` (with `1 <= n <= 2^63 - 1`) can be expressed as the sum of a geometric series `1 + a + a^2 + ... + a^k` for some integer base `a >= 2` and integer exponent `k >= 2` (i.e., at least three terms). The function must return `true` if such a representation exists, and `false` otherwise. For example, `31` is representable as `1 + 2 + 4 + 8 + 16` (a=2, k=4), but `32` is not. Handle small numbers specially: if `n <= 6`, the answer is always `false` because the smallest possible sum is `1 + 2 + 4 = 7`. The solution should work efficiently for very large `n`, avoiding overflow by using `__int128` or early breaks.
*/

#include <cstdint>
#include <cmath>

// Determine if n can be written as 1 + a + a^2 + ... + a^k with a>=2, k>=2.
bool isRepresentableAsGeometricSeriesSum(long long n) {
    if (n <= 6) return false;

    // Check k=2 case: 1 + a + a^2 = n => a^2 + a + (1-n) = 0
    // a = floor(sqrt(n-1)) is candidate
    long long a2 = static_cast<long long>(std::sqrt(static_cast<long double>(n - 1)));
    // Try a2 and a2-1 and a2+1 to avoid precision issues
    for (long long a = a2 - 1; a <= a2 + 1; ++a) {
        if (a < 2) continue;
        __int128 sum = (__int128)a * a + a + 1;
        if (sum == n) return true;
    }

    // For k >= 3, a must satisfy a^3 <= n (since 1+a+a^2+a^3 > n if a^3 > n)
    // So iterate a up to cbrt(n)
    long long max_a = static_cast<long long>(std::cbrt(static_cast<long double>(n))) + 2;
    for (long long a = 2; a <= max_a; ++a) {
        __int128 term = 1;       // current power a^0
        __int128 sum = 0;        // cumulative series sum
        // Compute 1 + a + a^2 + ... until sum >= n or overflow risk
        while (sum <= n) {
            sum += term;
            if (sum == n) {
                // Need at least 3 terms (i.e., at least one a, so exponent >=1)
                // But we require k>=2, meaning at least 3 terms: 1, a, a^2
                // We ensure by checking that term >= a^2 (i.e., at least 3 terms added)
                // We can track term count.
                // Simpler: we start adding from term=1, then multiply, but we need to ensure at least 3 terms.
                // We'll restructure below.
                break;
            }
            if (sum > n) break;
            term *= a;
            if (term > n) break;
        }
        // The above loop is flawed. Let's do it cleanly:
        // We'll compute the series with explicit term count.
    }

    // Clean implementation:
    for (long long a = 2; a <= max_a; ++a) {
        __int128 term = 1;   // a^0
        __int128 sum = 0;
        int terms = 0;
        while (true) {
            sum += term;
            terms++;
            if (sum == n && terms >= 3) return true;  // at least 1 + a + ...
            if (sum > n) break;
            if (term > n / a) break; // avoid overflow for next multiplication
            term *= a;
        }
    }

    return false;
}

Wait, my first attempt above is messy. Let me write a clean, correct version.

#include <cassert>
#include <cstdint>

// Declaration of the function being tested
bool isRepresentableAsGeometricSeriesSum(long long n);

int main() {
    // Basic cases
    assert(isRepresentableAsGeometricSeriesSum(7) == true);  // 1+2+4
    assert(isRepresentableAsGeometricSeriesSum(8) == false);
    assert(isRepresentableAsGeometricSeriesSum(13) == true); // 1+3+9
    assert(isRepresentableAsGeometricSeriesSum(31) == true); // 1+2+4+8+16
    assert(isRepresentableAsGeometricSeriesSum(32) == false);

    // Edge cases
    assert(isRepresentableAsGeometricSeriesSum(1) == false); // n <= 6
    assert(isRepresentableAsGeometricSeriesSum(6) == false);
    assert(isRepresentableAsGeometricSeriesSum(7) == true);
    assert(isRepresentableAsGeometricSeriesSum(40) == true); // 1+3+9+27
    assert(isRepresentableAsGeometricSeriesSum(121) == true); // 1+3+9+27+81? Actually 1+3+9+27+81=121, a=3, k=4
    assert(isRepresentableAsGeometricSeriesSum(122) == false);

    // Larger values
    assert(isRepresentableAsGeometricSeriesSum(1 + 2 + 4 + 8 + 16 + 32) == true); // 63
    assert(isRepresentableAsGeometricSeriesSum(1 + 3 + 9 + 27 + 81 + 243) == true); // 364
    assert(isRepresentableAsGeometricSeriesSum(1 + 10 + 100 + 1000) == true); // 1111
    assert(isRepresentableAsGeometricSeriesSum(1112) == false);

    // Very large value that is exactly a sum: 1+2+4+...+2^62 = 2^63 - 1
    long long maxVal = (1LL << 62); // 2^62
    long long sum = 1;
    long long term = 2;
    while (term <= maxVal) {
        sum += term;
        term <<= 1;
    }
    assert(isRepresentableAsGeometricSeriesSum(sum) == true); // 1+2+...+2^62

    // Just above that sum
    assert(isRepresentableAsGeometricSeriesSum(sum + 1) == false);

    return 0;
}

// The key observation is that for any base `a >= 2`, the sum of the geometric series with `k+1` terms (from 0 to k) is `(a^(k+1) - 1) / (a - 1)`. For a given `a`, this sum grows rapidly with `k`, so we can iterate over possible bases `a` from 2 upward, and for each base, iterate over possible exponents `k` (starting from 2, because we need at least 3 terms) while the sum does not exceed `n`. The number of terms in the geometric series is limited: for the smallest base `a=2`, the sum grows exponentially, so the maximum `k` is about `log2(n)`, which is at most 62 for `n <= 2^63 - 1`. For larger bases, the maximum `k` becomes even smaller. We iterate `a` from 2 up to `sqrt(n)` (because if `a > sqrt(n)`, then `1 + a + a^2 > n` already, so no valid series), and for each `a` we compute the cumulative sum term by term to avoid overflow, checking if it equals `n`. We can stop early when the cumulative sum exceeds `n`. The special case `n <= 6` is handled immediately. Time complexity is `O(sqrt(n) * log(n))`, but since `sqrt(n)` is at most about `3e9` for maximum `n`, the actual loop bound is capped at `1e5`? No, that would be too slow. The provided snippet uses `i <= 1e5` as a practical bound, but for a robust solution, we can iterate `a` from 2 while `a * a <= n` because for any `a > sqrt(n)`, `1 + a + a^2 > n` (since `a^2 > n`), so no need to check. However, for `n` up to `2^63-1`, `sqrt(n)` is about `3e9`, which is too large to iterate over. The correct optimization is to note that for `k=2`, the sum is `a^2 + a + 1`, so we only need to check `a` up to about `sqrt(n)`. But again, `sqrt(9e18)` is about `3e9`, which is still too large. The provided snippet uses `1e5` as a heuristic because `a=1e5` gives `a^2 + a + 1` about `1e10`, which is much smaller than the maximum `n`. But that is not a complete solution. A better approach: for each `a`, we only need to iterate `k` while `1 + a + ... + a^k <= n`, and since `a` grows, the number of valid `a` is actually much smaller than `sqrt(n)` because even for `k=2`, we need `a^2 + a + 1 <= n`, so `a <= sqrt(n)`. But we can optimize by iterating over `k` first: for each exponent `k >= 2`, the base `a` must be at least 2 and at most `n^(1/k)`, which is much smaller. For `k=2`, `a` can be up to `sqrt(n)` (about 3e9), which is still huge, so we need a faster way to check for `k=2` specifically: solve `a^2 + a + 1 = n` by checking if `a = floor(sqrt(n-1))` satisfies the equation. For `k >= 3`, the number of valid `a` is tiny because `a^3` grows quickly, so we can iterate `a` from 2 while `a^3 <= n` (i.e., `a <= n^(1/3)`), which is at most about `2e6` for `n=9e18`, which is feasible. So the algorithm: handle `n <= 6` as false. Then check `k=2` by solving the quadratic. For `k >= 3`, iterate `a` from 2 up to `cbrt(n)` (since `a^3 <= n` for the smallest series with three terms), and for each `a`, compute the sum by repeated multiplication using `__int128` to avoid overflow, break if sum exceeds `n`. If sum equals `n`, return true. Also, we can pre-check for `a=2` up to `1e6`? No. But the provided snippet uses a loop up to `1e5`, which is not fully correct but works for many test cases. For a rigorous solution, we can combine: for `a` from 2 to `1e6` (since `(1e6)^3 = 1e18`, which covers a good range) and also solve the `k=2` case exactly. Actually, for `k=3`, `a^3 + a^2 + a + 1` grows even faster, so the maximum `a` for `k=3` is about `n^(1/3)`, which is about `2e6` for `n=9e18`. So iterating `a` up to `2e6` is fine (2 million loops). For `k=2`, we can solve directly. The overall complexity is `O(n^(1/3))` time and `O(1)` space. Edge cases: `n=7` (2^2+2+1=7) true, `n=8` false (no series), `n=13` (1+3+9=13) true, large powers like `n=1+2+4+...+2^62` should be true. The main challenge is overflow: use `__int128` for intermediate products.
