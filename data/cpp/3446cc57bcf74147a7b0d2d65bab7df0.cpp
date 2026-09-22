// Write a C++ function that takes an integer `n` as input and returns a `std::vector<long long>` containing, for each integer `x` from 1 to `n` (inclusive), the ceiling of the base‑10 logarithm of `x!` (factorial of `x`). For `x = 1`, the result is defined as 1. The function should compute the logarithm by summing `log10(k)` for `k = 1` to `x` (using `double` precision) and then applying `std::ceil`. If `x = 1`, directly return 1 without the logarithmic sum. The input `n` is a positive integer (≥1). Output the results as a vector of `long long` values. This task is designed to test your ability to handle floating‑point summation, ceiling operations, and factorial‑related magnitude estimation without overflow (since actual factorials grow extremely fast).

#include <cassert>
#include <vector>
#include <cmath>

// Include the solution function here (or copy above).

int main() {
    // n = 1: only x=1 -> 1
    assert(factorialDigitCeilings(1) == std::vector<long long>{1});

    // n = 2: 1! -> 1, 2! = 2 -> ceil(log10(2)) = 1
    assert(factorialDigitCeilings(2) == std::vector<long long>({1, 1}));

    // n = 3: 1! -> 1, 2! -> 1, 3! = 6 -> ceil(log10(6)) = 1
    assert(factorialDigitCeilings(3) == std::vector<long long>({1, 1, 1}));

    // n = 4: 4! = 24 -> ceil(log10(24)) = 2
    assert(factorialDigitCeilings(4) == std::vector<long long>({1, 1, 1, 2}));

    // n = 5: 5! = 120 -> ceil(log10(120)) = 3
    assert(factorialDigitCeilings(5) == std::vector<long long>({1, 1, 1, 2, 3}));

    // n = 10: known values (computed via exact digits: 1,2,6,24,120,720,5040,40320,362880,3628800)
    // digits: 1,1,1,2,3,3,4,5,6,7
    assert(factorialDigitCeilings(10) == std::vector<long long>({1,1,1,2,3,3,4,5,6,7}));

    // n = 20: spot check via independent calculation
    auto result = factorialDigitCeilings(20);
    assert(result.size() == 20);
    assert(result[0] == 1);
    assert(result[19] == 19); // 20! has 19 digits (verified: 20! = 2432902008176640000)
    return 0;
}

#include <vector>
#include <cmath>

// For each x from 1 to n, return ceil(log10(x!)) with special case x=1 -> 1.
std::vector<long long> factorialDigitCeilings(int n) {
    std::vector<long long> result;
    result.reserve(n);
    double logFactorial = 0.0;
    for (int x = 1; x <= n; ++x) {
        if (x == 1) {
            result.push_back(1);
        } else {
            logFactorial += std::log10(static_cast<double>(x));
            result.push_back(static_cast<long long>(std::ceil(logFactorial)));
        }
    }
    return result;
}

// The core idea is to compute the number of digits in `x!` (or an upper bound via ceiling of log10). Since `log10(x!) = sum_{k=1}^x log10(k)`, we can accumulate this sum in a `double` to avoid huge integer values. After the sum, `std::ceil` gives the smallest integer ≥ the exact value, which approximates the number of digits (with possible off‑by‑one due to floating‑point rounding, but for this task it's acceptable). For `x = 1`, `1! = 1` has 1 digit, but the formula would give `ceil(log10(1)) = ceil(0) = 0`, which is incorrect; hence a special case is needed. The algorithm runs in O(n²) time if we recompute the sum from scratch for each `x` (as in the snippet), but we can optimize to O(n) by maintaining a running sum: for each `x`, add `log10(x)` to the previous sum. However, the task specification (based on the snippet) recomputes; for clarity and efficiency, we'll provide the O(n) version. Edge cases: `x = 1` (special), large `x` (sum may be large but `double` handles up to ~1e308, so for `x` up to a few thousand it's fine; for larger, we'd need `long double`). Time complexity: O(n) with running sum, O(n²) if naive; space O(n) for the result vector. The provided solution uses a running sum for efficiency.
