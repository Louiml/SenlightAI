/*
Write a C++ function named `printGoldbachPairs` that takes an even integer `n` (with `n >= 4`) and returns a string containing all pairs of prime numbers `(p, q)` such that `p + q = n`, `p <= q`, and `p < q` (i.e., distinct primes). Each pair should be printed in the format `"p q"` and multiple pairs should be separated by a newline character (`'\n'`). The function should not print anything if no such pair exists. For example, for `n = 10`, the output string should be `"3 7\n"` (since 5+5 is not allowed because primes must be distinct). For `n = 12`, the output should be `"5 7\n"`. For `n = 6`, output should be `""` (empty string, since 3+3 is not allowed). If `n` is odd or less than 4, return an empty string. Do not use any external libraries beyond the standard headers.
*/
#include <string>
#include <cmath>

// Helper function to check if a number is prime.
bool isPrime(int number) {
    if (number < 2) return false;
    if (number == 2) return true;
    if (number % 2 == 0) return false;
    for (int divisor = 3; divisor * divisor <= number; divisor += 2) {
        if (number % divisor == 0) return false;
    }
    return true;
}

// Returns a string containing all distinct prime pairs (p, q) with p+q=n, p<q,
// each pair on a new line. Returns empty string if no such pair exists.
std::string printGoldbachPairs(int n) {
    if (n < 4 || n % 2 != 0) return "";

    std::string result;
    for (int first = 3; first <= n / 2; ++first) {
        int second = n - first;
        if (first < second && isPrime(first) && isPrime(second)) {
            if (!result.empty()) result += "\n";
            result += std::to_string(first) + " " + std::to_string(second);
        }
    }
    return result;
}
#include <cassert>
#include <string>

// Assume printGoldbachPairs is defined above.

int main() {
    // n=4: 2+2 not allowed (distinct primes required), so no pairs.
    assert(printGoldbachPairs(4) == "");

    // n=6: 3+3 not allowed (distinct), so no pairs.
    assert(printGoldbachPairs(6) == "");

    // n=8: 3+5 only.
    assert(printGoldbachPairs(8) == "3 5");

    // n=10: 3+7 only (5+5 not allowed).
    assert(printGoldbachPairs(10) == "3 7");

    // n=12: 5+7 only.
    assert(printGoldbachPairs(12) == "5 7");

    // n=14: 3+11 and 7+7 (skip 7+7) -> only 3 11.
    assert(printGoldbachPairs(14) == "3 11");

    // n=16: 3+13 and 5+11 -> two lines.
    assert(printGoldbachPairs(16) == "3 13\n5 11");

    // n=18: 5+13 and 7+11 -> two lines.
    assert(printGoldbachPairs(18) == "5 13\n7 11");

    // n=20: 3+17 and 7+13.
    assert(printGoldbachPairs(20) == "3 17\n7 13");

    // Odd n returns empty.
    assert(printGoldbachPairs(11) == "");
    assert(printGoldbachPairs(1) == "");
    assert(printGoldbachPairs(2) == "");

    return 0;
}
// The core problem is finding distinct prime pairs that sum to a given even number `n`. The original snippet iterates `i` from 3 to `n/2` (inclusive) and checks if `i` is prime using trial division up to `i`. If `i` is prime, it computes `s = n - i` and checks if `s` is also prime (and also distinct from `i`, which is guaranteed because `i <= n/2` implies `s >= i`, and equality only when `i = n/2`; for even `n`, `n/2` is an integer, but if `n/2` is prime, then `i = s`, but the original code's condition `s != k` ensures we only output when `s` is prime and `k` reaches `s`; however for `i = n/2` and `s = n/2`, the loop `k` goes from 2 to `s`, and when `k == s`, the condition `s % k == 0` is true and `s != k` is false, so it breaks, not outputting. So the original code effectively outputs only distinct pairs). The algorithm iterates over all possible first primes `i` from 3 to `n/2` (since 2 can be a prime, but if `n` is even, `n-2` is even and >2 for `n>4`, so 2 won't pair with a prime; for `n=4`, 2+2=4 but 2 and 2 are not distinct, so we skip 2 entirely). For each `i`, we check primality of `i` and `s`. Edge cases: `n` even, `n >= 4`, but if `n` is 4, no distinct prime pair (2+2 not allowed), so output empty. For `n=8`, 3+5=8, output "3 5". We must ensure we don't output both "3 5" and "5 3" – only output when `i <= s` (which is guaranteed by looping `i <= n/2`; but careful: when `n` is even, `n/2` is integer, and for `i = n/2`, `s = n/2`, so `i == s` but we skip because `i == s` not allowed). So we only consider `i` from 3 to `n/2 - 1` if `n/2` is integer, but our loop condition `i <= n/2` is fine because we check distinctness via the primality logic. We'll write a helper `isPrime` that checks numbers up to sqrt(n) for efficiency. Time complexity: O(n * sqrt(n)) in the worst case because for each `i` we do up to sqrt(i) and sqrt(n-i) checks, but overall O(n * sqrt(n)). Space complexity O(1) auxiliary, excluding the returned string which can be O(number of pairs). We'll build the result string using `std::ostringstream` or string concatenation.
