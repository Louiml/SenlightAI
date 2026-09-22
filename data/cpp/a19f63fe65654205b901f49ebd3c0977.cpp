// Write a C++ function `sumOfDivisorsUpTo(int N)` that takes a positive integer `N` and returns the sum over all integers `i` from `1` to `N` of the sum of all positive divisors of `i`. That is, compute `∑_{i=1}^{N} ∑_{d|i} d`. The result may be large, so return a `long long`. For example, if `N = 4`, divisors are: `1→1`, `2→1+2=3`, `3→1+3=4`, `4→1+2+4=7`, so total = `1+3+4+7 = 15`. Do not iterate over each divisor individually for each number (naive O(N√N)), but instead use an efficient counting approach.
#include <cassert>

int main() {
    assert(sumOfDivisorsUpTo(1) == 1);
    assert(sumOfDivisorsUpTo(2) == 4);   // 1 + (1+2) = 1+3 = 4
    assert(sumOfDivisorsUpTo(3) == 8);   // 1 + 3 + 4 = 8
    assert(sumOfDivisorsUpTo(4) == 15);  // 1+3+4+7 = 15
    assert(sumOfDivisorsUpTo(5) == 21);  // 15 + (1+5) = 21
    assert(sumOfDivisorsUpTo(6) == 33);  // 21 + (1+2+3+6)=33
    assert(sumOfDivisorsUpTo(10) == 87); // calculated manually or via known sequence
    assert(sumOfDivisorsUpTo(100) == 48250);
    return 0;
}
#include <cstddef>

// Compute the sum over all i from 1 to N of the sum of divisors of i.
// Returns the result as a long long.
long long sumOfDivisorsUpTo(int N) {
    long long total = 0;
    for (int d = 1; d <= N; ++d) {
        total += static_cast<long long>(d) * (N / d);
    }
    return total;
}
// The key observation is to change the order of summation: instead of summing divisors for each number, we count how many times each divisor `d` appears in the range `1..N`. For a given divisor `d`, it divides every multiple of `d` up to `N`, so there are exactly `floor(N/d)` numbers in that range that have `d` as a divisor. Therefore, the total contribution of `d` to the final sum is `d * floor(N/d)`. Summing this over all `d` from `1` to `N` gives the answer: `∑_{d=1}^{N} d * (N / d)` (using integer division). This runs in O(N) time and O(1) auxiliary space. Edge cases: when `N = 1`, the sum is `1`; when `N` is large (up to e.g. `10^7`), the product `d * (N/d)` fits in `long long` but avoid overflow by computing with `long long` types. The formula handles all positive integers correctly, and negative or zero inputs are not expected per specification.
