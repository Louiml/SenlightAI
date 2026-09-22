// Write a C++ function named `swapIntegersByValue` that takes two integer parameters, prints the values before the swap, swaps the values internally, prints the values after the swap, and returns nothing. The function must demonstrate that value semantics do not affect the caller’s variables. After the function, the caller should confirm that the original integers remain unchanged. Additionally, the function must be implemented in a separate header file (`swapIntegers.hpp`) and source file (`swapIntegers.cpp`) following the principle of separate compilation, and the task must include a proper function declaration in the header and a definition in the source file, with the main program including only the header. The solution should avoid using any external swap utility and must use a temporary variable to perform the swap.

// The core is a value-swap function that prints before/after states and uses a local temporary. Since parameters are passed by value, any modifications inside the function operate on copies, leaving caller variables untouched—this is the key edge case to verify. The main algorithm: (1) print original values, (2) create a temporary variable to hold one value, (3) assign the second to the first, (4) assign the temporary to the second, (5) print swapped values. No arithmetic trickery or `std::swap` is allowed. The function is `void` and does not return anything; it must be declared in the header with `const`-correctness? Actually `const` is not needed because we are modifying the copies, but the parameters themselves are not `const`—they are plain `int` by value. However, the function signature is `void swapIntegersByValue(int a, int b)`. For time complexity: O(1) for the swap and prints—constant. Space complexity: O(1) for local variables (one temporary, plus the two parameters are copies). Important edge cases: negative numbers, zero, and identical values—all work trivially. The separate compilation requires the header to contain only the declaration, the source to include the header and define the function, and the test file to include only the header (not the source). The test confirms that after calling the function, the caller’s original `a` and `b` retain their initial values.

// swapIntegers.hpp
#ifndef SWAP_INTEGERS_HPP
#define SWAP_INTEGERS_HPP

// Declaration: swaps two integers internally, but does not affect caller's values.
void swapIntegersByValue(int a, int b);

#endif
// swapIntegers.cpp
#include "swapIntegers.hpp"
#include <iostream>

// Definition: prints before, swaps using a temporary, prints after.
void swapIntegersByValue(int a, int b) {
    std::cout << "Before swap: a = " << a << ", b = " << b << std::endl;
    int temp = a;
    a = b;
    b = temp;
    std::cout << "After swap:  a = " << a << ", b = " << b << std::endl;
}

// test_swap.cpp
#include "swapIntegers.hpp"
#include <cassert>
#include <iostream>

int main() {
    // Test 1: typical positive values
    int a1 = 10, b1 = 20;
    swapIntegersByValue(a1, b1);
    assert(a1 == 10 && b1 == 20);

    // Test 2: negative values
    int a2 = -5, b2 = -3;
    swapIntegersByValue(a2, b2);
    assert(a2 == -5 && b2 == -3);

    // Test 3: identical values
    int a3 = 7, b3 = 7;
    swapIntegersByValue(a3, b3);
    assert(a3 == 7 && b3 == 7);

    // Test 4: zero and a positive
    int a4 = 0, b4 = 42;
    swapIntegersByValue(a4, b4);
    assert(a4 == 0 && b4 == 42);

    // Test 5: one large, one small
    int a5 = 1000000, b5 = -1000000;
    swapIntegersByValue(a5, b5);
    assert(a5 == 1000000 && b5 == -1000000);

    std::cout << "All tests passed." << std::endl;
    return 0;
}
