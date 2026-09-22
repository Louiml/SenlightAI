Write a C++ function `std::string reduceFractions(int n, const std::vector<std::pair<long long, long long>>& fractions)` that takes a list of fractions (numerator, denominator) and returns a string representing their sum in reduced form. The sum must be output as: an integer alone if the result is a whole number; otherwise, if the absolute value of the sum is greater than or equal to 1, output the integer part followed by a space and then the proper fraction part (e.g., `2 1/3`); otherwise output just the proper fraction (e.g., `-1/2`). For a zero sum, output `"0"`. You may assume all denominators are nonzero, but numerators and denominators can be negative. The reduction must use the greatest common divisor (gcd) for simplification, but careful: the effective denominator for the sum should be the least common multiple (LCM) of all denominators, not the product. However, to simplify the implementation, you may first compute the sum using a common denominator that is the LCM (which can be obtained via gcd), then reduce the final fraction. A key edge case is a zero numerator: the result must be exactly `"0"` without a denominator. Another edge case is a negative sum with a fraction part; for example, `-7/2` should be output as `-3 1/2` (the integer part is floor division in C++ termination for positive denominators, but for negatives, ensure the fraction part is always positive). Implement the function to handle up to 100 fractions each with numerator and denominator up to 10^9, and the final numerator and denominator may exceed 64-bit if not reduced, so reduce intermediate steps is not necessary; only the final gcd reduction is needed, and the LCM approach avoids overflow by using gcd before multiplication.

#include <cassert>
#include <vector>
#include <string>

// Function prototype (since solution is separate)
std::string reduceFractions(int n, const std::vector<std::pair<long long, long long>>& fractions);

int main() {
    // Basic positive sum
    assert(reduceFractions(2, {{1,2},{1,3}}) == "5/6");
    // Whole number sum
    assert(reduceFractions(2, {{1,2},{1,2}}) == "1");
    // Negative proper fraction
    assert(reduceFractions(1, {{-1,2}}) == "-1/2");
    // Negative improper fraction with integer part
    assert(reduceFractions(1, {{-7,2}}) == "-3 1/2");
    // Zero sum
    assert(reduceFractions(2, {{1,2},{-1,2}}) == "0");
    // Mixed signs and reduction
    assert(reduceFractions(3, {{1,4},{-1,6},{1,3}}) == "5/12");
    // Large denominators, reduction to integer
    assert(reduceFractions(2, {{100,200},{100,200}}) == "1");
    // Negative numerator and denominator cancellation
    assert(reduceFractions(1, {{-3,-4}}) == "3/4");
    // More than one integer part
    assert(reduceFractions(2, {{5,2},{5,2}}) == "5");
    // Negative integer part with fraction
    assert(reduceFractions(1, {{-9,4}}) == "-2 1/4");
    return 0;
}

#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>

using Fraction = std::pair<long long, long long>;

// Compute gcd of two non-negative numbers (using __int128 for safety)
long long gcdLL(long long a, long long b) {
    a = std::llabs(a);
    b = std::llabs(b);
    while (b != 0) {
        long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

// Ensure denominator positive, denominator not zero
std::string reduceFractions(int n, const std::vector<Fraction>& fractions) {
    if (n == 0) return "0";

    // Compute LCM of denominators using __int128 to avoid overflow
    __int128 lcm = 1;
    for (int i = 0; i < n; ++i) {
        long long den = fractions[i].second;
        if (den < 0) den = -den; // use absolute denominator for LCM
        __int128 g = gcdLL((long long)lcm, den);
        lcm = lcm / g * static_cast<__int128>(den);
    }

    // Sum numerators scaled to lcm
    __int128 totalNum = 0;
    for (int i = 0; i < n; ++i) {
        long long num = fractions[i].first;
        long long den = fractions[i].second;
        if (den < 0) { num = -num; den = -den; } // move sign to numerator
        totalNum += (lcm / den) * static_cast<__int128>(num);
    }

    // Handle zero
    if (totalNum == 0) return "0";

    // Reduce fraction
    __int128 g = gcdLL((long long)totalNum, (long long)lcm);
    totalNum /= g;
    lcm /= g;

    long long num = (long long)totalNum;
    long long den = (long long)lcm;

    // If denominator is 1, output integer
    if (den == 1) return std::to_string(num);

    // Determines output format
    bool negative = num < 0;
    long long absNum = std::llabs(num);
    if (absNum < den) {
        // Proper fraction, e.g., -1/2
        return std::to_string(num) + "/" + std::to_string(den);
    }

    // Improper fraction, output integer and proper fraction part
    // Use floor division so remainder is always positive
    long long integerPart;
    long long remainder;
    if (num >= 0) {
        integerPart = num / den;
        remainder = num % den;
    } else {
        integerPart = num / den;
        remainder = num % den;
        if (remainder < 0) {
            integerPart--;
            remainder += den;
        }
    }
    std::string result = std::to_string(integerPart);
    if (remainder != 0) {
        result += " " + std::to_string(remainder) + "/" + std::to_string(den);
    }
    return result;
}

// The core algorithm is: (1) compute the LCM of all denominators. The LCM of two numbers a and b is a / gcd(a,b) * b. To compute LCM for a list, iterate while maintaining the running LCM, using 64-bit unsigned arithmetic carefully to avoid overflow (note: the product of two denominators up to 1e9 could be 1e18 which still fits in unsigned long long, but when multiplied again, overflow may occur; however, since we compute LCM incrementally and the problem constraints have n up to 100 and denominators up to 1e9, the LCM could be enormous; but the snippet uses a trick of removing any common factors from the product using prime statuses, but we will instead compute LCM via gcd accurately, but if LCM exceeds unsigned long long, we can instead compute using __int128 for safety in the reference solution). Actually, the reference solution can use `__int128` to avoid overflow for intermediate product, but the output must be handled. For robustness, we can compute the LCM using `std::lcm` in C++17, but since the task is independent, we implement our own. After obtaining LCM, compute the sum of numerators scaled to that denominator: for each fraction, add `(LCM / denominator) * numerator`. The sum may be negative. Let `sumNum` be the total numerator, and `LCM` the denominator. Then reduce the fraction by gcd of `abs(sumNum)` and `LCM`. After reduction, if the reduced numerator is zero, return `"0"`. Let `num = reduced numerator`, `den = reduced denominator`. If `abs(num) < den`, then output just `num/den` (e.g., `-1/2`). If `abs(num) >= den`, compute integer part = `num / den` (C++ integer division truncates toward zero for negative numbers, which is not what we want; we need floor division so the fraction part is positive). For positive num, integer part = num/den, remainder = num%den. For negative num, to ensure remainder positive, we can do: `long long integerPart = num / den; long long remainder = num % den; if (remainder < 0) { integerPart--; remainder += den; }` Then output `integerPart` and if remainder > 0, append a space and `remainder/den`. Edge cases: den could be 1 after reduction, then just output integer part. Also ensure that after reduction, if remainder is zero, output just integer part. Complexity: O(n log M) where M is the magnitude of denominators for gcd/lcm operations, and O(1) extra space.
