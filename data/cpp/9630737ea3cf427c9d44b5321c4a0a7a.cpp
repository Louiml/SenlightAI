/*
Write a C++ function that takes three integer parameters `a`, `b`, and `c` and returns the value of `(a - b) * (b - c) * (c - a)` as an integer. The function must handle negative inputs and zero correctly, and the result may be negative, zero, or positive depending on the relationship between the inputs. The function should be named `cyclicProductDifference` and must be `const`-correct (i.e., parameters passed by value, no modification of any external state). The function should simply compute and return the product; no input/output is performed inside the function.
*/

#include <cstddef> // for std::ptrdiff_t? Not needed, but keep for completeness

// Computes (a - b) * (b - c) * (c - a).
// Returns the product as a long long to avoid overflow for moderate inputs.
// The function is const-correct: parameters are passed by value, no side effects.
long long cyclicProductDifference(int a, int b, int c) {
    // Cast to long long before subtraction to avoid int overflow in intermediate steps.
    long long first = static_cast<long long>(a) - b;
    long long second = static_cast<long long>(b) - c;
    long long third = static_cast<long long>(c) - a;
    return first * second * third;
}

#include <cassert>

int main() {
    // Case 1: distinct positive numbers
    assert(cyclicProductDifference(1, 2, 3) == (1-2)*(2-3)*(3-1));
    assert(cyclicProductDifference(1, 2, 3) == -2);

    // Case 2: distinct negative numbers
    assert(cyclicProductDifference(-1, -2, -3) == (-1+2)*(-2+3)*(-3+1));
    assert(cyclicProductDifference(-1, -2, -3) == 1 * 1 * (-2) == -2);

    // Case 3: zeros
    assert(cyclicProductDifference(0, 1, 2) == (0-1)*(1-2)*(2-0) == -1 * -1 * 2 == 2);
    assert(cyclicProductDifference(5, 5, 3) == (5-5)*(5-3)*(3-5) == 0 * 2 * -2 == 0);

    // Case 4: all equal
    assert(cyclicProductDifference(7, 7, 7) == 0);

    // Case 5: two equal, one different
    assert(cyclicProductDifference(4, 2, 4) == (4-2)*(2-4)*(4-4) == 2 * -2 * 0 == 0);

    // Case 6: mixture of signs, order matters
    assert(cyclicProductDifference(-3, 0, 3) == (-3-0)*(0-3)*(3-(-3)) == -3 * -3 * 6 == 54);

    // Case 7: large inputs that might overflow int but not long long
    // Use values within int range but product larger than INT_MAX
    assert(cyclicProductDifference(100000, -100000, 1) == (100000+100000)*(-100000-1)*(1-100000));
    // Compute manually: 200000 * (-100001) * (-99999) = 200000 * 100001 * 99999 ≈ 2e15, fits long long.
    assert(cyclicProductDifference(100000, -100000, 1) == 200000LL * (-100001LL) * (-99999LL));

    // Case 8: zero result from a factor being zero
    assert(cyclicProductDifference(0, 0, 5) == 0);

    // Case 9: negative product
    assert(cyclicProductDifference(1, 3, 2) == (1-3)*(3-2)*(2-1) == -2 * 1 * 1 == -2);

    // Case 10: all distinct and cyclic order reversed
    assert(cyclicProductDifference(5, 1, 3) == (5-1)*(1-3)*(3-5) == 4 * -2 * -2 == 16);
}

// The core algorithm is direct arithmetic: evaluate the expression `(a - b) * (b - c) * (c - a)` using standard integer multiplication. No special edge cases beyond integer overflow: since the inputs are arbitrary integers, the intermediate differences and the final product may overflow the `int` range. To be safe, the function should use `long long` internally for intermediate calculations to reduce overflow risk, and return a `long long` — but to keep the task simple and match typical integer use, we can assume inputs are small enough (e.g., within `[-1000, 1000]`) so `int` is sufficient. However, for robust design, we compute using `long long` and cast the result back to `int` if desired. The tricky part is that if `a = b` or `b = c` or `c = a`, then one factor is zero, so the product is zero. Negative inputs work naturally. There is no need for sorting or comparisons. Time complexity is O(1), space complexity is O(1). The only edge case is overflow, which we handle by using `long long` for internal multiplication. If we stick to `int` return, we can document that inputs are assumed within safe range.
