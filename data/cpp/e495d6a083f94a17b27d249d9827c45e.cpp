// Write a C++ function named `classifyParity` that takes a single integer `n` (representing the number of iterations) and returns a string. If `n` is even, return `"even"`; if `n` is odd, return `"odd"`. However, if `n` is less than 0, return `"negative"`. The function must be pure (no side effects), const-correct, and use only standard library headers. The task is to implement this standalone function without a `main` wrapper.
// The solution is a straightforward conditional check. The main algorithm: first, check if the input is negative; if so, return `"negative"`. Otherwise, check if the number is divisible by 2 using the modulo operator `%`. Note: In C++, negative numbers with `%` can be tricky, but since we handle negatives first, we avoid any issues. Edge cases: the number 0 is even (return `"even"`), very large integers are handled by using `long long`? The task says integer, but to be safe use `int` as specified. Time complexity is O(1), space complexity O(1) because we return a constant-size string.
#include <string>

// Returns a string describing the parity of the given integer.
// Negative numbers are labeled "negative"; otherwise "even" or "odd".
std::string classifyParity(const int n) {
    if (n < 0) {
        return "negative";
    }
    if (n % 2 == 0) {
        return "even";
    }
    return "odd";
}
#include <cassert>
#include <string>

// Function under test (provided here for completeness in test file)
std::string classifyParity(const int n) {
    if (n < 0) {
        return "negative";
    }
    if (n % 2 == 0) {
        return "even";
    }
    return "odd";
}

int main() {
    assert(classifyParity(0) == "even");
    assert(classifyParity(2) == "even");
    assert(classifyParity(-2) == "negative");
    assert(classifyParity(7) == "odd");
    assert(classifyParity(-7) == "negative");
    assert(classifyParity(1000000) == "even");
    assert(classifyParity(999999) == "odd");
    return 0;
}
