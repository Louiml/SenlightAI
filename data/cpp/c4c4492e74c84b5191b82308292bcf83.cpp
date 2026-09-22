/*
Write a C++ function named `smallestOfTwo` that takes two integer parameters and returns the smaller of the two values using the conditional operator (`?:`). The function must not use any branching statements such as `if`, `else`, `switch`, or loops—only the conditional operator is allowed. The returned value should be the minimum of the two inputs, and the function must be `const`-correct (i.e., parameters are passed by value, so no additional `const` is needed, but the function itself should not modify its parameters). The input integers can be negative, zero, or positive, and no special edge-case handling is needed beyond the straightforward comparison.
*/
// Return the smaller of two integers using the conditional operator.
int smallestOfTwo(int a, int b) {
    return a < b ? a : b;
}
#include <cassert>

// The function declaration is provided here for completeness in the test.
// In a real test file, this would be from the header or included above.
int smallestOfTwo(int a, int b);

int main() {
    // Basic positive cases
    assert(smallestOfTwo(3, 5) == 3);
    assert(smallestOfTwo(10, 2) == 2);
    
    // Equal numbers
    assert(smallestOfTwo(7, 7) == 7);
    assert(smallestOfTwo(-4, -4) == -4);
    
    // Negative numbers
    assert(smallestOfTwo(-1, -5) == -5);
    assert(smallestOfTwo(-8, 3) == -8);
    
    // Mixed signs and zero
    assert(smallestOfTwo(0, -1) == -1);
    assert(smallestOfTwo(0, 5) == 0);
    
    // Large magnitude integers
    assert(smallestOfTwo(2147483647, -2147483647) == -2147483647);
    
    // Random check
    assert(smallestOfTwo(42, 17) == 17);
    
    return 0;
}
// The solution uses the conditional operator `a < b ? a : b`. The condition `a < b` is evaluated; if it is true, the expression evaluates to `a` (which is the smaller), otherwise to `b`. This directly implements the minimum operation. Edge cases: if the numbers are equal, the condition is false, so `b` is returned, which equals `a`; this is correct because either value is the minimum. Negative and positive numbers are handled naturally by the relational operator. The algorithm has constant time complexity \(O(1)\) and constant auxiliary space \(O(1)\) because it only performs a single comparison and returns a primitive integer. There are no loops, recursion, or dynamic allocations.
