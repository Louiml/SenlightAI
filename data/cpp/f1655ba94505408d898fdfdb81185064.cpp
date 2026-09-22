/*
Write a C++ function that accepts a non-empty string of digits (only characters `'0'` through `'9'`, no spaces or other characters) and returns a string representation of a certain rational number derived from the digits, formatted as either an integer (if the fraction is zero) or as `integer numerator/denominator` (if the numerator is nonzero, reduced to lowest terms). Specifically, given the digit string `d_0 d_1 ... d_{n-1}` (where `d_0` is the most significant digit, not necessarily nonzero, and `n >= 2`), compute the weighted sum: `interm = sum_{i=0}^{n-1} digit_i * (1 << i) * (3^(n-1-i))`. Then compute `A = interm mod (1 << (n-1))` and `B = 1 << (n-1)`. The result is the fraction `interm / B` expressed as a mixed number: if `A == 0`, return just the integer part `interm / B` as a decimal string; otherwise, return the integer part followed by a space, then the reduced fraction `A/gcd(A,B) / B/gcd(A,B)`. Use 64-bit signed integers for all intermediate arithmetic; the input length is at most 30, so all values fit within `int64_t`. The function should be named `binaryTernaryFraction` and take a `const std::string&` parameter.
*/
#include <string>
#include <cstdint>
#include <algorithm>

// Fast integer exponentiation for positive base and non-negative exponent.
static int64_t ipow(int64_t base, int exp) {
    int64_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

// Euclidean algorithm for greatest common divisor.
static int64_t gcd(int64_t a, int64_t b) {
    while (b != 0) {
        int64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Given a string of digits (at least 2 characters), compute the described
// weighted sum and return the mixed-number representation of the fraction
// (interm / 2^(n-1)), reduced to lowest terms.
std::string binaryTernaryFraction(const std::string& digits) {
    const int n = static_cast<int>(digits.size());

    // Compute the weighted sum per the specification.
    int64_t interm = 0;
    for (int i = 0; i < n; ++i) {
        int64_t digit = digits[i] - '0';
        int64_t power_of_two = static_cast<int64_t>(1) << i;
        int64_t power_of_three = ipow(3, n - 1 - i);
        interm += digit * power_of_two * power_of_three;
    }

    const int64_t denominator = static_cast<int64_t>(1) << (n - 1);
    const int64_t integer_part = interm / denominator;
    const int64_t remainder = interm % denominator;

    // If no fractional part, return just the integer as a string.
    if (remainder == 0) {
        return std::to_string(integer_part);
    }

    // Reduce the fraction by the GCD.
    const int64_t divisor = gcd(remainder, denominator);
    const int64_t numerator = remainder / divisor;
    const int64_t reduced_denominator = denominator / divisor;

    return std::to_string(integer_part) + " " +
           std::to_string(numerator) + "/" + std::to_string(reduced_denominator);
}
#include <cassert>
#include <string>

// The solution function is declared above; this test harness verifies correctness.
int main() {
    // Example from the original snippet logic: "10" -> n=2, interm = 1*1*3 + 0*2*1 = 3, denominator=2 => 1 1/2
    assert(binaryTernaryFraction("10") == "1 1/2");
    // "01" -> interm = 0*1*3 + 1*2*1 = 2, denominator=2 => 1 (integer)
    assert(binaryTernaryFraction("01") == "1");
    // "11" -> interm = 1*1*3 + 1*2*1 = 5, denominator=2 => 2 1/2
    assert(binaryTernaryFraction("11") == "2 1/2");
    // "000" -> n=3, interm=0, denominator=4 => 0
    assert(binaryTernaryFraction("000") == "0");
    // "100" -> interm = 1*1*9 + 0*2*3 + 0*4*1 = 9, denominator=4 => 2 1/4
    assert(binaryTernaryFraction("100") == "2 1/4");
    // "222" -> interm = 2*1*9 + 2*2*3 + 2*4*1 = 18+12+8=38, denominator=4 => 9 1/2
    assert(binaryTernaryFraction("222") == "9 1/2");
    // "99" -> interm = 9*1*3 + 9*2*1 = 27+18=45, denominator=2 => 22 1/2
    assert(binaryTernaryFraction("99") == "22 1/2");
    // Large input: "123456789012345678901234567890" (30 digits) — verify no crash and sensible output length
    std::string large = "123456789012345678901234567890";
    std::string large_result = binaryTernaryFraction(large);
    // For sanity, ensure result contains a space or is just a number; we just check it's non-empty
    assert(!large_result.empty());
    return 0;
}
// The problem decomposes into three parts: parsing digits, computing the weighted sum, and forming a reduced mixed number. First, iterate over the input string, converting each character `c` to a digit by `c - '0'`. For the weighted sum, use the given formula: for index `i` (0-based from leftmost digit), multiply the digit by `2^i` (using `1LL << i`) and by `3^(n-1-i)` (computed via fast exponentiation `ipow`). Accumulate into an `int64_t`. After the sum, compute the integer part as `interm >> (n-1)` (since `B = 2^(n-1)`), and the fractional remainder as `A = interm & ((1LL << (n-1)) - 1)` (or via modulo). If `A` is zero, return the integer part as a string. Otherwise, compute `g = gcd(A, B)` (using the Euclidean algorithm), reduce both `A` and `B` by `g`, and concatenate the integer part, a space, `A/g`, a slash, and `B/g`. Edge cases: the input length is at least 2, so `n-1 >= 1` and `B > 0`; leading zeros are allowed (e.g., "01" gives `interm = 0*3 + 1*2*1?` careful: for "01", n=2, i=0 digit 0 => 0*1*3=0, i=1 digit 1 => 1*2*1=2, so interm=2, B=2 => integer=1, A=0 => result "1"). The fast exponentiation `ipow` handles exponent up to 29 without overflow (3^29 ≈ 6.8e13, times up to 2^30 ≈ 1e9 gives ~6.8e22, but the sum is bounded by something smaller; still, using 64-bit is safe because max value of `interm` is at most digit sum * max term, and max term at i=0 is 9*3^(n-1) with n=30 gives ~9*6.8e13 ≈ 6.1e14, well within int64). Time complexity is \(O(n^2)\) due to the loop and exponentiation per term (or \(O(n \log n)\) if exponentiation is O(log n)), and space is O(n) for the digit vector (or O(1) if we avoid storing digits and process directly, but the snippet stores them; we can process directly for O(1) extra space). The overall time is linear in n with a constant factor for exponentiation, and space O(1) if we avoid the vector.
