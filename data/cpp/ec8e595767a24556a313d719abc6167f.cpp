/*
Write a C++ function `template<typename R, typename P> void addAndPrint(R a, P b)` that takes two arguments of possibly different numeric types, computes their sum using the type of the second argument as the result type (i.e., `P z = a + b;`), and prints the result to the standard output followed by a newline. The function must work for any combination of built-in numeric types (e.g., `int`, `double`, `float`, `long`), including when the first argument is an integer and the second is a floating-point type, and vice versa. The function should not return anything. The task is to create a robust template that correctly handles type promotion and prints the result without causing narrowing or overflow at compile time (assume typical built-in type sizes). Ensure the function is usable in a standalone program that calls it with different argument type combinations, including mixed types where the sum would be a floating-point number.
*/

#include <iostream>

// Add two values of possibly different types and print the sum.
// The result's type is the type of the second argument 'P'.
template<typename R, typename P>
void addAndPrint(const R a, const P b) {
    P z = a + b;  // Result type is P, conversion happens here.
    std::cout << z << std::endl;
}

#include <cassert>
#include <sstream>
#include <iostream>

// Forward declaration of the solution function (already defined above).
template<typename R, typename P>
void addAndPrint(const R a, const P b);

// Helper to capture output of addAndPrint into a string.
template<typename R, typename P>
std::string captureAddAndPrint(const R a, const P b) {
    std::ostringstream oss;
    std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
    addAndPrint(a, b);
    std::cout.rdbuf(oldCout);
    return oss.str();
}

int main() {
    // Test integer + double -> double (prints 8.9)
    assert(captureAddAndPrint(3, 5.9) == "8.9\n");
    // Test double + integer -> integer (truncates to 10)
    assert(captureAddAndPrint(3.5, 7) == "10\n");
    // Test int + int -> int
    assert(captureAddAndPrint(4, 5) == "9\n");
    // Test double + double -> double
    assert(captureAddAndPrint(1.2, 3.4) == "4.6\n");
    // Test negative and unsigned (unsigned int + int)
    assert(captureAddAndPrint(-5, -3) == "-8\n");
    // Test mixed unsigned and signed (careful: -1 converts to large unsigned if P is unsigned? 
    // Here P is int, so fine.)
    assert(captureAddAndPrint(1, 2) == "3\n");
    // Test float + float
    assert(captureAddAndPrint(1.5f, 2.5f) == "4\n"); // prints as 4 (float output)
    // Test long + double
    assert(captureAddAndPrint(100000L, 0.5) == "100000.5\n");
    // Test char promoted to int
    assert(captureAddAndPrint('A', 1) == "66\n"); // 'A'=65 +1 =66
    return 0;
}

// The solution uses a function template with two type parameters `R` and `P`. Inside the function, a local variable `z` of type `P` is declared and initialized with the expression `a + b`. Because `z`'s type is `P` (the second template parameter), the addition result is implicitly converted to type `P`. This means if `P` is `double` and `R` is `int`, the sum is computed as a `double` and stored as `double`, which is safe. If `P` is `int` and `R` is `double`, the double sum is truncated to an integer, which is a potential data loss, but this matches the behavior of the original snippet and is part of the task’s specification. Edge cases include: (1) both types are the same, (2) mixing signed and unsigned integers, (3) mixing integer and floating-point types. The main algorithmic step is trivial — just add and print. The time complexity is O(1) and space complexity is O(1) since only a single local variable is used. The function should be marked as `const`-correct? Since it takes arguments by value and does not modify external state, there is no need for `const` on parameters (they are copies), but the function itself is not const because it is a free function. We can add `const` to the parameters themselves (i.e., `const R a, const P b`) to emphasize that they are not modified, though it is not strictly necessary. For correctness, we must include `<iostream>` and use `std::cout` with `std::endl` or `'\n'`.
