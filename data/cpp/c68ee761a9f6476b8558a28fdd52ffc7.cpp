// Write a C++ function named `classifyNumber` that takes a single integer argument `n` and returns a `std::string` indicating whether the number is even or odd. The returned string must follow the exact format: if `n` is even, return `"even"`; if `n` is odd, return `"odd"`. The function must not read from standard input or output anything; it should only return the classification string. The function should be correct for all valid `int` values, including negative numbers, zero, and the extremes of the `int` range. Assume the input is always a valid integer; no error handling is needed for non-integer input. The function must be declared with appropriate `const` correctness where applicable (e.g., the parameter can be passed by value, and the return type is `std::string`).

#include <cassert>
#include <string>

// Declare the function (in a real project, include the header).
std::string classifyNumber(const int n);

int main() {
    // Basic even and odd
    assert(classifyNumber(4) == "even");
    assert(classifyNumber(7) == "odd");
    
    // Zero is even
    assert(classifyNumber(0) == "even");
    
    // Negative numbers
    assert(classifyNumber(-2) == "even");
    assert(classifyNumber(-3) == "odd");
    
    // Extremes of int range
    assert(classifyNumber(2147483647) == "odd");   // largest int
    assert(classifyNumber(-2147483647) == "odd");  // closest to INT_MIN in magnitude
    
    // Large even numbers
    assert(classifyNumber(2147483646) == "even");
    assert(classifyNumber(-2147483646) == "even");
    
    // One and negative one
    assert(classifyNumber(1) == "odd");
    assert(classifyNumber(-1) == "odd");
    
    // A few random checks
    assert(classifyNumber(100) == "even");
    assert(classifyNumber(101) == "odd");
    
    return 0;
}

#include <string>

// Return "even" if n is even, "odd" otherwise.
std::string classifyNumber(const int n) {
    return (n % 2 == 0) ? "even" : "odd";
}

// The solution is straightforward: to determine if an integer is even or odd, compute the remainder when divided by 2 using the modulo operator `%`. If the remainder is zero, the number is even; otherwise, it is odd. This works for negative numbers as well because in C++, the result of `n % 2` for negative `n` is either `0` (if even) or `-1` (if odd), both of which are non-zero for odd numbers. Zero is even because `0 % 2 == 0`. The algorithm performs a single modulo operation and a comparison, so it runs in \(O(1)\) time and uses \(O(1)\) auxiliary space. No edge cases require special handling beyond the modulo condition; however, note that for negative odd numbers, the remainder is `-1`, which is non-zero, so the condition `n % 2 == 0` correctly returns "even" only for even numbers. The function can be implemented using a ternary operator for conciseness, similar to the inspiration code, but it must return the string instead of printing.
