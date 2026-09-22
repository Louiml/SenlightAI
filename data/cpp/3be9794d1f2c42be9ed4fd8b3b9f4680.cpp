// Write a C++ function named `computeTriangleAreaFormula` that takes three integer side lengths and returns an integer result representing the Heron’s formula product \( s \cdot (s-a) \cdot (s-b) \cdot (s-c) \), where \( s = (a+b+c)/2 \) using integer division (truncation). The function should accept three `int` parameters, use `const` correctness for the parameters, and return the computed product. Note that the function does not need to validate triangle inequality or handle invalid inputs; it simply performs the arithmetic as specified. The function should be standalone (no `main`), and the caller is responsible for providing non-negative integers such that the intermediate calculations do not overflow.
The solution directly implements the given formula. First, compute the semi-perimeter `s` as `(a + b + c) / 2` using integer division (which truncates toward zero). Then return the product `s * (s - a) * (s - b) * (s - c)`. Important edge cases include: when any side is zero or the sum is odd, the truncation in integer division may produce a product that does not match the real-valued area formula – but the task explicitly mirrors the snippet's behavior, so we replicate it exactly. Also, if the input is such that `(s - a)`, `(s - b)`, or `(s - c)` becomes negative, the product could be negative or zero; we do not guard against this. Time complexity is \(O(1)\) constant time, and space complexity is \(O(1)\) as well. The function uses only a few local variables; parameters are passed by value, and we mark them `const` in the signature to emphasize they are not modified.
// Compute s = (a+b+c)/2 (integer division) and return s*(s-a)*(s-b)*(s-c).
int computeTriangleAreaFormula(const int a, const int b, const int c) {
    const int s = (a + b + c) / 2;
    const int term1 = s - a;
    const int term2 = s - b;
    const int term3 = s - c;
    return s * term1 * term2 * term3;
}
#include <cassert>

// Function declaration (prototype) for the solution under test.
int computeTriangleAreaFormula(const int a, const int b, const int c);

int main() {
    // Example from typical usage: sides 3,4,5 -> s=6 -> product=36
    assert(computeTriangleAreaFormula(3, 4, 5) == 36);
    // Equilateral triangle: 6,6,6 -> s=9 -> product=9*3*3*3=243
    assert(computeTriangleAreaFormula(6, 6, 6) == 243);
    // Degenerate: 1,1,2 -> s=2 -> product = 2*1*1*0 = 0
    assert(computeTriangleAreaFormula(1, 1, 2) == 0);
    // Zero side: 0,3,4 -> s=3 -> product = 3*3*0*(-1) = 0
    assert(computeTriangleAreaFormula(0, 3, 4) == 0);
    // Odd sum: 1,1,1 -> s=1 (since 3/2=1) -> product=1*0*0*0=0
    assert(computeTriangleAreaFormula(1, 1, 1) == 0);
    // Large values (no overflow here): 100,100,100 -> s=150 -> product=150*50*50*50=18,750,000
    assert(computeTriangleAreaFormula(100, 100, 100) == 18750000);
    // Negative result possible if s-a negative: 2,2,10 -> s=7 -> product=7*5*5*(-3) = -525
    assert(computeTriangleAreaFormula(2, 2, 10) == -525);
    return 0;
}
