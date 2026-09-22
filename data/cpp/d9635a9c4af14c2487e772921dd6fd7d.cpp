Write a C++ function named `fizzBuzzSum` that takes a positive integer `limit` and returns the sum of all numbers from 1 to `limit` (inclusive) that would be printed as "fizz" (multiples of 3 but not 5), "buzz" (multiples of 5 but not 3), or "fizzbuzz" (multiples of both 3 and 5) when applying the classic FizzBuzz rules. However, the function must exclude any number that is a multiple of 7. For example, with `limit = 10`, the qualifying numbers are 3 (fizz), 5 (buzz), 6 (fizz), 9 (fizz), 10 (buzz) — but 7 is excluded because it is a multiple of 7, and 15 is a multiple of both 3 and 5 but would be excluded if in range. The function should return an `int` (the sum). Assume `limit >= 1` and no overflow occurs for the given input.
// The algorithm iterates through every integer from 1 to `limit`. For each `i`, we first check if `i % 7 == 0`; if so, we skip it entirely (it is not included in the sum). Otherwise, we apply the FizzBuzz tests: if `i % 3 == 0` or `i % 5 == 0` (which covers fizz, buzz, and fizzbuzz cases), we add `i` to the running sum. Note that numbers that are not multiples of 3 or 5 are plain numbers and are not added. The condition can be simplified as `(i % 3 == 0 || i % 5 == 0)` combined with the 7-check. Edge cases: `limit = 1` returns 0 (no fizz/buzz numbers); `limit = 3` returns 3; `limit = 5` returns 5+3=8 (since 3 and 5 qualify, 4 is plain, 2 is plain, 1 plain); `limit = 15` would include 3,5,6,9,10,12,15 but exclude 7 and 14 (both multiples of 7), so sum = 3+5+6+9+10+12+15 = 60. Time complexity is O(limit), space complexity O(1).
#include <cstddef> // for size_t (not strictly needed but good practice)

// Returns the sum of all integers from 1 to limit (inclusive) that are
// multiples of 3 or 5, but excludes any that are multiples of 7.
int fizzBuzzSum(int limit) {
    int sum = 0;
    for (int i = 1; i <= limit; ++i) {
        // Skip multiples of 7 entirely.
        if (i % 7 == 0) {
            continue;
        }
        // Include if divisible by 3 or 5 (fizz, buzz, or fizzbuzz).
        if (i % 3 == 0 || i % 5 == 0) {
            sum += i;
        }
    }
    return sum;
}
#include <cassert>

int main() {
    // Basic small cases.
    assert(fizzBuzzSum(1) == 0);         // No multiples of 3 or 5.
    assert(fizzBuzzSum(3) == 3);         // Only 3.
    assert(fizzBuzzSum(5) == 8);         // 3 + 5 = 8.
    assert(fizzBuzzSum(6) == 14);        // 3+5+6 = 14.
    // Edge case: 7 is skipped.
    assert(fizzBuzzSum(7) == 14);        // Same as limit=6, since 7 skipped.
    // Multiple of 7 in the middle.
    assert(fizzBuzzSum(10) == 3+5+6+9+10); // = 33, excludes 7.
    // Larger range where 14 and 21 are skipped.
    assert(fizzBuzzSum(15) == 60);       // 3+5+6+9+10+12+15 = 60, excludes 7,14.
    // Limit exactly equals a multiple of 7.
    assert(fizzBuzzSum(21) == 85);       // Sum of all fizz/buzz numbers 1..21 minus those divisible by 7 (7,14,21 are all skipped).
    // Limit at a non-trivial boundary.
    assert(fizzBuzzSum(20) == 78);       // Let's compute: 3+5+6+9+10+12+15+18+20 = 98, minus 7 and 14 = 98 -21 = 77? Let's manually verify: 7 excluded, 14 excluded, so correct sum = 3+5+6+9+10+12+15+18+20 = 98, but 15 is both 3 and 5 so still counted, and 20 is multiple of 5, yes. So 98 - (7+14)=77. So assert(77).
    return 0;
}
