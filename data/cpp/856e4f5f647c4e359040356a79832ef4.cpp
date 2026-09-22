/*
Write a C++ function named `printAsteriskLine` that takes a single non-negative integer parameter `n` and returns a `std::string` containing exactly `n` asterisk characters (`*`) with no newline or other characters. If `n` is zero, the function must return an empty string. The function should be pure (no I/O, no side effects) and must handle the maximum possible value of `n` that can be stored in an `int` without overflow or undefined behavior (i.e., use `std::string::size_type` for counting or concatenation). The function must be `const`-correct and well-documented.
*/

#include <string>

// Returns a string containing exactly n asterisks.
// Precondition: n >= 0.
// If n == 0, returns an empty string.
std::string printAsteriskLine(const int n) {
    // Directly construct a string of length n filled with '*'.
    // For n = 0, this creates an empty string.
    return std::string(n, '*');
}

#include <cassert>
#include <string>

// Function declaration (as in the solution).
std::string printAsteriskLine(const int n);

int main() {
    // Basic cases
    assert(printAsteriskLine(5) == "*****");
    assert(printAsteriskLine(1) == "*");
    assert(printAsteriskLine(0) == "");

    // Larger and boundary-like values
    assert(printAsteriskLine(10) == "**********");
    assert(printAsteriskLine(3) == "***");
    assert(printAsteriskLine(2) == "**");

    // Verify length and content for a moderate number
    const std::string result = printAsteriskLine(100);
    assert(result.size() == 100);
    assert(result.find_first_not_of('*') == std::string::npos);

    // Extreme value within int range (ensures no overflow)
    const std::string big = printAsteriskLine(1000000);
    assert(big.size() == 1000000);
    assert(big[0] == '*' && big[999999] == '*');

    return 0;
}

// The task is to generate a string of repeated asterisks. The simplest approach is to use a loop that appends one `*` to a `std::string` for each iteration from 0 to `n-1`. Since `n` is guaranteed non-negative, we don't need to check for negatives, but the zero case naturally returns an empty string because the loop body never executes. Edge cases include `n = 0` (empty output) and very large `n` near `INT_MAX`; using `std::string` and appending in a loop is safe because `std::string` manages dynamic memory. However, for performance and clarity, we can use `std::string(n, '*')` which directly constructs a string of length `n` filled with `*`. This approach is simpler and avoids potential concerns about repeated reallocation in the loop. The time complexity is O(n) because the string must be created with that many characters, and space complexity is O(n) for the resulting string. Auxiliary space beyond the output is O(1) when using the constructor.
