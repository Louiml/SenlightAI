Write a C++ function named `sumOfAmicableNumbersBelow` that takes an integer limit `N` (with `N > 0`) and returns the sum of all amicable numbers less than `N`. Two distinct positive integers `a` and `b` are amicable if the sum of the proper divisors of `a` equals `b`, and the sum of the proper divisors of `b` equals `a`, with `a != b`. For example, 220 and 284 are amicable because the proper divisors of 220 (1, 2, 4, 5, 10, 11, 20, 22, 44, 55, 110) sum to 284, and the proper divisors of 284 (1, 2, 4, 71, 142) sum to 220. The function should compute the sum efficiently for `N` up to 100,000 and return it as an `int`. You must include a helper function to compute the sum of proper divisors of a given number.
The solution requires computing the sum of proper divisors for each number from 2 to `N-1` (since 1 has no proper divisors other than 0, but we treat it as having sum 0). A straightforward approach is to precompute all divisor sums in one pass using a sieve-like method: for each `i` from 1 to `N-1`, add `i` to all multiples of `i` (excluding the number itself). This yields the sum of proper divisors for every number in `O(N log N)` time. Then, iterate through all numbers `a` from 2 to `N-1`; if `sumDiv[a] > a`, check whether `sumDiv[a] < N` and `sumDiv[sumDiv[a]] == a`. If so, `a` and `sumDiv[a]` form an amicable pair, and both are added to the total sum, ensuring each pair is counted once. Edge cases: numbers where the sum exceeds `N` or equals itself (perfect numbers) must be excluded; the function should return 0 if no amicable numbers exist below `N`. Time complexity is `O(N log N)` for the sieve plus `O(N)` for the scan, and space complexity is `O(N)` for the divisor sum array.
#include <vector>

namespace {
    // Compute the sum of proper divisors for all numbers up to limit using a sieve.
    std::vector<int> computeProperDivisorSums(int limit) {
        std::vector<int> divisorSum(limit, 0);
        // For each i, add i to all multiples of i greater than i.
        for (int i = 1; i < limit; ++i) {
            for (int multiple = 2 * i; multiple < limit; multiple += i) {
                divisorSum[multiple] += i;
            }
        }
        return divisorSum;
    }
}

// Returns the sum of all amicable numbers less than N.
int sumOfAmicableNumbersBelow(int N) {
    if (N <= 2) {
        return 0; // No amicable numbers below 3.
    }

    std::vector<int> sumDiv = computeProperDivisorSums(N);
    int totalSum = 0;

    for (int a = 2; a < N; ++a) {
        int b = sumDiv[a];
        // Only consider pairs where b > a to avoid double counting.
        if (b > a && b < N && sumDiv[b] == a) {
            totalSum += a + b;
        }
    }

    return totalSum;
}
#include <cassert>

int main() {
    // Small limits with no amicable numbers.
    assert(sumOfAmicableNumbersBelow(1) == 0);
    assert(sumOfAmicableNumbersBelow(2) == 0);
    assert(sumOfAmicableNumbersBelow(219) == 0);

    // The first amicable pair is 220 and 284.
    assert(sumOfAmicableNumbersBelow(220) == 0);      // 284 is not below 220.
    assert(sumOfAmicableNumbersBelow(221) == 220);    // 220 is included, 284 not.
    assert(sumOfAmicableNumbersBelow(284) == 220);    // 220 is below, 284 not.
    assert(sumOfAmicableNumbersBelow(285) == 504);    // 220 + 284.

    // Next amicable pair is 1184 and 1210.
    assert(sumOfAmicableNumbersBelow(1210) == 504);   // only 220+284, since 1184<1210.
    assert(sumOfAmicableNumbersBelow(1211) == 2898);  // 220+284+1184+1210.

    // Perfect numbers (like 6, 28) should not be counted.
    assert(sumOfAmicableNumbersBelow(1000) == 504);   // Only 220 and 284.

    // Larger limit from Project Euler (under 10000).
    assert(sumOfAmicableNumbersBelow(10000) == 31626);

    return 0;
}
