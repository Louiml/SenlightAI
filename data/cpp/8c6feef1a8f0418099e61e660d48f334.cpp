Write a standalone C++ function named `sumUsingPointers` that accepts two integers by value and returns their sum, but the function must internally store the two parameters in separate local variables and compute the sum by dereferencing pointers to those local variables. The function should demonstrate explicit use of pointers for basic arithmetic, and it must be `const`-correct by taking the parameters as `const int` references (or by value) and using `const int*` pointers internally where appropriate. The function must not use any global variables or input/output operations; it should simply return the computed sum.

The solution approach is straightforward: create two local `int` variables, assign the function parameters to them, then declare two `const int*` pointers that point to those local variables. Compute the sum by dereferencing both pointers and adding the values. The key point is to ensure the pointers are properly initialized to the addresses of the local variables using the address-of operator. Edge cases are minimal because the input are plain `int`s, so overflow is possible but is standard behavior for C++ `int` arithmetic (no special handling required). Time complexity is \(O(1)\) because the function performs a fixed number of operations regardless of input size. Space complexity is also \(O(1)\) because only a constant number of local variables and pointers are used. The function should be marked `const`-correct by using `const int*` (pointers to constant data) to indicate that the pointed-to values are not modified. The parameters can be passed by value since the task specifies "by value", but to be more const-correct, we can take them as `const int` parameters (by value is fine). The function signature should be `int sumUsingPointers(int a, int b)`.

#include <cstddef> // not strictly needed, but included for completeness

// Compute the sum of two integers using pointers to local copies.
int sumUsingPointers(const int a, const int b) {
    int localA = a; // local copies to have addresses
    int localB = b;
    
    const int* ptrA = &localA; // pointer to constant int
    const int* ptrB = &localB;
    
    return *ptrA + *ptrB;
}

#include <cassert>

int sumUsingPointers(const int a, const int b);

int main() {
    // Basic positives
    assert(sumUsingPointers(3, 4) == 7);
    // Negative numbers
    assert(sumUsingPointers(-5, 10) == 5);
    // Zero and negative
    assert(sumUsingPointers(0, -7) == -7);
    // Both negative
    assert(sumUsingPointers(-3, -2) == -5);
    // Large values (potential overflow not checked, but typical)
    assert(sumUsingPointers(100000, 200000) == 300000);
    // Same value
    assert(sumUsingPointers(42, 42) == 84);
    // Zero and positive
    assert(sumUsingPointers(0, 0) == 0);
    return 0;
}
