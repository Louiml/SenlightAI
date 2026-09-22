/*
Write a C++ function named `factorial` that takes a single non-negative integer `n` as input and returns its factorial as an unsigned 64-bit integer (`std::uint64_t`). The function must handle edge cases gracefully: if `n` is 0 or 1, return 1; if the factorial would overflow 64 bits (i.e., for `n` > 20), throw an `std::overflow_error` exception with a descriptive message. Do not rely on global variables or external state; the function must be self-contained and usable in any program. For example, `factorial(5)` should return 120, and `factorial(0)` should return 1.
*/
#include <cstdint>   // for std::uint64_t
#include <stdexcept> // for std::overflow_error
#include <limits>    // for std::numeric_limits

// Compute the factorial of a non-negative integer n.
// Returns n! as a 64-bit unsigned integer.
// Throws std::overflow_error if the result exceeds 64-bit range (n > 20).
std::uint64_t factorial(int n) {
    if (n < 0) {
        throw std::invalid_argument("factorial: input must be non-negative");
    }

    std::uint64_t result = 1;
    const std::uint64_t max = std::numeric_limits<std::uint64_t>::max();

    for (int i = 2; i <= n; ++i) {
        // Check for overflow before multiplying: if result > max / i, then result * i overflows.
        if (result > max / static_cast<std::uint64_t>(i)) {
            throw std::overflow_error("factorial: result overflows 64-bit integer");
        }
        result *= static_cast<std::uint64_t>(i);
    }
    return result;
}
#include <cassert>
#include <cstdint>
#include <stdexcept>

// Declaration of the factorial function (should be defined in the same translation unit or included)
std::uint64_t factorial(int n);

int main() {
    // Basic cases
    assert(factorial(0) == 1ULL);
    assert(factorial(1) == 1ULL);
    assert(factorial(2) == 2ULL);
    assert(factorial(5) == 120ULL);
    assert(factorial(10) == 3628800ULL);
    assert(factorial(20) == 2432902008176640000ULL); // 20! fits in 64-bit

    // Overflow cases
    bool threw = false;
    try {
        factorial(21);
    } catch (const std::overflow_error&) {
        threw = true;
    }
    assert(threw && "factorial(21) should throw overflow_error");

    // Negative input (optional test, if you added that check)
    threw = false;
    try {
        factorial(-1);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "factorial(-1) should throw invalid_argument");

    return 0;
}
// The algorithm computes the product of all integers from 1 to `n` iteratively. Initialize a result variable to 1, then for each `i` from 1 to `n`, multiply the result by `i`. The main edge cases are: (1) `n` = 0, where the loop body never executes and the result stays 1 (this is correct because 0! = 1); (2) `n` = 1, where the loop runs once and multiplies by 1, giving 1; (3) overflow, which occurs when `n` > 20 because 21! = 51,090,942,171,709,440,000 exceeds the maximum value of `std::uint64_t` (18,446,744,073,709,551,615). To detect overflow before it happens, we check if multiplying the current result by `i` would exceed the maximum representable value: since `result` and `i` are both positive, overflow can be detected by checking if `result > max / i` before doing the multiplication. If overflow is detected, throw an `std::overflow_error`. Time complexity is O(n) for the loop, and space complexity is O(1) because only a constant number of variables are used.
