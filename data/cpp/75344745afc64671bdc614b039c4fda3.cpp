/*
Write a C++ function named `isPerfectNumber(long long n)` that determines whether a given positive integer `n` is a perfect number. A perfect number is a positive integer that equals the sum of its proper positive divisors (all divisors excluding the number itself). For example, 6 is perfect because 1 + 2 + 3 = 6, while 12 is not because 1 + 2 + 3 + 4 + 6 = 16 ≠ 12. The function should return `true` if `n` is perfect and `false` otherwise. The input may be as large as 10^12, so the solution must be efficient—do not iterate through all numbers up to `n`. Instead, iterate only up to the square root of `n` to find all divisors and sum them. Handle the special case where `n` equals 1 (which is not perfect, since 1 has no proper divisors other than itself, so the sum is 0). Also, ensure that when you find a divisor `d`, you also add `n / d` if `d` is not equal to the square root, to avoid double-counting. The function must be `const`-correct for its parameter.
*/
#include <cmath>

// Determine if a positive integer n is a perfect number (sum of proper divisors equals n).
bool isPerfectNumber(const long long n) {
    if (n <= 1) {
        return false; // 1 has no proper divisors sum to 0; non-positive not expected.
    }

    long long sum = 1; // 1 is always a proper divisor for n > 1
    const long long limit = static_cast<long long>(std::sqrt(n));

    for (long long d = 2; d <= limit; ++d) {
        if (n % d == 0) {
            sum += d; // add the divisor
            const long long other = n / d;
            if (other != d) {
                sum += other; // add the paired divisor if distinct
            }
        }
    }

    return sum == n;
}
#include <cassert>

int main() {
    // Known perfect numbers
    assert(isPerfectNumber(6) == true);
    assert(isPerfectNumber(28) == true);
    assert(isPerfectNumber(496) == true);
    assert(isPerfectNumber(8128) == true);

    // Non-perfect numbers and edge cases
    assert(isPerfectNumber(1) == false);
    assert(isPerfectNumber(2) == false);
    assert(isPerfectNumber(12) == false);
    assert(isPerfectNumber(36) == false); // square, divisor 6 counted once
    assert(isPerfectNumber(1000000) == false); // large non-perfect
    assert(isPerfectNumber(33550336) == true); // 5th perfect number
    assert(isPerfectNumber(999999999999) == false); // near 10^12 non-perfect
}
// The algorithm is based on the mathematical property that if `d` is a divisor of `n`, then `n / d` is also a divisor. Therefore, we only need to check divisors from 2 up to the floor of the square root of `n`. For each divisor `d` that divides `n` evenly, we add `d` to the sum, and if `n / d` is different from `d`, we also add `n / d`. We start the sum at 1 (since 1 is always a proper divisor for `n > 1`), but for `n = 1`, the sum of proper divisors is 0 (1's only divisor is itself, which is excluded). So we handle `n <= 1` separately by returning `false`. For perfect numbers, the sum must equal `n`. The loop runs only until `sqrt(n)`, so the time complexity is O(√n), which for `n` up to 10^12 is about 10^6 iterations, which is acceptable. The space complexity is O(1) since we only use a few variables. The function should be declared with `bool` return type and take a `long long` parameter by value. Edge cases include `n = 1`, `n` being a perfect square (e.g., 36 where the square root 6 is counted once), and large even numbers (perfect numbers are all even, but the function must still work for any positive integer).
