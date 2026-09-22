/*
Write a C++ function named `computeFullMultiplication` that takes three integer inputs (`a`, `b`, and `c`) and returns their product as an integer. The function must handle negative numbers, zero, and large positive/negative values without overflow for the given test cases. The task is to design a clean, reusable function (not a program with console I/O) that directly computes and returns the result, suitable for unit testing. The function should be `const`-correct and avoid any side effects.
*/
// Compute the product of three integers.
int computeFullMultiplication(int a, int b, int c) {
    return a * b * c;
}
#include <assert.h>

int computeFullMultiplication(int a, int b, int c);

int main() {
    // Basic positive product
    assert(computeFullMultiplication(2, 3, 4) == 24);
    // Negative times negative times positive
    assert(computeFullMultiplication(-2, -3, 4) == 24);
    // One negative
    assert(computeFullMultiplication(-2, 3, 4) == -24);
    // Zero always yields zero
    assert(computeFullMultiplication(0, 100, -5) == 0);
    // Three negatives (odd count → negative result)
    assert(computeFullMultiplication(-2, -3, -4) == -24);
    // Large values within range
    assert(computeFullMultiplication(100, 100, 100) == 1000000);
    // One and negative one
    assert(computeFullMultiplication(1, -1, 1) == -1);
    // Mixed large and small
    assert(computeFullMultiplication(-10, 10, -10) == 1000);
}
// The solution is straightforward: multiply the three integers sequentially and return the result. The main algorithm is `a * b * c`. Edge cases include:  
// - If any operand is zero, the product is zero (handled naturally).  
// - If all operands are positive, the product is positive.  
// - If an even number of operands are negative, the product is positive; if an odd number, the product is negative—multiplication handles this automatically.  
// - Overflow is not a concern for the provided test cases (values within ±1000), but in general, we could use `long long` for safety; however, the task specifies integer return, so we stay with `int`.  
// Time complexity is O(1) and space complexity is O(1).
