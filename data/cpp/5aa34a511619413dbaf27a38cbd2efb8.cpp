// Write a C++ function named `printMaximum` that takes two parameters of potentially different types (e.g., `int` and `float`, or any two arithmetic types that support the `>` operator) and prints to standard output the larger of the two values using `cout`, followed by a newline via `endl`. The function must be a template that deduces the types of both arguments automatically. It should handle cases where values are equal (printing either one is acceptable, but the output should match the expected format exactly: `max = <value>`). The function should not return any value; it should only perform the output. Additionally, ensure the solution works with built-in numeric types like `int`, `double`, `float`, `short`, `long`, etc.

The core requirement is to create a generic function that works with two arbitrary types. Use a function template with two template parameters `typename A` and `typename B` to accept parameters of different types. Inside the function, compare the two values using the `>` operator. If `a > b` is true, display `max = a`; otherwise, display `max = b`. Since `a` and `b` might be of different types, the comparison is performed after implicit type conversion (e.g., `int` vs `float` promotes `int` to `float`). The output uses `cout` and `endl` for a newline. Edge cases: equal values—the `else` branch is taken, printing `b`, which is valid because both are equal. No special handling needed for negative numbers or zero. Time complexity is O(1) since it's a single comparison and output. Space complexity is O(1). The test cases should cover same-type and different-type calls, including equal values and floating-point precision.

#include <iostream>

// Print the maximum of two values of possibly different types.
// Uses the > operator and outputs "max = <value>" followed by a newline.
template <typename A, typename B>
void printMaximum(const A& a, const B& b) {
    if (a > b) {
        std::cout << "max = " << a << std::endl;
    } else {
        std::cout << "max = " << b << std::endl;
    }
}

#include <cassert>
#include <sstream>
#include <string>

// Helper to capture output from printMaximum into a string.
template <typename A, typename B>
std::string capturePrintMaximum(const A& a, const B& b) {
    std::ostringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
    printMaximum(a, b);
    std::cout.rdbuf(oldCout);
    return buffer.str();
}

int main() {
    // Same types
    assert(capturePrintMaximum(10, 20) == "max = 20\n");
    assert(capturePrintMaximum(20, 10) == "max = 20\n");
    assert(capturePrintMaximum(5, 5) == "max = 5\n");

    // Different types
    assert(capturePrintMaximum(10, 19.5f) == "max = 19.5\n");
    assert(capturePrintMaximum(25.5, 10) == "max = 25.5\n");
    assert(capturePrintMaximum(-5.5f, -10) == "max = -5.5\n");

    // Equal different types
    assert(capturePrintMaximum(10, 10.0f) == "max = 10\n");
    
    // Larger numeric types
    assert(capturePrintMaximum(10000000000LL, 9999999999LL) == "max = 10000000000\n");
    assert(capturePrintMaximum(2.5, 3.14159) == "max = 3.14159\n");

    return 0;
}
