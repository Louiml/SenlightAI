Write a C++ function `double LnFac(int32_t n)` that computes the natural logarithm of the factorial of a non-negative integer `n`. The function must handle very large `n` (up to at least 2,000,000) without overflow by using a precomputed lookup table for small values and Stirling's approximation for large values. For `n` less than 50, return exact values from a static, lazily initialized table computed on first call. For `n` ≥ 50, use the Stirling series with terms up to \(1/r^3\), where \(r = 1/n\), i.e., \(\ln(n!) \approx (n+0.5)\ln(n) - n + \ln(\sqrt{2\pi}) + \frac{1}{12n} - \frac{1}{360n^3}\). If the input is negative, throw a `std::invalid_argument` exception. The function must be thread-safe on first call (use a mutex or C++11 thread-safe static initialization) and must be `const`-correct, though as a free function it simply takes a parameter. The implementation should be self-contained, not relying on any external libraries beyond the standard C++ library.

#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>

// Declare the solution function (assumed to be defined elsewhere or above)
double LnFac(int32_t n);

int main() {
    // Known exact values for small n
    assert(std::abs(LnFac(0) - 0.0) < 1e-12);
    assert(std::abs(LnFac(1) - 0.0) < 1e-12);
    assert(std::abs(LnFac(2) - std::log(2.0)) < 1e-12);
    assert(std::abs(LnFac(3) - std::log(6.0)) < 1e-12);
    assert(std::abs(LnFac(10) - std::log(3628800.0)) < 1e-12);
    
    // Consistency across the boundary (n=49 and n=50)
    double exact_49 = 0.0;
    for (int i = 1; i <= 49; ++i) exact_49 += std::log(static_cast<double>(i));
    assert(std::abs(LnFac(49) - exact_49) < 1e-12);
    
    // Verify Stirling's approximation for large n against a known formula
    // ln(2000000!) computed via Stirling with more terms for reference
    int32_t n = 2000000;
    double approx = LnFac(n);
    // Reference value using series with C5 = 1/1260 and C7 = -1/1680
    double n1 = static_cast<double>(n);
    double r = 1.0 / n1;
    double reference = (n1 + 0.5) * std::log(n1) - n1 + 0.9189385332046727418
                     + r * (1.0/12.0) - r*r*r * (1.0/360.0) + r*r*r*r*r * (1.0/1260.0);
    // Difference should be at most ~1e-9 relative
    assert(std::abs(approx - reference) / reference < 1e-9);
    
    // Verify that LnFac is monotonic and increasing for large values
    assert(LnFac(100) < LnFac(101));
    assert(LnFac(1000) < LnFac(1001));
    
    // Verify negative input throws exception
    bool threw = false;
    try {
        LnFac(-1);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
    
    std::cout << "All tests passed." << std::endl;
    return 0;
}

#include <cmath>
#include <cstdint>
#include <stdexcept>

// Compute ln(n!) for non-negative integer n using exact table for n < 50
// and Stirling's approximation for n >= 50. Throws std::invalid_argument for n < 0.
double LnFac(int32_t n) {
    if (n < 0) {
        throw std::invalid_argument("LnFac: n must be non-negative");
    }
    
    // Constants for Stirling's approximation
    static const double C0 = 0.9189385332046727418;   // ln(sqrt(2*pi))
    static const double C1 = 1.0 / 12.0;
    static const double C3 = -1.0 / 360.0;
    
    // Lookup table for small n, initialized lazily and thread-safe via magic statics.
    static const int FAK_LEN = 50;
    static double fac_table[FAK_LEN] = {0.0};
    static const bool initialized = []() {
        fac_table[0] = 0.0;
        for (int i = 1; i < FAK_LEN; ++i) {
            fac_table[i] = fac_table[i - 1] + std::log(static_cast<double>(i));
        }
        return true;
    }();
    (void)initialized;  // suppress unused variable warning
    
    if (n < FAK_LEN) {
        return fac_table[n];
    }
    
    // Stirling's approximation for n >= 50
    double n1 = static_cast<double>(n);
    double r = 1.0 / n1;
    return (n1 + 0.5) * std::log(n1) - n1 + C0 + r * (C1 + r * r * C3);
}

// The solution uses a two-tier approach. For small `n`, a static array `fact_table` stores exact values of \(\ln(n!)\) computed iteratively using the recurrence \(\ln(n!) = \ln((n-1)!) + \ln(n)\). This table is lazily initialized on the first call when `n < 50`, using a local static variable that C++11 guarantees to be thread-safe (magic statics). For `n ≥ 50`, Stirling's approximation is used: \(\ln(n!) \approx (n + 0.5)\ln(n) - n + C_0 + r(C_1 + r^2 C_3)\), where \(C_0 = \ln(\sqrt{2\pi}) \approx 0.9189385332046727\), \(C_1 = 1/12\), \(C_3 = -1/360\), and \(r = 1/n\). This approximation is accurate to about 14 decimal digits for `n ≥ 50` and becomes more accurate as `n` grows. The negative input case immediately throws `std::invalid_argument`. Edge cases: `n = 0` and `n = 1` return 0 (since \(0! = 1! = 1\), whose natural log is 0). The lookup table requires `O(FAK_LEN)` space (here 50 entries) and `O(1)` time per call after initialization; the first call takes `O(FAK_LEN)` to build the table. For large `n`, time complexity is `O(1)` and space `O(1)`. The implementation avoids overflow by using `double` arithmetic directly.
