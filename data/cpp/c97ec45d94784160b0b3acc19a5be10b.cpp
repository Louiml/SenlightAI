// Write a C++ function named `numbersNotDivisibleBy` that takes two positive integers, `limit` and `divisor`, and returns a string containing all integers from 1 to `limit` (inclusive) that are **not divisible** by `divisor`, separated by single spaces, in ascending order. If no numbers qualify (e.g., `limit < 1`), return an empty string. Do not print anything to the console inside the function. Include appropriate `const` correctness for parameters.
// The solution iterates through every integer from 1 to `limit` using a simple `for` loop. For each number, check whether it is divisible by `divisor` using the modulo operator (`%`). If divisible, skip it (using `continue`); otherwise, append the number to the result string, adding a space before each number except the first to avoid trailing spaces. Edge cases: `limit` could be 0 or negative (return empty string), `divisor` could be 1 (then all numbers are skipped, returning empty string), and `divisor` could be larger than `limit` (then all numbers qualify). Time complexity is O(`limit`) because we examine each number once, and the string concatenation is O(1) per number on average (amortized). Space complexity is O(`limit`) because the output string holds up to `limit` numbers as text.
#include <string>

// Return a string with numbers from 1 to limit (inclusive) that are not divisible by divisor.
std::string numbersNotDivisibleBy(int limit, int divisor) {
    std::string result;
    bool first = true;

    for (int i = 1; i <= limit; ++i) {
        if (i % divisor == 0) {
            continue;
        }
        if (!first) {
            result += ' ';
        }
        result += std::to_string(i);
        first = false;
    }

    return result;
}
#include <cassert>
#include <string>

// Include the solution function here (or via header)

int main() {
    // Basic case: limit 10, divisor 3 → 1 2 4 5 7 8 10
    assert(numbersNotDivisibleBy(10, 3) == "1 2 4 5 7 8 10");

    // Divisor 1 → all numbers skipped → empty string
    assert(numbersNotDivisibleBy(5, 1) == "");

    // Divisor larger than limit → all numbers included
    assert(numbersNotDivisibleBy(4, 10) == "1 2 3 4");

    // Limit 1 → only number 1 (if divisor != 1)
    assert(numbersNotDivisibleBy(1, 2) == "1");

    // Limit 0 or negative → empty string
    assert(numbersNotDivisibleBy(0, 3) == "");
    assert(numbersNotDivisibleBy(-5, 3) == "");

    // Divisor 2 → all odd numbers
    assert(numbersNotDivisibleBy(7, 2) == "1 3 5 7");

    // Divisor equal to limit → only numbers < limit appear
    assert(numbersNotDivisibleBy(6, 6) == "1 2 3 4 5");
}
