// Write a C++ function `classifyPositiveNumbers` that takes a single integer `n` as input and returns a string. If `n` is between 1 and 9 inclusive (i.e., a positive single-digit number), the function should return the exact message `"What an obedient servant you are!"` (without quotes). Otherwise, if `n` is less than 1 or greater than 9, the function should return the string `"-1"`. The function must handle all integer values (including negative, zero, and very large positive numbers) without any special-case exceptions. The message strings must match exactly, including capitalization, spaces, and punctuation.
#include <cassert>
#include <string>

// Declare the function being tested (in practice, include the header or paste above).
std::string classifyPositiveNumbers(int n);

int main() {
    // Success cases: 1 through 9
    assert(classifyPositiveNumbers(1) == "What an obedient servant you are!");
    assert(classifyPositiveNumbers(5) == "What an obedient servant you are!");
    assert(classifyPositiveNumbers(9) == "What an obedient servant you are!");

    // Failure cases: zero, negative, and ≥10
    assert(classifyPositiveNumbers(0) == "-1");
    assert(classifyPositiveNumbers(-1) == "-1");
    assert(classifyPositiveNumbers(-100) == "-1");
    assert(classifyPositiveNumbers(10) == "-1");
    assert(classifyPositiveNumbers(1000000) == "-1");

    return 0;
}
#include <string>

// Returns the success message if n is a positive single-digit number (1-9),
// otherwise returns "-1".
std::string classifyPositiveNumbers(int n) {
    if (n > 0 && n < 10) {
        return "What an obedient servant you are!";
    }
    return "-1";
}
// The core logic is a simple range check: if `0 < n && n < 10`, return the success message; otherwise return `"-1"`. Edge cases to handle carefully: zero, negative numbers, and numbers ≥10 should all return `"-1"`. There are no overflow concerns because the input is a 32-bit integer and the check uses simple comparisons. The time complexity is O(1) because only one comparison is performed regardless of input value, and space complexity is O(1) as no additional memory is used beyond the returned string constant. The function should be `const`-correct by taking the parameter by value (an `int`) and returning a `std::string`; no state is modified.
