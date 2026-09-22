Given three integers \(n\), \(m\), and a number of test cases \(T\), write a C++ function that computes two probabilities:  
- \(p_1\): the probability that a random real number \(x\) uniformly chosen from \([0,1]\) will be **strictly less than** \(0.5\) after being scaled by a factor of \(n\) occurrences? Actually, the original snippet computes two values:  
  \(a = 0.5\) normally, \(a = 1\) if \(n == 1\).  
  \(b = \frac{m+1}{2m}\), \(b = 1\) if \(m == 1\).  
Your task: implement a function `pair<double,double> computeProbabilities(int n, int m)` that returns these two values with the same logic as the snippet:  
- If \(n == 1\), \(a = 1.0\), else \(a = 0.5\).  
- If \(m == 1\), \(b = 1.0\), else \(b = (m + 1.0) / (2.0 * m)\).  
Return the pair exactly as doubles. The function must handle all positive integers \(n\) and \(m\), including very large values (up to \(10^9\)).

#include <cassert>
#include <cmath>
#include <utility>

// Assuming the solution function is in the same translation unit.
std::pair<double, double> computeProbabilities(int n, int m);

int main() {
    // Test n == 1, m == 1
    auto p = computeProbabilities(1, 1);
    assert(std::fabs(p.first - 1.0) < 1e-9);
    assert(std::fabs(p.second - 1.0) < 1e-9);

    // Test n == 1, m == 2
    p = computeProbabilities(1, 2);
    assert(std::fabs(p.first - 1.0) < 1e-9);
    assert(std::fabs(p.second - 0.75) < 1e-9);

    // Test n == 2, m == 1
    p = computeProbabilities(2, 1);
    assert(std::fabs(p.first - 0.5) < 1e-9);
    assert(std::fabs(p.second - 1.0) < 1e-9);

    // Test n == 2, m == 2
    p = computeProbabilities(2, 2);
    assert(std::fabs(p.first - 0.5) < 1e-9);
    assert(std::fabs(p.second - 0.75) < 1e-9);

    // Test n == 3, m == 4
    p = computeProbabilities(3, 4);
    assert(std::fabs(p.first - 0.5) < 1e-9);
    assert(std::fabs(p.second - (5.0/8.0)) < 1e-9);

    // Test large values: m = 1e9, n = 123456789
    p = computeProbabilities(123456789, 1000000000);
    assert(std::fabs(p.first - 0.5) < 1e-9);
    double expected_b = (1000000000.0 + 1.0) / (2.0 * 1000000000.0);
    assert(std::fabs(p.second - expected_b) < 1e-12);

    // Test n = 1, m = 1000000000
    p = computeProbabilities(1, 1000000000);
    assert(std::fabs(p.first - 1.0) < 1e-9);
    assert(std::fabs(p.second - expected_b) < 1e-12);

    return 0;
}

#include <utility>

// Computes two probabilities based on n and m.
// Returns a pair {a, b} where:
//   a = 1.0 if n == 1, else 0.5
//   b = 1.0 if m == 1, else (m+1)/(2m)
std::pair<double, double> computeProbabilities(int n, int m) {
    double a = 0.5;
    if (n == 1) a = 1.0;

    double b = (static_cast<double>(m) + 1.0) / (2.0 * static_cast<double>(m));
    if (m == 1) b = 1.0;

    return {a, b};
}

// The problem is straightforward: given two integer parameters, we compute two independent probabilities.  
// - For \(a\): the default is always \(0.5\), except when \(n == 1\), in which case it becomes \(1.0\). This is a simple conditional.  
// - For \(b\): the formula is \((m+1)/(2m)\) for all \(m > 1\), but when \(m == 1\), the formula gives \((1+1)/(2*1) = 1.0\), which matches the special case anyway; however, the snippet explicitly sets \(b = 1\) when \(m == 1\). This is redundant because the formula already yields 1, but we keep the conditional for consistency.  
// Important edge cases: both \(n\) and \(m\) are positive integers; the minimum value is 1. For large \(m\), the division uses `double` arithmetic to avoid integer overflow (since \(m\) can be up to \(10^9\), \(m+1\) fits in 64-bit but we cast to double first). Time complexity is \(O(1)\) per call, space complexity is \(O(1)\). No floating-point precision issues arise because the results are either exactly 0.5, 1.0, or a small rational with denominator up to \(2\cdot10^9\), which is easily representable in double.
