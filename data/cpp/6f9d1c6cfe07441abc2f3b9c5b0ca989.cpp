Write a C++ function named `printNumbersUpTo` that takes a single non-negative integer `maxNumber` and returns a `std::string` containing the integers from 1 up to and including `maxNumber`, each on its own line (i.e., separated by `\n`). If `maxNumber` is 0, the function must return an empty string. The function must use a `do-while` loop to generate the output, not a `for` or `while` loop.

#include <cassert>
#include <string>

// The solution function declaration (for clarity)
std::string printNumbersUpTo(int maxNumber);

int main() {
    assert(printNumbersUpTo(0) == "");
    assert(printNumbersUpTo(1) == "1\n");
    assert(printNumbersUpTo(3) == "1\n2\n3\n");
    assert(printNumbersUpTo(5) == "1\n2\n3\n4\n5\n");
    assert(printNumbersUpTo(-2) == "");
    return 0;
}

#include <string>

// Returns a string with numbers from 1 to maxNumber, each on a new line.
std::string printNumbersUpTo(int maxNumber) {
    if (maxNumber < 1) {
        return "";
    }

    std::string result;
    int i = 1;
    do {
        result += std::to_string(i);
        result += '\n';
        ++i;
    } while (i <= maxNumber);

    return result;
}

// The function should initialize a counter to 1 and use a `do-while` loop to append the current counter and a newline character to a string, then increment the counter, and repeat while the counter is less than or equal to `maxNumber`. Edge cases: For `maxNumber = 0`, the condition `while (i <= 0)` is false from the start, so the loop body (which would print `1`) must never execute — but since it's a `do-while`, the body executes once before checking. To handle this, we can start the loop with `i=1` and use a check before the loop: if `maxNumber == 0`, return empty string immediately. Alternatively, place the condition check inside but ensure the body prints only if `i <= maxNumber`. The cleanest is to guard with an early return for `maxNumber == 0`. Time complexity is O(n) because we append each number once, and space complexity is O(n) for the resulting string (since the output length grows with the number of digits and newlines). The function should use `std::to_string` or string stream to convert integers.
