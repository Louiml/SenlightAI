Write a C++ function named `findMinOverload` that takes two `int` parameters and returns the smaller of the two values. The function must be defined in a header file named `OurMin.h` (which you should create in your solution) and must be callable from another translation unit. Additionally, write a separate free function `describeMin` that takes two `int` parameters by value and returns a `std::string` in the format `"min(a,b) = X"` where `a` and `b` are the original values and `X` is the smaller value. The task is to implement both functions in the same header, with `findMinOverload` marked as `inline` to avoid multiple-definition errors when included in multiple source files. The `describeMin` function should use `findMinOverload` internally and must be `const`-correct (though parameters are passed by value, so `const` applies to the function itself if it were a member; here just ensure proper use of `const` in the function signature and local variables). The provided code snippet is a starting point that calls `min(1,2)`; you are to replace that with calls to your `findMinOverload` in a separate `.cpp` file (not required for the solution code, but the solution must include the header definition). The solution must be self-contained in the sense that the header should only rely on standard C++ headers and should not include any non‑standard dependencies.

The core problem is to provide a reusable, inline function that returns the minimum of two integers, and a helper that formats a descriptive string. The main algorithm is trivial: compare the two integers with `<` and return the smaller. Edge cases include equal values, where either can be returned; the result is identical either way. Negative numbers and large magnitudes are handled naturally by the `int` type. The `describeMin` function concatenates strings using `std::to_string` for the values and the result; it must ensure correct formatting with a space between the comma and the second value. The `findMinOverload` must be `inline` to avoid ODR violations when the header is included in multiple translation units. Time complexity is O(1) for each call, space complexity O(1) for `findMinOverload` and O(n) for the returned string in `describeMin`, where n is the number of digits in the integers (negligible). No dynamic memory is used except that which `std::string` manages internally.

#ifndef OURMIN_H
#define OURMIN_H

#include <string>

// Return the smaller of two integers.
inline int findMinOverload(int a, int b) {
    return (a < b) ? a : b;
}

// Return a string describing the minimum of two integers.
std::string describeMin(int a, int b) {
    int result = findMinOverload(a, b);
    return "min(" + std::to_string(a) + "," + std::to_string(b) + ") = " + std::to_string(result);
}

#endif

#include <cassert>
#include <string>
#include "OurMin.h"

int main() {
    // Basic cases
    assert(findMinOverload(1, 2) == 1);
    assert(findMinOverload(2, 1) == 1);
    assert(findMinOverload(-5, 3) == -5);
    assert(findMinOverload(-3, -5) == -5);

    // Equal values
    assert(findMinOverload(7, 7) == 7);
    assert(findMinOverload(0, 0) == 0);

    // describeMin formatting
    assert(describeMin(1, 2) == "min(1,2) = 1");
    assert(describeMin(2, 1) == "min(2,1) = 1");
    assert(describeMin(-5, 3) == "min(-5,3) = -5");
    assert(describeMin(7, 7) == "min(7,7) = 7");

    return 0;
}
