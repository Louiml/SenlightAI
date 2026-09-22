Write a C++ function named `areAmicable` that takes two positive integers `a` and `b` as parameters and returns a boolean indicating whether they form an amicable pair. An amicable pair is defined as two distinct positive integers where the sum of the proper divisors of each equals the other number. The sum of proper divisors of a number `x` includes all positive divisors of `x` except `x` itself (e.g., for 6, proper divisors are 1, 2, 3, sum = 6). The function must handle numbers up to 100,000 efficiently and must not include the number itself in its divisor sum. Assume the inputs are already validated as positive integers, but the function should still be correct for all positive inputs. The solution must be self-contained, use only standard C++ libraries, and apply `const` correctness where appropriate.

// The solution computes the sum of proper divisors for each number using a trial-division loop that runs only up to the square root of the number. For each divisor `i` found (starting from 2), we add `i` and, if `i` is not equal to `x/i`, we also add `x/i`. We initialize the sum to 1 (since 1 is always a proper divisor for any positive integer greater than 1). We skip `i=1` in the loop to avoid double-counting. After computing both sums, we check if `sum_a == b` and `sum_b == a`, which ensures the numbers are distinct because if `a == b`, then `sum_a == a` would imply a perfect number, not an amicable pair (and the problem context assumes distinctness, but the check still returns false for perfect numbers like 6,6 because sum_a=6==b but sum_b=6==a, so it would wrongly return true; to ensure distinctness, we should also require `a != b`). Edge cases: for 1, the sum of proper divisors is 0, so `areAmicable(1,0)` is false because inputs are positive (0 not allowed). The loop runs `O(sqrt(n))` per number, so total time is `O(sqrt(a) + sqrt(b))`, with constant space. For numbers up to 100,000, the maximum sqrt is about 316, so this is very fast. We must also handle the case where `x` is 1: the loop condition `i*i <= x` is false, and sum initialized to 1 would be wrong because 1 has no proper divisors (sum=0). So we add a special case: if `x == 1`, return 0.

#include <cstdint>  // for int64_t to avoid overflow on divisor sums

// Returns the sum of proper divisors of x (all divisors except x itself).
// For x == 1, returns 0. Assumes x > 0.
int64_t sumProperDivisors(int64_t x) {
    if (x == 1) {
        return 0;
    }
    int64_t sum = 1;  // 1 is always a proper divisor for x > 1
    // Check divisors from 2 up to sqrt(x)
    for (int64_t i = 2; i * i <= x; ++i) {
        if (x % i == 0) {
            sum += i;
            if (i != x / i) {
                sum += x / i;
            }
        }
    }
    return sum;
}

// Returns true if a and b form an amicable pair (a != b and each equals the other's proper divisor sum).
bool areAmicable(int64_t a, int64_t b) {
    // Amicable numbers must be distinct
    if (a == b) {
        return false;
    }
    return sumProperDivisors(a) == b && sumProperDivisors(b) == a;
}

int main() {
    // Classic amicable pair: 220 and 284
    assert(areAmicable(220, 284) == true);
    // Classic amicable pair: 1184 and 1210
    assert(areAmicable(1184, 1210) == true);
    // Not amicable: different sums
    assert(areAmicable(6, 6) == false);      // perfect number, same number
    assert(areAmicable(28, 28) == false);    // perfect number
    assert(areAmicable(1, 1) == false);      // both have sum 0
    assert(areAmicable(1, 0) == false);      // 0 not valid, but just in case
    assert(areAmicable(10, 8) == false);     // 10's sum=8, but 8's sum=7
    assert(areAmicable(220, 221) == false);  // mismatched
    assert(areAmicable(2620, 2924) == true); // another known pair
    assert(areAmicable(5020, 5564) == true); // another known pair
    return 0;
}
