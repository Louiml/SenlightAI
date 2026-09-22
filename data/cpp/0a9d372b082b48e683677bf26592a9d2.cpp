Write a C++ function that takes two integer parameters and returns their product as an integer. The function must be named `multiplyIntegers` and must be declared with `const` correctness where appropriate. You do not need to handle overflow or special cases such as zero — just return the mathematical product of the two integers. The function should be self-contained, include all necessary headers, and be usable in a larger program without any global state.

The solution is straightforward: compute `num1 * num2` and return it. Since multiplication of two `int` values is a built-in operation in C++, no special algorithm is required. Edge cases to consider: if either operand is zero, the product is zero; if both are negative, the product is positive; if one is negative and the other positive, the product is negative — all handled naturally by integer arithmetic. There is no need for additional includes beyond `<cstdint>` or basic headers, but for clarity we include `<iostream>` and `<cassert>` for the test. The time complexity is O(1) constant time, and space complexity is O(1) auxiliary space. The function parameters are passed by value, so no `const` reference is needed; however, the parameters can be marked `const` inside the function body to avoid accidental modification, and the return type is `int`. The function should be defined exactly as `int multiplyIntegers(const int num1, const int num2)`.

// Returns the product of two integers.
int multiplyIntegers(const int num1, const int num2) {
    return num1 * num2;
}

#include <cassert>

int main() {
    assert(multiplyIntegers(3, 5) == 15);
    assert(multiplyIntegers(-4, 6) == -24);
    assert(multiplyIntegers(-7, -8) == 56);
    assert(multiplyIntegers(0, 10) == 0);
    assert(multiplyIntegers(1, 1) == 1);
    assert(multiplyIntegers(100, 0) == 0);
    assert(multiplyIntegers(-1, 1) == -1);
    return 0;
}
