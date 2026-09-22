/*
Write a C++ function named `computeBinomialCoefficient` that takes two integers `n` and `r` as parameters and returns the binomial coefficient \( \binom{n}{r} \) (often written as nCr) as an integer. The function must handle the mathematical definition where \( \binom{n}{r} = \frac{n!}{r! \cdot (n-r)!} \). However, to avoid computing large factorials directly (which can cause integer overflow even for moderate values like 20!), your implementation should compute the result using a multiplicative formula that cancels common factors: start with a result of 1 and multiply by `(n - i)` then divide by `(i + 1)` for `i` from 0 to `r-1`. The function should return 0 if `r < 0` or `r > n` (since the binomial coefficient is defined as 0 in these cases, except for the special case where both are 0, which should return 1). You must also ensure the function is marked as `const`-correct (use `const` for parameters that are not modified) and include appropriate header files. The function should be efficient and avoid overflow for reasonable input ranges (e.g., up to n=60, r=30) by performing the multiplication and division at each step in a way that keeps intermediate results as small as possible.
*/
#include <algorithm>  // for std::min

// Compute the binomial coefficient nCr using a multiplicative approach.
// Returns 0 if r < 0 or r > n; returns 1 for n=0, r=0.
unsigned long long computeBinomialCoefficient(const int n, const int r) {
    if (r < 0 || r > n) {
        return 0ULL;
    }
    if (r == 0 || r == n) {
        return 1ULL;
    }

    // Use symmetry to reduce number of iterations: nCr = nC(n-r)
    const int k = std::min(r, n - r);
    unsigned long long result = 1ULL;

    for (int i = 1; i <= k; ++i) {
        result *= static_cast<unsigned long long>(n - k + i);  // multiply by next numerator factor
        result /= static_cast<unsigned long long>(i);          // divide by denominator factor i
    }

    return result;
}
#include <cassert>

int main() {
    // Basic cases
    assert(computeBinomialCoefficient(5, 2) == 10ULL);
    assert(computeBinomialCoefficient(5, 3) == 10ULL);  // symmetry
    assert(computeBinomialCoefficient(6, 0) == 1ULL);
    assert(computeBinomialCoefficient(6, 6) == 1ULL);
    assert(computeBinomialCoefficient(0, 0) == 1ULL);

    // Edge cases: invalid inputs return 0
    assert(computeBinomialCoefficient(5, -1) == 0ULL);
    assert(computeBinomialCoefficient(5, 7) == 0ULL);

    // Larger values to verify no overflow up to n=60, r=30
    assert(computeBinomialCoefficient(60, 30) == 118264581564861424ULL);
    assert(computeBinomialCoefficient(50, 25) == 126410606437752ULL);

    // Another symmetry check
    assert(computeBinomialCoefficient(10, 4) == computeBinomialCoefficient(10, 6));

    return 0;
}
// The main algorithm uses the multiplicative formula for binomial coefficients: \( \binom{n}{r} = \prod_{i=1}^{r} \frac{n - r + i}{i} \). This computes the value in O(r) time, rather than computing three factorials separately (which would be O(n) time and risk overflow). The key idea is to iteratively update a running result: for each `i` from 1 to `r`, multiply by `(n - r + i)` and then divide by `i`. Performing the division immediately after each multiplication keeps the intermediate result as an integer (since the product of the first i terms is always divisible by i! ) and minimizes the magnitude of intermediate values. Edge cases: if `r` is negative or greater than `n`, the binomial coefficient is defined as 0 (except for n=0, r=0, which equals 1). We also handle the symmetry property by setting `r = min(r, n - r)` to reduce the number of iterations, thus improving performance and reducing overflow risk. Time complexity is O(min(r, n-r)), which is O(n) in the worst case, and space complexity is O(1) since we only use a few integer variables.
