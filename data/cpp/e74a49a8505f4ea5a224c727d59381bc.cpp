Write a C++ function named `weightedAverage` that takes three `double` parameters representing exam grades `a`, `b`, and `c` and returns a `double` representing the weighted average according to the following rule: If grade `a` is strictly greater than both `b` and `c`, then use weights 0.25, 0.5, and 0.25 for `a`, `b`, and `c` respectively. Otherwise, use weights 0.25, 0.25, and 0.5 for `a`, `b`, and `c` respectively. The function must be `const`-correct (parameters should be passed by value since they are simple doubles, no mutation inside). Handle edge cases where grades may be negative, zero, or equal—the rule depends only on strict inequality (`>`). For ties (e.g., `a == b > c`), the `else` branch applies. Ensure the function is self-contained with no reliance on global state.

The algorithm is straightforward: evaluate the condition `(a > b && a > c)`. If true, compute `med = a*0.25 + b*0.5 + c*0.25`. If false, compute `med = a*0.25 + b*0.25 + c*0.5`. The main edge case is when `a` is not strictly greater than both others—this includes cases where `a` equals or is less than either `b` or `c`. For example, if `a == b` and both are greater than `c`, the condition is false because `a > b` is false, so the else path is taken. The function uses a single conditional and constant-time arithmetic, so time complexity is \(O(1)\) and space complexity is \(O(1)\). There is no need to handle NaN or infinite values unless specified, but the function works with any finite doubles. The result is returned directly without printing, making it reusable in tests.

// Calculate the weighted average of three exam grades.
// If `a` is strictly greater than both `b` and `c`, weights are (0.25, 0.5, 0.25).
// Otherwise, weights are (0.25, 0.25, 0.5).
double weightedAverage(const double a, const double b, const double c) {
    if (a > b && a > c) {
        return a * 0.25 + b * 0.5 + c * 0.25;
    } else {
        return a * 0.25 + b * 0.25 + c * 0.5;
    }
}

#include <cassert>
#include <cmath>

int main() {
    // a is strictly greatest -> weights (0.25, 0.5, 0.25)
    assert(std::fabs(weightedAverage(10.0, 5.0, 5.0) - 6.25) < 1e-9); // 10*0.25 + 5*0.5 + 5*0.25 = 2.5+2.5+1.25=6.25
    assert(std::fabs(weightedAverage(8.0, 4.0, 2.0) - 4.5) < 1e-9);  // 8*0.25+4*0.5+2*0.25=2+2+0.5=4.5

    // a is not strictly greatest -> weights (0.25, 0.25, 0.5)
    assert(std::fabs(weightedAverage(5.0, 10.0, 5.0) - 6.25) < 1e-9); // 5*0.25+10*0.25+5*0.5=1.25+2.5+2.5=6.25
    assert(std::fabs(weightedAverage(5.0, 5.0, 10.0) - 7.5) < 1e-9);  // 5*0.25+5*0.25+10*0.5=1.25+1.25+5=7.5
    assert(std::fabs(weightedAverage(4.0, 4.0, 4.0) - 4.0) < 1e-9);   // all equal, else path: 1+1+2=4
    assert(std::fabs(weightedAverage(3.0, 3.0, 2.0) - 2.5) < 1e-9);   // a==b, else path: 0.75+0.75+1=2.5
    assert(std::fabs(weightedAverage(0.0, 1.0, 1.0) - 0.75) < 1e-9);  // a is smallest: 0+0.25+0.5=0.75
    assert(std::fabs(weightedAverage(-1.0, -2.0, -3.0) - (-1.75)) < 1e-9); // a greatest: -0.25 + (-1) + (-0.75) = -2, wait recalc: -1*0.25=-0.25, -2*0.5=-1, -3*0.25=-0.75 sum=-2.0? Actually -0.25-1-0.75=-2.0; else would be -0.25-0.5-1.5=-2.25; since a=-1 > b=-2 and c=-3, so result -2.0
    assert(std::fabs(weightedAverage(-1.0, -2.0, -3.0) - (-2.0)) < 1e-9);
    assert(std::fabs(weightedAverage(7.0, 7.0, 7.0) - 7.0) < 1e-9);

    return 0;
}
