Write a C++ function that takes three integers `a`, `b`, and `c` as parameters and returns the value of the mathematical expression: `a * a + 4 * (b * c + 2 * a)`. The function must be named `computeExpression`, use `const` for parameters where appropriate, and return the result as an `int`. The function should work for any integer inputs, including negative numbers and zero, and must not read from or write to any standard input/output — it should only compute and return the result.

// The solution is straightforward: compute each part of the expression step‑by‑step or directly in a single return statement. The expression is `a² + 4·(b·c + 2·a)`. Start by calculating `a*a` (which may be large, but `int` overflow is not a concern for typical test values). Then compute `b*c`, add `2*a` to that product, multiply by 4, and finally add `a*a`. Order of operations follows standard arithmetic rules: multiplication before addition, parentheses first. Edge cases include negative inputs — the product `b*c` can be negative, and `2*a` can be negative, but their sum multiplied by 4 is handled correctly by integer arithmetic (truncation is not an issue since there is no division). The time complexity is O(1) as it performs a constant number of arithmetic operations. The space complexity is O(1) since only a few local variables are used.

#include <cstdint> // for int32_t if needed, but plain int is fine

// Computes the expression: a*a + 4 * (b*c + 2*a)
// Returns the integer result.
int computeExpression(const int a, const int b, const int c) {
    const int a_squared = a * a;           // a^2
    const int inside = b * c + 2 * a;      // b*c + 2*a
    const int result = a_squared + 4 * inside; // a^2 + 4*inside
    return result;
}

#include <cassert>

int main() {
    // Test 1: Basic positive numbers
    assert(computeExpression(1, 2, 3) == 1 + 4 * (6 + 2)); // 1 + 4*8 = 33
    // Test 2: All zeros
    assert(computeExpression(0, 0, 0) == 0);
    // Test 3: Negative values
    assert(computeExpression(-2, -3, 4) == 4 + 4 * (-12 + -4)); // 4 + 4*(-16) = -60
    // Test 4: Mixed signs
    assert(computeExpression(3, -1, 5) == 9 + 4 * (-5 + 6)); // 9 + 4*1 = 13
    // Test 5: Larger numbers
    assert(computeExpression(10, 5, 2) == 100 + 4 * (10 + 20)); // 100 + 120 = 220
    // Test 6: b*c negative, 2*a positive
    assert(computeExpression(1, -2, 3) == 1 + 4 * (-6 + 2)); // 1 + 4*(-4) = -15
    // Test 7: b*c positive, 2*a negative
    assert(computeExpression(-1, 2, 3) == 1 + 4 * (6 + -2)); // 1 + 4*4 = 17
    // Test 8: Zero b
    assert(computeExpression(2, 0, 7) == 4 + 4 * (0 + 4)); // 4 + 16 = 20
    // Test 9: Zero c
    assert(computeExpression(-3, 4, 0) == 9 + 4 * (0 + -6)); // 9 + (-24) = -15
    // Test 10: Extremes (within int range)
    assert(computeExpression(100, -100, 100) == 10000 + 4 * (-10000 + 200)); // 10000 + 4*(-9800) = 10000 - 39200 = -29200
    return 0;
}
