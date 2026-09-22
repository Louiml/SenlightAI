Write a C++ function `std::string factorizeExpression(int n)` that takes a positive integer `n` and returns a string representing its prime factorization in the form `2^a * 3^b * 5^c ...` using the `*` symbol between factors, without spaces. For example, for `n = 12`, the output should be `"2*2*3"`; for `n = 7`, the output should be `"7"`; for `n = 1`, the output should be `"1"`. The factors must be in ascending order, and repeated primes must appear as many times as their exponent. The function should handle edge cases: `n = 0` is not expected, but if given, return `"0"`. The solution must be efficient for `n` up to `10^6`. Do not use any external libraries beyond standard C++ headers.
The task is to factorize a positive integer into its prime factors and produce a string with the factors in ascending order, separated by `*`. The main algorithm is trial division: start with divisor `d = 2` and repeatedly divide `n` by `d` while `d * d <= n`. For each divisor, if it divides `n`, append the divisor to the result, and continue dividing the remaining quotient. After the loop, if the remaining value of `n` is greater than 1, it is a prime factor and must be appended. Edge cases: `n = 1` should produce `"1"` (since 1 has no prime factors, and we treat it as the identity). `n = 0` is not valid for factorization, but the function returns `"0"` for robustness. Time complexity is \(O(\sqrt{n})\) for trial division, which is acceptable for \(n \le 10^6\). Space complexity is \(O(\log n)\) for the output string (at most about 20 characters for 10^6). The key is to correctly handle repeated factors by appending each occurrence individually, and to avoid `sqrt` function calls in the loop condition by using `d * d <= n` to prevent floating-point inaccuracies and improve performance.
#include <string>

// Returns the prime factorization of n as a string like "2*3*3" for n=18.
// For n=1 returns "1", for n=0 returns "0".
std::string factorizeExpression(int n) {
    if (n == 0) return "0";
    if (n == 1) return "1";

    std::string result;
    bool first = true;

    // Trial division for all prime factors up to sqrt(n)
    for (int d = 2; d * d <= n; ++d) {
        while (n % d == 0) {
            if (!first) result += "*";
            result += std::to_string(d);
            first = false;
            n /= d;
        }
    }

    // If n is still > 1, it's a prime factor
    if (n > 1) {
        if (!first) result += "*";
        result += std::to_string(n);
    }

    return result;
}
#include <cassert>
#include <string>

// The solution function is assumed to be defined above.
std::string factorizeExpression(int n);

int main() {
    assert(factorizeExpression(1) == "1");
    assert(factorizeExpression(2) == "2");
    assert(factorizeExpression(3) == "3");
    assert(factorizeExpression(4) == "2*2");
    assert(factorizeExpression(12) == "2*2*3");
    assert(factorizeExpression(18) == "2*3*3");
    assert(factorizeExpression(100) == "2*2*5*5");
    assert(factorizeExpression(97) == "97");
    assert(factorizeExpression(999983) == "999983"); // large prime < 10^6
    assert(factorizeExpression(0) == "0");
    return 0;
}
