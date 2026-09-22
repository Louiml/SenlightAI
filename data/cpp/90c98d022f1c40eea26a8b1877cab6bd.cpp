// Write a C++ function named `safeDivisionResult` that takes two integer parameters `dividend` and `divisor`, and returns an integer. The function must attempt to compute `dividend / divisor` using integer division. If the divisor is zero, the function must rethrow an exception (of any type) after catching it, so that the caller can handle the division-by-zero case. The function signature must be `int safeDivisionResult(int dividend, int divisor)` and should not print anything on its own. The caller will be responsible for catching the exception and printing `"UNDEF"` when a division by zero occurs. Ensure the function uses exception handling internally, throws on zero divisor, and passes the exception up the call stack.
The solution approach is straightforward: implement `safeDivisionResult` that wraps the division operation in a `try` block. Inside the `try`, check if `divisor == 0`; if so, `throw` an integer (e.g., `1`) to signal an error. Otherwise, return `dividend / divisor`. In the `catch(...)` block, simply `throw;` to rethrow the caught exception unchanged, ensuring the caller sees the exception. Edge cases: only zero divisor causes an exception; negative divisors and dividends work normally with C++ integer division (truncation toward zero). The function does not handle overflow (e.g., `INT_MIN / -1`) because that is undefined behavior in C++, but the task does not require it. Time complexity: O(1). Space complexity: O(1) excluding the call stack overhead for exception handling.
#include <stdexcept> // Not strictly needed but for completeness

// Compute dividend / divisor, throwing an exception if divisor is zero.
// The exception is caught and immediately rethrown to propagate it to the caller.
int safeDivisionResult(int dividend, int divisor) {
    try {
        if (divisor == 0) {
            throw 1; // Arbitrary exception value to indicate division by zero
        }
        return dividend / divisor;
    }
    catch (...) {
        throw; // Rethrow the original exception unchanged
    }
}
#include <cassert>
#include <iostream>

// Declaration of the solution function (in a real test, link against the solution)
int safeDivisionResult(int dividend, int divisor);

int main() {
    // Normal divisions
    assert(safeDivisionResult(10, 2) == 5);
    assert(safeDivisionResult(7, 3) == 2); // Integer truncation
    assert(safeDivisionResult(-7, 3) == -2); // Truncation toward zero
    assert(safeDivisionResult(0, 5) == 0);

    // Division by zero must throw
    bool threw = false;
    try {
        safeDivisionResult(1, 0);
    } catch (...) {
        threw = true;
    }
    assert(threw);

    // Negative divisor with zero numerator
    threw = false;
    try {
        safeDivisionResult(0, 0);
    } catch (...) {
        threw = true;
    }
    assert(threw);

    // Negative divisors work normally
    assert(safeDivisionResult(10, -2) == -5);
    assert(safeDivisionResult(-10, -2) == 5);

    // Large values (no overflow for normal division)
    assert(safeDivisionResult(1000000, 10) == 100000);

    // Ensure exception is rethrown (not swallowed) by verifying catch(...) sees it
    threw = false;
    try {
        safeDivisionResult(42, 0);
    } catch (int e) {
        threw = true; // Catch the specific int throw
    }
    assert(threw);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
