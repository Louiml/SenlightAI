// Write a C++ function named `factorialDigitSum` that takes an integer `n` (where `n >= 1`) and returns the sum of the decimal digits of `n!` (the factorial of `n`). The factorial can be extremely large (e.g., for `n = 1000`, `1000!` has 2568 digits), so you must use arbitrary-precision arithmetic. Use `boost::multiprecision::cpp_int` to compute the factorial. You must handle edge cases like `n = 1` (where `1! = 1` and digit sum is 1) and large `n` up to at least `10000`. The function should be `const`-correct and not modify its input.

The main challenge is computing `n!` for large `n` without overflow. Standard integer types overflow quickly (e.g., `20!` exceeds 64-bit). Using `boost::multiprecision::cpp_int` allows arbitrary precision. The algorithm: initialize a `cpp_int` result to 1, then multiply it by every integer from 2 to `n` in a loop. After computing the factorial, convert the `cpp_int` to a string (via `result.convert_to<std::string>()` or using `ostringstream`), then iterate over each character of the string, converting each digit to an integer and summing them. Edge cases: for `n == 1`, the loop never runs (since we start from 2), and the factorial remains 1, so digit sum is 1. For `n == 0` (though not required by spec, we can handle it by returning 1 for `0! = 1`). Time complexity: O(n * M) where M is the number of digits of `n!` (since multiplication by a small integer is proportional to the number of digits). This is approximately O(n * n log n) for very large n, but for practical `n` up to 10000 it is fast. Space complexity: O(M) for storing the factorial and the string, where M is the number of digits of `n!` (which is roughly O(n log n)).

#include <boost/multiprecision/cpp_int.hpp>
#include <string>

using boost::multiprecision::cpp_int;

// Compute the sum of decimal digits of n! using arbitrary-precision arithmetic.
int factorialDigitSum(int n) {
    if (n < 0) return 0; // Invalid input, though spec says n >= 1
    cpp_int factorial = 1;
    for (int i = 2; i <= n; ++i) {
        factorial *= i;
    }
    // Convert factorial to string to extract digits
    std::string digits = factorial.convert_to<std::string>();
    int sum = 0;
    for (char c : digits) {
        sum += c - '0';
    }
    return sum;
}

#include <cassert>

int main() {
    assert(factorialDigitSum(1) == 1);       // 1! = 1
    assert(factorialDigitSum(2) == 2);       // 2! = 2
    assert(factorialDigitSum(3) == 6);       // 3! = 6
    assert(factorialDigitSum(4) == 6);       // 4! = 24, digits sum = 2+4=6
    assert(factorialDigitSum(5) == 3);       // 5! = 120, sum = 1+2+0=3
    assert(factorialDigitSum(10) == 27);     // 10! = 3628800, sum = 3+6+2+8+8+0+0=27
    assert(factorialDigitSum(100) == 648);   // known value: sum of digits of 100! is 648
    // Larger test: 1000! digit sum is 10539 (known from Project Euler)
    assert(factorialDigitSum(1000) == 10539);
    return 0;
}
