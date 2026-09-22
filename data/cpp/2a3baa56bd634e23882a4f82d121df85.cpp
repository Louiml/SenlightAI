// Write a C++ function named `printPrimesUpTo` that takes an integer `limit` (where `limit >= 2`) and returns a `std::string` containing all prime numbers from 2 up to and including `limit`, separated by a single tab character (`'\t'`), with no leading or trailing whitespace. For example, for `limit = 10`, the function must return `"2\t3\t5\t7"`. The function must use the trial-division algorithm with optimization: check divisors only up to `i / 2` (as in the snippet) or `sqrt(i)`, whichever you prefer, but ensure correctness for `limit` values up to at least 10,000.

The core algorithm mirrors the provided snippet: iterate from `i = 2` to `limit`, and for each `i`, test primality by checking divisibility against all integers `j` from 2 up to `i / 2`. If any `j` divides `i` evenly, `i` is not prime and we break early. For `i` that pass the inner loop, append `i` to the result string, adding a tab before each number except the first (to avoid trailing tabs). Edge cases: `limit = 2` returns `"2"`; the inner loop for `i = 2` runs zero times (since `i/2 = 1`), so `2` is prime. The number `1` is never checked because the outer loop starts at 2, which matches standard prime definitions. Complexity: for each `i`, we test up to `i/2` divisors in the worst case, giving a worst-case time of \(O(\sum_{i=2}^{limit} i) = O(limit^2)\). Space is \(O(limit)\) for the returned string (proportional to number of primes found). For `limit = 10000`, this is acceptable. The `limit == 1` case is excluded by the problem constraints, but if needed, we can guard against it by returning an empty string; however, the task spec says `limit >= 2`.

#include <string>

// Return a tab-separated string of all prime numbers from 2 to limit inclusive.
std::string printPrimesUpTo(const int limit) {
    std::string result;

    for (int i = 2; i <= limit; ++i) {
        bool isPrime = true;

        // Check divisors from 2 up to i/2.
        for (int j = 2; j <= i / 2; ++j) {
            if (i % j == 0) {
                isPrime = false;
                break;
            }
        }

        if (isPrime) {
            if (!result.empty()) {
                result += '\t';
            }
            result += std::to_string(i);
        }
    }

    return result;
}

#include <cassert>
#include <string>

// The solution function is assumed to be declared above.

int main() {
    assert(printPrimesUpTo(2) == "2");
    assert(printPrimesUpTo(5) == "2\t3\t5");
    assert(printPrimesUpTo(10) == "2\t3\t5\t7");
    assert(printPrimesUpTo(20) == "2\t3\t5\t7\t11\t13\t17\t19");
    assert(printPrimesUpTo(30) == "2\t3\t5\t7\t11\t13\t17\t19\t23\t29");
    assert(printPrimesUpTo(100) == "2\t3\t5\t7\t11\t13\t17\t19\t23\t29\t31\t37\t41\t43\t47\t53\t59\t61\t67\t71\t73\t79\t83\t89\t97");
    return 0;
}
