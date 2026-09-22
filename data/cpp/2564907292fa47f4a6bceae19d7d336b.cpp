// Write a C++ function that takes an integer input and returns its doubled value as an integer. The function should be named `doubleValue` and accept a single `int` parameter by value. It must be a pure function with no side effects, meaning it should not read from standard input, write to standard output, or modify any external state. The function must use `const` correctness where appropriate and return the result directly. The task is to implement the logic, not to handle input/output — that will be done externally by the test harness.
The solution is straightforward: given an integer `x`, the doubled value is `x * 2`. Since the parameter is passed by value, no reference issues arise, and we can safely use `const int` for the parameter to indicate it will not be modified (though passing by value already implies this). The function returns `int` directly. There are no edge cases other than potential integer overflow, which is a concern only for extremely large `int` values; assuming standard 32-bit `int`, the input size is not specified, but we can ignore overflow for typical test values. The time complexity is \(O(1)\) and space complexity is \(O(1)\). The function is trivially correct for all `int` inputs within the representable range that do not cause overflow when doubled.
#include <cstdint> // not needed, but included for clarity (optional)

// Return the doubled value of the given integer.
int doubleValue(const int value) {
    return value * 2;
}
#include <cassert>

// local declaration of the solution function
int doubleValue(const int value);

int main() {
    assert(doubleValue(0) == 0);
    assert(doubleValue(1) == 2);
    assert(doubleValue(-3) == -6);
    assert(doubleValue(10) == 20);
    assert(doubleValue(-100) == -200);
    assert(doubleValue(12345) == 24690);
    assert(doubleValue(-1) == -2);
    assert(doubleValue(2) == 4);
    assert(doubleValue(1000) == 2000);
    assert(doubleValue(-500) == -1000);
    return 0;
}
