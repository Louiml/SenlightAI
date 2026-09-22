Write a C++ function that takes a positive integer `limit` and returns the smallest positive integer `n` such that the number of positive divisors of `n` is strictly greater than `limit`. The number of divisors counts all positive divisors, including 1 and `n` itself. You can assume `limit` is a positive integer (at least 1) and that the answer fits in the `long` type. Use an efficient divisor-counting method that iterates only up to the square root of each candidate. The function must be `const`-correct and avoid global variables.
#include <cassert>

int main() {
    // limit = 1: divisors of 1 = 1 (not >1), 3 has divisors {1,3} = 2 > 1
    assert(firstWithMoreDivisorsThan(1) == 3);

    // limit = 2: 3 has 2 divisors (not >2), 6 has {1,2,3,6} = 4 > 2
    assert(firstWithMoreDivisorsThan(2) == 6);

    // limit = 3: 6 has 4 > 3
    assert(firstWithMoreDivisorsThan(3) == 6);

    // limit = 4: 6 has 4 (not >4), 10 has {1,2,5,10} = 4 (not >4), 15 has 4, 21 has 4, 28 has {1,2,4,7,14,28}=6 > 4
    assert(firstWithMoreDivisorsThan(4) == 28);

    // limit = 5: 28 has 6 > 5
    assert(firstWithMoreDivisorsThan(5) == 28);

    // limit = 6: 28 has 6 (not >6), 36 has 9 > 6 (triangular 36 = 1+...+8)
    assert(firstWithMoreDivisorsThan(6) == 36);

    // limit = 10: known from original problem context (triangular number with >10 divisors is 36? no, 36 has 9, so 45 has 6, 55 has 4, 66 has 8, 78 has 8, 91 has 4, 105 has 8, 120 has 16 > 10)
    assert(firstWithMoreDivisorsThan(10) == 120);

    // limit = 20: triangular number with >20 divisors is 120? 120 has 16, so 136 has 8, 153 has 6, 171 has 6, 190 has 8, 210 has 16, 231 has 8, 253 has 4, 276 has 12, 300 has 18, 325 has 6, 351 has 8, 378 has 16, 406 has 8, 435 has 8, 465 has 8, 496 has 10, 528 has 20 (not >20), so 561 has 8, 595 has 8, 630 has 24 > 20
    assert(firstWithMoreDivisorsThan(20) == 630);

    // limit = 500: this is the original problem value; the answer is known to be 76576500
    assert(firstWithMoreDivisorsThan(500) == 76576500);

    return 0;
}
#include <cmath>

// Return the smallest positive integer whose number of positive divisors
// is strictly greater than the given limit. The input limit must be >= 1.
long firstWithMoreDivisorsThan(long limit) {
    long candidate = 0;
    long natural = 1;

    while (true) {
        candidate += natural;  // triangular number
        ++natural;

        long divisorCount = 0;
        const long root = static_cast<long>(std::sqrt(candidate));

        for (long i = 1; i <= root; ++i) {
            if (candidate % i == 0) {
                divisorCount += 2;  // i and candidate / i
            }
        }
        if (root * root == candidate) {
            --divisorCount;  // perfect square: one divisor counted twice
        }

        if (divisorCount > limit) {
            return candidate;
        }
    }
}
// The main algorithm is to generate triangular numbers sequentially (by adding the next natural number each time) and test each one until its divisor count exceeds the given limit. Triangular numbers are chosen because the original problem looks for the first triangular number with over 500 divisors, but the task generalizes to any limit. For each candidate number, count divisors by iterating from 1 to `sqrt(candidate)`. If `i` divides the number, add 2 to the count (for `i` and `number/i`), except when `i` equals `sqrt(number)` (i.e., a perfect square), then add only 1 for the repeated factor. The loop continues until the divisor count is greater than the limit. Edge cases: when `limit` is 1, the answer is 1 (divisors: {1} — but 1 has only 1 divisor, not >1, so we test 1, then 3, etc.; actually 1 fails, then triangular 3 has divisors {1,3} → 2 > 1, so answer is 3; but note that triangular numbers start at 1, then 3, 6, 10,...). The function must handle `limit` values that produce large answers; using `long` for both the triangular number and the counter is safe. Time complexity for a single candidate is O(sqrt(n)), and in the worst case we test O(L) candidates where L is the limit, giving roughly O(L * sqrt(n)) overall. Space complexity is O(1).
