Write a C++ function named `harmonic_series_sum` that takes a positive integer `n` and returns a `double` representing the sum of the series 1/1 + 1/2 + 1/3 + ... + 1/n. The function must compute the sum iteratively in natural order (from 1 to n) as shown in the original snippet. Handle the edge case where `n` is 0 by returning 0.0. The function must be `const`-correct (i.e., it should not modify the input parameter) and should not print anything to the console—it must only return the computed value. Assume the input is always a non-negative integer, but still handle 0 gracefully.
// The main algorithm initializes a `double` accumulator to 0.0 and a loop counter to 1. Then, for each integer `i` from 1 to `n`, it adds `1.0 / i` to the accumulator. Using `1.0 / i` (instead of `1 / i`) is crucial to perform floating-point division rather than integer division, which would truncate to 0 for all `i > 1`. The edge case `n == 0` is naturally handled because the loop body never executes, so the accumulator remains 0.0. The time complexity is O(n) because the loop runs exactly `n` iterations. The space complexity is O(1) since only a constant number of variables are used (the accumulator and loop index). No large intermediate storage is required. Note that for very large `n`, floating-point rounding errors may accumulate, but this is inherent to the iterative summation and acceptable for typical educational tasks.
// Compute the sum of the harmonic series: 1/1 + 1/2 + ... + 1/n.
// Returns 0.0 for n == 0.
double harmonic_series_sum(const int n) {
    double sum = 0.0;
    for (int i = 1; i <= n; ++i) {
        sum += 1.0 / i;  // Use 1.0 to force floating-point division.
    }
    return sum;
}
int main() {
    // n = 0 edge case
    assert(harmonic_series_sum(0) == 0.0);
    // n = 1: only 1/1
    assert(harmonic_series_sum(1) == 1.0);
    // n = 2: 1 + 1/2 = 1.5
    assert(harmonic_series_sum(2) == 1.5);
    // n = 3: 1 + 1/2 + 1/3 = 1.833333...
    assert(harmonic_series_sum(3) == 1.0 + 0.5 + 1.0/3.0);
    // n = 10: sum = 2.9289682539682538 (exact double representation)
    double expected_10 = 0.0;
    for (int i = 1; i <= 10; ++i) expected_10 += 1.0 / i;
    assert(harmonic_series_sum(10) == expected_10);
    // Verify with a few manual values using == (exact for these small cases)
    assert(harmonic_series_sum(4) >= 2.083333 && harmonic_series_sum(4) <= 2.083334);
    return 0;
}
