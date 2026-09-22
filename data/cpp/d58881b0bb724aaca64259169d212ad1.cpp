// Write a standalone C++ function that computes the binomial coefficient \( \binom{n}{r} \) (n choose r) for non-negative integers \( n \) and \( r \), where \( 0 \le r \le n \). The function must take two integer parameters, compute the result using the multiplicative formula to avoid large intermediate factorials, and return the result as an `int`. You must handle the edge cases where \( r = 0 \) or \( r = n \) (result is 1), and assume inputs are always valid (non-negative and \( r \le n \)). Do not include a `main` function in your solution; only provide the function definition.

// The straightforward approach from the snippet computes three factorials separately (n!, r!, (n-r)!) and then divides, but this is inefficient and risks overflow even for moderate n (e.g., n=20) because intermediate factorial values grow extremely fast. A better approach uses the multiplicative formula: \( \binom{n}{r} = \prod_{i=1}^{r} \frac{n - r + i}{i} \). Since \( \binom{n}{r} = \binom{n}{n-r} \), we reduce r to the smaller of r and n-r to minimize iterations. The algorithm works as follows: if r > n - r, set r = n - r. Initialize result = 1. For i from 1 to r, compute result = result * (n - r + i) / i. This division is exact at each step because the intermediate product is always divisible by i (a known combinatorial identity). This avoids overflow because intermediate values are never larger than the final result (which grows slowly for typical inputs). Edge cases: r = 0 or r = n leads to result = 1 (the loop runs zero times). Time complexity is O(min(r, n-r)), which is O(n) in worst case when r ≈ n/2. Space complexity is O(1). The function must be `const`-correct in the sense that parameters are passed by value (no modification needed), and we can mark them `const` inside the function if desired.

#include <algorithm> // for std::min

// Compute the binomial coefficient C(n, r) for 0 <= r <= n.
// Uses the multiplicative formula with integer division to avoid large intermediate factorials.
int binomialCoefficient(int n, int r) {
    // Reduce r to the smaller side for fewer iterations.
    r = std::min(r, n - r);
    
    int result = 1;
    for (int i = 1; i <= r; ++i) {
        // Multiply and divide in each step; division is exact at each step.
        result = result * (n - r + i) / i;
    }
    return result;
}

#include <cassert>

// Declaration of the function under test (provided elsewhere).
int binomialCoefficient(int n, int r);

int main() {
    // Basic cases
    assert(binomialCoefficient(5, 2) == 10);
    assert(binomialCoefficient(5, 3) == 10); // symmetric
    assert(binomialCoefficient(10, 0) == 1);
    assert(binomialCoefficient(10, 10) == 1);
    assert(binomialCoefficient(0, 0) == 1);
    // Larger values
    assert(binomialCoefficient(10, 5) == 252);
    assert(binomialCoefficient(20, 10) == 184756);
    assert(binomialCoefficient(30, 15) == 155117520);
    // Edge with r > n/2 (tests min reduction)
    assert(binomialCoefficient(7, 6) == 7);
    assert(binomialCoefficient(4, 3) == 4);
    return 0;
}
